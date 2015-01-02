from django.contrib import admin
from counting_app.models import Config,Job,ResultCounter

@admin.register(Job,ResultCounter)
class ReadOnlyModelAdmin(admin.ModelAdmin):
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
    def get_readonly_fields(self, request, obj=None):
        if obj: # editing an existing object
            return self.readonly_fields + ('n', 'n0')
        return self.readonly_fields

