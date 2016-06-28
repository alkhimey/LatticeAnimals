import math

from django.db import models
from django.db.models import Sum,Count, Avg
from django.db.models.signals import post_save
from django.dispatch import receiver
import datetime


class Config(models.Model):
  date_created           = models.DateTimeField(auto_now_add=True) # TODO: Use custom save method
  date_activated         = models.DateTimeField(null=True) # TODO: Consider using only creation date?
  client_version         = models.CharField(max_length=8, help_text = "Which client version this config can communicate with")
  algo_id                = models.IntegerField(help_text = "Which counting algorithm to tell the client to use")
  n                      = models.IntegerField(help_text = "The size of the polyocubes we want to count")
  n0                     = models.IntegerField(help_text = "At which size we want to split the counting")
  max_id                 = models.IntegerField(help_text = "Total Number of policubes of size n0 without applying filter. Determined from algo id and n0")
  num_of_jobs            = models.IntegerField(help_text = "To how many jobs we want to split the counting")
  secret_hash_length     = models.IntegerField(default=32, help_text = "Max is 64")
  minutes_before_realloc = models.IntegerField(default=10, help_text = "Minutes to wait before an unreported job can be reallocated")
  comment                = models.CharField(max_length=1000, help_text = "User comment")
    
  
  def num_of_jobs_allocated(self):
    """ Allocated and not complete yet """
    
    return self.job_set.filter(date_reported = None).exclude(date_allocated = None).count()

  def num_of_jobs_complete(self):
    return self.job_set.exclude(date_reported = None).count()

  def num_of_jobs_left(self):
    return self.job_set.filter(date_reported = None).count()

  def results_totals(self):
    return KeyValueResult.objects.filter(job__config = self).values('key').annotate(Sum('value'))  
    
  def result_for_n(self):
    """ Result for the highest counted value """
    p =  self.results_totals().filter(key = self.n)
    if len(p):
      return p[0]['value__sum']
    else:
      return 0

  def total_cpu_time(self):
    """ Conpound CPU time spent by all participants on this counting effort """

    return datetime.timedelta(
      seconds = self.job_set.aggregate(Sum('cpu_time'))['cpu_time__sum'])

  def avarage_job_cpu_time(self):
    """ Return the avarage job cpu time for a config """
    if not self.job_set.exclude(date_reported__isnull = True).exists():
      return 0.0
    else:
      return self.job_set.exclude(date_reported__isnull = True).aggregate(Avg('cpu_time'))['cpu_time__avg']

  def last_report_date(self):
    return self.job_set.order_by('-date_reported')[0].date_reported
  
  def total_real_time(self):
    """ Total real world time that passed since activation """

    if not self.date_activated:
      return datetime.timedelta(0)
    else:
      return  datetime.datetime.now() - self.date_activated
    

  def participants_list(self):
    """ Return aggregate info about ip addresses that are participating in the effort """

    return self.job_set.exclude(date_reported__isnull = True).values('ip_allocated').annotate(cpu_time_sum = Sum('cpu_time'), avarage_job_cpu_time = Avg('cpu_time'),  num_of_jobs_complete = Count('pk')).order_by('-cpu_time_sum')
  

  def predicted_time_left(self):
    return datetime.timedelta(
      seconds = self.avarage_job_cpu_time() * self.num_of_jobs_left())


  def   job_complete_histo(self, num_of_bins = 12):
    """ Produces an array of number of complete jobs per round hour 
        For example if num_of_bins is 3 and current time is 15:33:22, the 
        result will be  dictonary with key 14:00:00 pointing to number of complete jobs 
        between 14:00:00 and 15:00:00 and key 13:00:00 pointing to number of 
        complete jobs between 13:00:00 and 14:00:00
    """    

    results = {}
    for dt in range(num_of_bins):
      end_date = datetime.datetime.today().replace(minute = 0, second = 0, microsecond = 0) - datetime.timedelta(hours = dt)
      start_date = end_date - datetime.timedelta(hours = 1)
      results[start_date] =   self.job_set.filter(date_reported__range = (start_date, end_date)).count()
    return results
    
  def __str__(self):
    return "%d %d v%s-%d" % (self.n, self.n0, self.client_version, self.algo_id)





class Job(models.Model):
  config         = models.ForeignKey(Config)
  secret_hash    = models.CharField(max_length=64)
  low_id         = models.IntegerField()
  high_id        = models.IntegerField()
  date_allocated = models.DateTimeField(null=True)
  date_reported  = models.DateTimeField(null=True)
  results        = models.CharField(max_length=1024, null=True) # Redundant as we have ResultCounter. Used as backup in case there is some concurency problem.
  cpu_time       = models.FloatField(default=0)

  ip_allocated   = models.CharField(max_length=15, null=True)


  def __str__(self):
    return "%d %d [%d..%d]" % (self.config.n, self.config.n0, self.low_id, self.high_id)


class KeyValueResult(models.Model):
  job         = models.ForeignKey(Job)
  key         = models.CharField(max_length=32)
  value       = models.CharField(max_length=32)

  def __str__(self):
    return "%s : %s" % (self.key, self.value)





@receiver(post_save, sender=Config)
def create_jobs(sender, instance, created, **kwargs):
  """Create a jobs  whenever a new config is created"""

  if created: 
    size = int(math.ceil(float(instance.max_id) / instance.num_of_jobs))

    for i in range(1, instance.num_of_jobs + 1 ):
      j = Job(config  = instance, 
              low_id  = size*(i-1), 
              high_id = min(size*i, instance.max_id))
      j.save()
