from django.conf.urls import patterns, url

from counting_app import views

urlpatterns = patterns('',
    url(r'^info$', views.info, name='info'),
    url(r'^allocate$', views.allocate, name='allocate'),
    url(r'^report$', views.report, name='report'),
)
