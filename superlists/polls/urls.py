from django.urls import path

from . import views

urlpatterns = [
    path('good', views.index, name='index'),
]
