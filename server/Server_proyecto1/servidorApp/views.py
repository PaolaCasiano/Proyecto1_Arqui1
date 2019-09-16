from django.shortcuts import render
from django.http import HttpResponse, HttpResponseBadRequest,HttpResponseRedirect
from django.shortcuts import render, HttpResponse
from django.views.decorators.csrf import csrf_exempt
from django.http import JsonResponse
from .file.Lista import ListaImagenes, Nodo
from .file.image_to_gcode import ImageToGcode
from django.views.decorators.csrf import csrf_exempt
import base64
import json


# Clase estatica donde estara la imagen, o incluso una lista, pero tendria que saber cual ya ha mostrado

imagesList = ListaImagenes()
printer = ImageToGcode()
conteo = 0;

def index(request):
	return HttpResponse('<h1>Hemlo world</h1>')

def pruebaHola(request):
	return HttpResponse("Hola")
# Create your views here.

@csrf_exempt
def guardar_info(request):
	if request.method == 'POST':
		name = request.POST.get('nombre')
		objB64 = request.POST.get('B64')
		nueva = Nodo(name+'.jpg', objB64)
		print("Entro a guardar_info")
		imgdata = base64.b64decode(objB64)
		with open(name+'.jpg', 'wb') as f:
		    f.write(imgdata)
		    printer.printImage(name,name+'.jpg') 
		imagesList.insertarYa(nueva)

		return JsonResponse({'success':'200'})
	else:
		return JsonResponse({'success':'500', "message":"metodo metodo incorrecto"})


@csrf_exempt
def request_impresion(request):
	if request.method == "GET":
		#imppresion = imagesList.imprimir()
		#return HttpResponse(imppresion)


		if(imagesList.estaVacio()):
			return HttpResponse("E")

		elif(imagesList.punteroTopado()):
			print ("tamano:" + str(imagesList.tamano()))
			print ("puntero:" + str(imagesList.getPUntero()))
			return HttpResponse("F")

		else:
			siguiente = imagesList.getPosicion()
			if siguiente == None:
				return HttpResponse("H")

			filename = siguiente.name 
			gcode = printer.printImageStr("", filename) 
			print("acabo de mandar algo")
			#print(gcode)
			imagesList.aumentarPuntero()
			return HttpResponse(gcode)



@csrf_exempt
def prueba(request):
	impresion = imagesList.imprimir()
	return HttpResponse(impresion)


@csrf_exempt
def getStringLiquid(request):
	print("THIS IS THE POST")
	body_unicode = request.body.decode('utf-8')
	print(body_unicode)
	print("THIS IS THE END OF POST")
	name = "pantalla_" + str(imagesList.tamano())+'.png'
	printer.coordenadasapng(name,body_unicode)
	nueva = Nodo(name, "")
	imagesList.insertarYa(nueva)
	return HttpResponse("inserto")