from django.contrib import admin
from django.forms import BaseInlineFormSet
from counting_app.models import Config,Job, KeyValueResult

@admin.register(Job, KeyValueResult)
class ReadOnlyModelAdmin(admin.ModelAdmin):
    """ Job and KeyValueResult models are read only in the admin view """
    def get_readonly_fields(self, request, obj=None):
        if self.declared_fieldsets:
            return flatten_fieldsets(self.declared_fieldsets)
        else:
            return list(set(
                [field.name for field in self.opts.local_fields] +
                [field.name for field in self.opts.local_many_to_many]
            ))

class LimitObjectsFormSet(BaseInlineFormSet): 

    def get_queryset(self) :
        qs = super(LimitObjectsFormSet, self).get_queryset()
        return qs[:5]

class ReadOnlyTabularInlineJob(admin.TabularInline):
    model = Job
    extra = 0
    formset = LimitObjectsFormSet
    can_delete = False

    fields =  [ "low_id", "high_id", "date_allocated", "date_reported", "cpu_time", "ip_allocated", "results", "secret_hash" ]
    readonly_fields = fields


class LastReportsInline(ReadOnlyTabularInlineJob):
    ordering = ("-date_reported",)
    verbose_name_plural = "Latest Reported Jobs"


class LastAllocationsInline(ReadOnlyTabularInlineJob):
    ordering = ("-date_allocated",)
    verbose_name_plural = "Latest Allocated Jobs"



"""
class ResultsInlnie(admin.TabularInline):
    model = KeyValueResult
    extra = 0
    formset = LimitObjectsFormSet
    can_delete = False

    fields =  [ "key", "value"]
    readonly_fields = fields

    def get_queryset(self) :
        qs = super(LimitObjectsFormSet, self).get_queryset()
        return qs
"""


@admin.register(Config)
class ConfigAdmin(admin.ModelAdmin):
    list_display = ( 'n', 'n0', 'algo_id', 'result_for_n',  'client_version', 'date_activated', 'num_of_jobs_left', 'num_of_jobs_allocated', 'num_of_jobs_complete', 'comment' )
    list_display_links = list_display
    readonly_fields = [ 'date_created', 'num_of_jobs_left', 'num_of_jobs_allocated', 'num_of_jobs_complete', 'results_totals', 'total_jobs' ]

    inlines = [ LastAllocationsInline, LastReportsInline, ]

    fieldsets = [
        ('Confiiguration',   
         {'fields' : ['date_created', 'date_activated', 'client_version', 'algo_id', 'n', 'n0', 'max_id', 'num_of_jobs', 'secret_hash_length', 'minutes_before_realloc', 'comment'], 
          'classes': []}),
        
        ('Progress',         
         {'fields' : ['num_of_jobs_left', 'num_of_jobs_allocated', 'num_of_jobs_complete', 'total_jobs', 'results_totals'], ## TODO: total num of jobs readonly
          'classes': []}), # collapse class? or use grappeli
    ]


    def total_jobs(self, obj):
        """ Used to display as read only in "Progress" fieldset """
        return obj.num_of_jobs









