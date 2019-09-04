from django.shortcuts import render
from django.http import HttpResponse, HttpResponseBadRequest,HttpResponseRedirect
from django.shortcuts import render, HttpResponse
from django.views.decorators.csrf import csrf_exempt
from django.http import JsonResponse
import json

def index(request):
	return HttpResponse('<h1>Hemlo world</h1>')

# Create your views here.

def guardar_info(request):
	if request.method == 'POST':
		name = request.POST.get('nombre')

		return JsonResponse({'foo':'bar'})
	else:
		return JsonResponse({'faa':'bar'})

def request_impresion(request):
	if request.method == "GET":
		return JsonResponse({'x':1, 'y':2})

