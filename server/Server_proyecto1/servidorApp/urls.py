from django.urls import path	
from . import views

urlpatterns = [
    path('', views.index, name='index'),
    path('pruebaHola', views.pruebaHola, name='pruebaHola'),
    path('guardar_info', views.guardar_info, name='guardar info'),
    path('getStringLiquid', views.getStringLiquid, name='getStringLiquid'),
    path('request_impresion', views.request_impresion, name='request_impresion'),
    path('prueba', views.prueba, name='prueba'),
    path('get_pila', views.get_pila, name='get_pila'),
    path('buscar_imagen', views.buscar_imagen, name='buscar_imagen'),
]
