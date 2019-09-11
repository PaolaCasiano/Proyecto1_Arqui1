from django.shortcuts import render
from django.http import HttpResponse, HttpResponseBadRequest,HttpResponseRedirect
from django.shortcuts import render, HttpResponse
from django.views.decorators.csrf import csrf_exempt
from django.http import JsonResponse
from .file.Lista import ListaImagenes, NodoI
from .file.image_to_gcode import ImageToGcode
from django.views.decorators.csrf import csrf_exempt
import base64
import json


# Clase estatica donde estara la imagen, o incluso una lista, pero tendria que saber cual ya ha mostrado

imagesList = ListaImagenes()
printer = ImageToGcode()

def index(request):
	return HttpResponse('<h1>Hemlo world</h1>')

def pruebaHola(request):
	return HttpResponse("Hola")
# Create your views here.

def guardar_info(request):


	print("THIS IS THE FKNG POST")
	body_unicode = request.body.decode('utf-8')
	print(body_unicode)
	print("THIS IS THE END OF FKNG POST")
	return JsonResponse({'success':'200'})
	if request.method == 'POST':
		name = request.POST.get('nombre')
		objB64 = request.POST.get('base64')
		nueva = Nodo(name, objB64)
		
		imageList.insertarYa(nueva)

		return JsonResponse({'success':'200'})
	else:
		return JsonResponse({'success':'500', "message":"metodo metodo incorrecto"})

@csrf_exempt
def request_impresion(request):
	if request.method == "GET":
		if(imageList.estaVacio):
			return JsonResponse({'size':'0'})
		else:
			siguiente = imageList.first
			tamano = imageList.tamano
			imageList.moveToNext()

			imgdata = base64.b64decode(siguiente.base64)
			filename = siguiente.nombre 
			f = filename
			with open(filename, 'wb') as f:
			    f.write(imgdata)
			if f!= None:
				printer.printImage(filename, filename + '.jpg') 




def prueba(request):
	if request.method == 'POST':
		name = request.POST.get('nom')
		objB64 = request.POST.get('nom2')
		print(name+'  '+ objB64)
		return JsonResponse({'success':'200'})	
	else:
		return JsonResponse({'success':'500', "message":"metodo metodo incorrecto"})


@csrf_exempt
def getStringLiquid(request):
	print("THIS IS THE FKNG POST")
	body_unicode = request.body.decode('utf-8')
	print(body_unicode)
	print("THIS IS THE END OF FKNG POST")
	return HttpResponse("Recibi el arreglo")