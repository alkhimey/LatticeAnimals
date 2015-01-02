from django.shortcuts import render
from django.http import HttpResponse
from counting_app.models import Job, ResultCounter
from django.db.models import Q

import math
import random
from datetime import datetime, timedelta

def gen_random_word(length):
  word = ''
  for i in range(length):
    word += random.choice('ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789')
  return word

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

def allocate(request):
  
  j = Job.objects.filter(date_reported = None).order_by('date_allocated').first()  # TODO: Prevent reallocating older than X hours, can use in code as the result of query in oldest anyway
  if not j:
    return HttpResponse("0 0 0 0 0 0 0")

  j.secret_hash     = gen_random_word(j.config.secret_hash_length)
  j.ip_allocated    = request.META['REMOTE_ADDR']
  j.date_allocated  = datetime.today()
  j.save()

  return HttpResponse("%s %s %s %d %d %d %d" % (j.config.client_version, j.secret_hash, j.config.algo_id, j.config.n, j.config.n0, j.low_id, j.high_id))
  
def report(request):
  j = Job.objects.filter(secret_hash  = request.GET.get('secret', None), 
                         ip_allocated = request.META['REMOTE_ADDR']).first()

  if not j or j.date_reported != None:
    return HttpResponse('')

  j.date_reported = datetime.today()
  j.cpu_time      = float(request.GET.get('cpu', 0.0))
  j.results       = request.GET.get('res', '')
  j.save()
    
  for r in [x for x in request.GET.get('res', u'').split(u' ') if x != u'']:
    x = r.split(u':')
    
    rc = ResultCounter.objects.get(config = j.config, size = int(x[0]))
    rc.accamulator = rc.accamulator + int(x[1])
    rc.save()


  return HttpResponse('')

def info(request):
  #context = {'latest_question_list': latest_question_list}
  #return render(request, 'polls/index.html', context)

  return HttpResponse("Use admin interface")

