from django.conf.urls import  include, url
from django.contrib import admin

urlpatterns = [
    # Examples:
    # url(r'^$', 'counting_site.views.home', name='home'),
    # url(r'^blog/', include('blog.urls')),

    url(r'^admin/', include(admin.site.urls)),
    url(r'^c/', include('counting_app.urls')),
    url(r'', include('counting_app.urls')),
    url(r'^counting/', include('counting_app.urls')),
]
