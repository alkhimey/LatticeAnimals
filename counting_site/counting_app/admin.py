from django.contrib import admin
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


@admin.register(Config)
class ConfigAdmin(admin.ModelAdmin):
    list_display = ( 'n', 'n0', 'algo_id', 'client_version', 'date_activated', 'num_of_jobs_left', 'num_of_jobs_allocated', 'num_of_jobs_complete' )

    readonly_fields = [ 'date_created', 'num_of_jobs_left', 'num_of_jobs_allocated', 'num_of_jobs_complete' ]

    fieldsets = [
        ('Confiiguration',   
         {'fields' : ['date_created', 'date_activated', 'client_version', 'algo_id', 'n', 'n0', 'max_id', 'num_of_jobs', 'secret_hash_length', 'minutes_before_realloc'], 
          'classes': []}),
        
        ('Progress',         
         {'fields' : ['num_of_jobs_left', 'num_of_jobs_allocated', 'num_of_jobs_complete', 'num_of_jobs'], 
          'classes': []}), # collapse class? or use grappeli
    ]



#    def get_readonly_fields(self, request, obj=None):
#        if obj: # editing an existing object
#            return self.readonly_fields + ('n', 'n0')
#        return self.readonly_fields

