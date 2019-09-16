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
		print("Entro a guardar_info")
		imgdata = base64.b64decode(objB64)
		with open(name, 'wb') as f:
		    f.write(imgdata)
		    #printer.printImage(name,name+'.jpg') 
		    gcode = printer.printImageStr("", name)+"$" #OBTENER GCODE DE IMAGEN
		    nueva = Nodo(name, objB64, gcode)
		    imagesList.insertarYa(nueva)

		return JsonResponse({'success':'200'})
	else:
		return JsonResponse({'success':'500', "message":"metodo metodo incorrecto"})




@csrf_exempt
def get_pila(request):
	if request.method == "GET":
		if(imagesList.estaVacio()):
			return HttpResponse("E")
		print("lista no vacia")

		if(imagesList.punteroTopado()):
			print ("tamano:" + str(imagesList.tamano()))
			print ("puntero:" + str(imagesList.getPUntero()))
			return HttpResponse("F")
		print("puntero no topado")

		actual = imagesList.getPosicion()#OBTENER LA ULTIMA QUE SE TRABAJO
		if actual == None:
			return HttpResponse("H")#SI ES NULO, PUES QUE YA TOPO LA COLA
		print("actual no nulo")
		if actual.pos < len(actual.gcode): #VERIFICAR SI YA TOPO EN GCODE
			print(actual.gcode)
			return HttpResponse("I")
		return HttpResponse("H") 

@csrf_exempt
def request_impresion(request):
	if request.method == "GET":

		if(imagesList.estaVacio()):
			return HttpResponse("E")

		elif(imagesList.punteroTopado()):
			print ("tamano:" + str(imagesList.tamano()))
			print ("puntero:" + str(imagesList.getPUntero()))
			return HttpResponse("F")

		else:
			actual = imagesList.getPosicion()#OBTENER LA ULTIMA QUE SE TRABAJO
			if actual == None:
					return HttpResponse("H")#SI ES NULO, PUES QUE YA TOPO LA COLA
			if actual.pos < len(actual.gcode): #VERIFICAR SI YA TOPO EN GCODE
				gcode = actual.gcode[actual.pos:actual.fin]#OBTENER EL CARACTER SIGUIENTE
				print(gcode)
				actual.aumentarPos()#AUMENTAR LA POSICION DEL PUNTERO DEL GCODE
				return HttpResponse(gcode)
			else:#SI YA TOPO, CAMBIAR EL PUNTERO
				imagesList.aumentarPuntero()
				return HttpResponse("$") 



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
	printer.coordenadasapng(name,body_unicode) #TRANSFORMAR EN IMAGEN
	gcode = printer.printImageStr("", name)+"$" #OBTENER GCODE DE IMAGEN
	nueva = Nodo(name, "", gcode)#CREAR NUEVO NODO
	imagesList.insertarYa(nueva)#INSERTAR EN LISTA
	return HttpResponse("inserto")


@csrf_exempt
def buscar_imagen(request):
	if request.method == 'POST':
		name = request.POST.get('nombre')+'.jpg'
		nodo = imagesList.buscar_imagen(name);
		if nodo == None:
			return HttpResponse("404");
		else:
			imagesList.insertarYa(nodo)	
		return HttpResponse("200")
	else:
		return HttpResponse("500")
