from django.urls import path	
from . import views

urlpatterns = [
    path('', views.index, name='index'),
    path('pruebaHola', views.pruebaHola, name='pruebaHola'),
    path('guardar_info', views.guardar_info, name='guardar info'),
    path('getStringLiquid', views.getStringLiquid, name='getStringLiquid'),
]
