from django.conf.urls import url

from counting_app import views

urlpatterns = [
    url(r'^info$', views.info, name='info'),
    url(r'^allocate$', views.allocate, name='allocate'),
    url(r'^report$', views.report, name='report'),
]
