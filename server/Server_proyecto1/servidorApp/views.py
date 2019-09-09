from django.shortcuts import render
from django.http import HttpResponse, HttpResponseBadRequest,HttpResponseRedirect
from django.shortcuts import render, HttpResponse
from django.views.decorators.csrf import csrf_exempt
from django.http import JsonResponse
from file.Lista import ListaImagenes, NodoI
import json

# Clase estatica donde estara la imagen, o incluso una lista, pero tendria que saber cual ya ha mostrado

imagesList = ListaImagenes()

def index(request):
	return HttpResponse('<h1>Hemlo world</h1>')

# Create your views here.

def guardar_info(request):
	if request.method == 'POST':
		name = request.POST.get('nombre')
		objJsn = request.POST.get('json')
		nueva = Nodo(name, objJsn)
		imageList.insertarYa(nueva)

		return JsonResponse({'success':'200'})
	else:
		return JsonResponse({'success':'500', "message":"metodo metodo incorrecto"})

def request_impresion(request):
	if request.method == "GET":
		if(imageList.estaVacio):
			return JsonResponse({'size':'0'})
		else:
			siguiente = imageList.first
			tamano = imageList.tamano
			imageList.moveToNext()
			return JsonResponse(
					{"size": tamano, "name": siguiente.name, "json": siguiente.json}
				)


