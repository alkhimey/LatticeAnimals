import cgi
import datetime
import webapp2
import logging

import os
import math
import random
from datetime import datetime, timedelta
from collections import defaultdict


from google.appengine.ext.webapp import template
from google.appengine.ext import db
from google.appengine.api import users

# Precomputed values using 'all' predicate
precomputed = {1 : 1,
               2 : 3,
               3 : 15,
               4 : 86,
               5 : 534,
               6 : 3481,
               7 : 23502,
               8 : 162913,
               9 : 1152870,
               10: 8294738 }

class Config:
  client_version = 1.2     	# Which client version the server is able to communicate with.
  
  pred_name = "full-convex" 	# Which predicae should the client use.  
  n0 = 6		   	# At wich size we want to split the counting.
  n = 10       			# Polycube size we want to count.

  max_id = precomputed[n0] 	# Total Number of policubes of size n0. Determined by pre-running Redelmier sequentially with pred=all.
  num_of_jobs = 300 		# To how many jobs we want to split the counting.
  secret_hash_length = 32
  password = 'GreenSwimming' 	# Password for protected features  
  default_batch_size = 50 	# When jobs are created in batches.
  

class Job(db.Model):
  secret_hash = db.StringProperty(multiline=True)
  n = db.IntegerProperty()
  n0 = db.IntegerProperty()
  low_id = db.IntegerProperty()
  hight_id = db.IntegerProperty()
  date_created = db.DateTimeProperty(auto_now_add=True)
  date_allocated = db.DateTimeProperty()
  date_reported = db.DateTimeProperty()
  ip_allocated = db.StringProperty(multiline=False)

class JobResult(db.Model):
  job = db.ReferenceProperty(Job, collection_name='results')
  size =  db.IntegerProperty()
  result =  db.IntegerProperty()

class DataCounter(db.Model):
  created_jobs = db.IntegerProperty()
  created_ids = db.IntegerProperty()
  allocated_ids = db.IntegerProperty()
  reported_ids = db.IntegerProperty()
  date_started = db.DateTimeProperty(auto_now_add=True)
  date_last = db.DateTimeProperty()
  cpu_time = db.FloatProperty()

  @staticmethod
  def get():
    c = db.GqlQuery("SELECT * FROM DataCounter LIMIT 1").get()
    if c == None:
      c = DataCounter()
      c.created_jobs = 0
      c.created_ids = 0
      c.allocated_ids = 0
      c.reported_ids = 0
      c.date_last = None
      c.cpu_time = 0.0
    return c

class ResultsCounter(db.Model):
  size = db.IntegerProperty()
  result = db.IntegerProperty()
  
  @staticmethod
  def get(size):
    res = db.GqlQuery("SELECT * FROM ResultsCounter WHERE size = %d LIMIT 1" % size).get()
    if res == None:
      res = ResultsCounter()
      res.size = size
      res.result = 0
    return res
    

def gen_random_word(length):
  word = ''
  for i in range(length):
    word += random.choice('ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789')
  return word
  

def gen_key(low, hight):
  return "%d_%d_%d_%d" % (Config.n, Config.n0, low, hight)

"""
This page is used to initialize our operations.
"""
class Init(webapp2.RequestHandler):
  def get(self):
    password = self.request.get('password')
    try:
      njobs = int(self.request.get('njobs'))
    except:
      njobs = Config.default_batch_size

    if password != Config.password:
      self.response.out.write("Wrong password: %s" % password)
      return   
 
    if Config.max_id < Config.num_of_jobs:
      self.response.out.write("Fix your cofiguration, max_id should be higher or equal than number of jobs.")
      return

    self.response.out.write("Notice that you might want to delete previously stored entities before.\n<br>\n")
    self.response.out.write("n=%s n0=%s\n<br>\n" % (Config.n, Config.n0))

    counter = DataCounter.get()

    size = int(math.ceil(float(Config.max_id) / Config.num_of_jobs))
    
    for i in range(counter.created_jobs, min(counter.created_jobs + njobs, Config.num_of_jobs - 1)):
      self.create_stub_job(i*size, (i+1)*size) 
      counter.created_jobs += 1
      counter.created_ids += size
      
    if counter.created_jobs == Config.num_of_jobs - 1:
      self.create_stub_job((Config.num_of_jobs - 1)*size, Config.max_id)  
      counter.created_jobs += 1
      counter.created_ids += Config.max_id - (Config.num_of_jobs - 1)*size

    counter.put()

  def create_stub_job(self, low, hight):
    key_name = gen_key(low, hight)
    j = Job(key_name = key_name)
    j.n = Config.n
    j.n0 = Config.n0
    j.low_id = low
    j.hight_id = hight

    j.put()
    self.response.out.write("%s\t%s\n<br>\n" % (Job.get_by_key_name(key_name).low_id,Job.get_by_key_name(key_name).hight_id))


"""
This will allocate a single job to a requesting party. The order of allocation is
unallocated jobs first and then allocated jobs with no results, ordered by their
date (oldest will be reallocated first). 
"""
class Allocate(webapp2.RequestHandler):
  def get(self):
    
    job = db.GqlQuery("SELECT * FROM Job WHERE date_allocated = NULL LIMIT 1").get()
    if job:
      job.secret_hash = gen_random_word(Config.secret_hash_length)
      
      c = DataCounter.get()
      c.allocated_ids += job.hight_id - job.low_id
      c.put()
    else:
      job = db.GqlQuery("SELECT * FROM Job WHERE date_reported = NULL ORDER BY date_allocated ASC LIMIT 10",).get()
      if not job:
        # All jobs are done!
        self.response.out.write("%s 0 0 0 0 0 0" % (Config.client_version))
        return

    job.ip_allocated = self.request.remote_addr
    job.date_allocated = datetime.today()
    job.put()

    self.response.out.write("%s %s %s %d %d %d %d" % (Config.client_version, job.secret_hash, Config.pred_name, job.n, job.n0, job.low_id, job.hight_id))

"""
This page allows clients reporting results of run jobs.
We use a secret and an ip comparison as a protection against mistakes
or deliberate tampering with the db.
"""
class Report(webapp2.RequestHandler):
  def get(self):
    secret = self.request.get('secret')
    low = int(self.request.get('low'))
    hight = int(self.request.get('hight')) 
    res = self.request.get('res').split(u' ') # Also splitting by '+'
    cpu_time = float(self.request.get('cpu'))
    


    job = Job.get_by_key_name(gen_key(low, hight))
    
    if not job or job.secret_hash != secret or job.date_reported:
      #self.response.out.write("Problem. %s %s %s" % (job.date_reported,res, len(res)))
      return

    # if job.ip_allocated != self.request.remote_addr:
    # TODO

    job.date_reported = datetime.today()
    job.put()
    
    c = DataCounter.get()
    c.reported_ids += job.hight_id - job.low_id
    c.date_last = datetime.today()
    c.cpu_time += cpu_time
    c.put()   

    for r in res:
      x = r.split(u':')
      if len(x) != 2: return
      jobr = JobResult()
      jobr.job = job
      jobr.size = int(x[0])
      jobr.result = int(x[1])
      jobr.put()

      countr = ResultsCounter.get(int(x[0]))
      countr.result += int(x[1])
      countr.put()
  
""" 
This page displays info about progress and results.
"""
class Info(webapp2.RequestHandler):
  def get(self):
    password = self.request.get('password')
    if password != Config.password:
      self.response.out.write("Wrong password: %s" % password)
      return   

    c = DataCounter.get()
    total = c.created_ids
    allocated = c.allocated_ids
    reported = c.reported_ids
    date_started = c.date_started
    date_last = c.date_last
    
    sums = sorted([(r.size, r.result) for r in ResultsCounter.all()])
   
    config_dict = {}
    for k,v in vars(Config).items():
      if k[0] != "_" and k != "password":
        config_dict[k] = v

    template_values = {
      'not_allocated': total-allocated,
      'not_reported': allocated-reported,
      'reported': reported,
      'config_dict': config_dict,
      'total': total,
      'allocated': allocated,
      'date_started': date_started,
      'date_last': date_last,
      'cpu_time' : c.cpu_time,
      'sums': sums,
    }

    path = os.path.join(os.path.dirname(__file__), 'info.html')
    self.response.out.write(template.render(path, template_values))




app = webapp2.WSGIApplication([
  ('/allocate', Allocate),
  ('/report', Report),

  ('/init', Init),
  ('/info', Info)

], debug=True)
