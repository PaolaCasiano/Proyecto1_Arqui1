from django.shortcuts import render
from django.http import HttpResponse, HttpResponseBadRequest,HttpResponseRedirect
from django.shortcuts import render, HttpResponse
from django.views.decorators.csrf import csrf_exempt
from django.http import JsonResponse
from .file.Lista import ListaImagenes, NodoI
from .file.image_to_gcode import ImageToGcode
import base64
import json

# Clase estatica donde estara la imagen, o incluso una lista, pero tendria que saber cual ya ha mostrado

imagesList = ListaImagenes()

def index(request):
	return HttpResponse('<h1>Hemlo world</h1>')

# Create your views here.

def guardar_info(request):
	if request.method == 'POST':
		name = request.POST.get('nombre')
		objB64 = request.POST.get('base64')
		nueva = Nodo(name, objB64)
		
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

			imgdata = base64.b64decode(siguiente.base64)
			filename = siguiente.nombre + '.jpg'  # I assume you have a way of picking unique filenames
			with open(filename, 'wb') as f:
			    f.write(imgdata)

def prueba(request):
	if request.method == 'POST':
		name = request.POST.get('nom')
		objB64 = request.POST.get('nom2')
		print(name+'  '+ objB64)
		return JsonResponse({'success':'200'})
	else:
		return JsonResponse({'success':'500', "message":"metodo metodo incorrecto"})

