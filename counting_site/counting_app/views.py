from django.shortcuts import render
from django.http import HttpResponse
from counting_app.models import Config,Job,KeyValueResult
from django.db.models import Q

import math
import random
from datetime import datetime, timedelta
import time

def gen_random_word(length):
  word = ''
  for i in range(length):
    word += random.choice('ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789')
  return word


def allocate(request):

  # Always work on the latest config
  c = Config.objects.order_by('-date_activated').first()

  if c == None:
    return HttpResponse("0 0 0 0 0 0 0")
    
  # Get job who is allocation candidate
  # TODO: Prevent reallocating older than X hours, can use in code as the result of query in oldest anyway
  j = Job.objects.filter(date_reported = None, config = c).order_by('date_allocated').first()  

  if not j:
    return HttpResponse("0 0 0 0 0 0 0")



  # Jobs are allowed to be reallocated only after a configured amount of time without response
  if j.date_allocated != None:
    
    d1_ts = time.mktime(j.date_allocated.timetuple())
    d2_ts = time.mktime(datetime.today().timetuple())

    if int(d2_ts-d1_ts) / 60 < j.config.minutes_before_realloc:
      return HttpResponse("0 0 0 0 0 0 0")

  # Actual allocation
  j.secret_hash     = gen_random_word(j.config.secret_hash_length)
  j.ip_allocated    = request.META['REMOTE_ADDR']
  j.date_allocated  = datetime.today()
  j.save()

  # Write response
  return HttpResponse("%s %s %s %d %d %d %d" % (j.config.client_version, j.secret_hash, j.config.algo_id, j.config.n, j.config.n0, j.low_id, j.high_id))
  
def report(request):
  
  j = Job.objects.filter(secret_hash  = request.GET.get('secret', None), 
                         ip_allocated = request.META['REMOTE_ADDR']).first()

  # If wrong secret hash or job allocated to someone else or reported already 
  if not j or j.date_reported != None:
    return HttpResponse('')


  j.date_reported = datetime.today()
  j.cpu_time      = float(request.GET.get('cpu', 0.0))
  j.results       = request.GET.get('res', '')
  j.save()
    
  # Parse the result and create result key value pair objects
  for r in [x for x in request.GET.get('res', u'').split(u' ') if x != u'']:
    print r

    x = r.split(u':')
    
    print x[0], x[1]

    kvr = KeyValueResult(job   = j,
                         key   = x[0],
                         value = x[1])
    kvr.save()

  return HttpResponse('')

def info(request):
  #context = {'latest_question_list': latest_question_list}
  #return render(request, 'polls/index.html', context)

  return HttpResponse("Obsolete: Use admin interface")

