from django.urls import path	
from . import views

urlpatterns = [
    path('', views.index, name='index'),
    path('guardar_info', views.guardar_info, name='guardar info'),
]
