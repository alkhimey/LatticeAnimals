import math

from django.db import models
from django.db.models.signals import post_save
from django.dispatch import receiver



class Config(models.Model):
  date_created           = models.DateTimeField(auto_now_add=True)
  date_activated         = models.DateTimeField(null=True)
  client_version         = models.CharField(max_length=8, help_text = "Which client version this config can communicate with")
  algo_id                = models.IntegerField(help_text = "Which counting algorithm to tell the client to use")
  n                      = models.IntegerField(help_text = "The size of the polyocubes we want to count")
  n0                     = models.IntegerField(help_text = "At which size we want to split the counting")
  max_id                 = models.IntegerField(help_text = "Total Number of policubes of size n0 without applying predicate. Determined from algo id and n0")
  num_of_jobs            = models.IntegerField(help_text = "To how many jobs we want to split the counting")
  secret_hash_length     = models.IntegerField(default=32, help_text = "Max is 64")
  minutes_before_realloc = models.IntegerField(default=10, help_text = "Minutes to wait before an unreported job can be reallocated")
    
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


class ResultCounter(models.Model):
  config      =  models.ForeignKey(Config)
  size        =  models.IntegerField()
  accamulator =  models.IntegerField(default=0)

  def __str__(self):
    return "%d : %d" % (self.size, self.accamulator)





@receiver(post_save, sender=Config)
def create_jobs_and_result_counters(sender, instance, created, **kwargs):
  """Create a jobs and result counters whenever a new config is created"""
  if created: 
    size = int(math.ceil(float(instance.max_id) / instance.num_of_jobs))

    for i in xrange(1, instance.num_of_jobs + 1 ):
      j = Job(config  = instance, 
              low_id  = size*(i-1), 
              high_id = min(size*i, instance.max_id))
      j.save()

    for i in range(instance.n0, instance.n + 1):
      r = ResultCounter(size = i, config = instance)
      r.save()

