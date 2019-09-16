class ListaImagenes(object):


	def __init__(self):
		print ('Creo una lista de imagenes')
		self.primero= None
		self.con = 0
		self.ultimo= None
		self.puntero = 0

	def estaVacio(self):
		return self.primero== None

	def insertarYa(self, nuevo):
		if self.primero == None:
			self.primero = nuevo
			self.ultimo = self.primero
			self.con +=1
		else:
			self.ultimo.siguiente = nuevo
			self.ultimo = self.ultimo.siguiente
			self.con += 1

	def tamano(self):
		return self.con

	def getPUntero(self):
		return self.puntero

	def first(self):
		return self.primero

	def moveToNext(self):
		if self.primero.siguiente == None:
			self.primero = None
			self.con = 0
			self.ultimo = None
		else:
			self.primero = self.primero.siguiente
			self.con =- 1

	def getPosicion(self):
		#a = str(self.primero.name) + ";"
		if self.primero == None:
			return None

		if self.puntero == 0:
			return self.primero

		aux = self.primero
		cont_aux = 0
		while aux.siguiente!= None:
			aux = aux.siguiente
			cont_aux = cont_aux +1
			if cont_aux == self.puntero:
				return aux
		return None

	def aumentarPuntero(self):
		if(self.puntero < self.con):
			self.puntero = self.puntero +1

	def punteroTopado(self):
		if(self.con == 0):
			return True
		if self.puntero >= self.con:
			return True

		return False

	def buscar_imagen(self,nombre):
		if self.primero ==None:
			return self.primero
		else:
			aux = self.primero
			while aux!=None:
				if aux.name == nombre:
					return aux
				else:
					aux = aux.siguiente
					print ('no esta')
			return None

	def imprimir(self):
 		print ('\nse entro a imprimir')

 		if self.primero==None:
 			print ('la lista esta vacia')
 			return "Lista Vacia"

 			#return "lista Vacia"
 		else:
 			print ('no esta vacio')
 			#a = str(self.primero.name) + ";"
 			aux = self.primero
 			a = aux.name;

 			while aux.siguiente!= None:
 				#a=a+ str(aux.name)+";"
 				a=a+ " -> "+ str(aux.siguiente.name)+ ";"
 				print (aux.name)
 				aux = aux.siguiente
 			print (aux.name)
 			return a;


class Nodo(object):

	def __init__(self, name,base64, gcode):
		self.name = name
		self.base64 = base64
		self.siguiente = None
		self.anterior = None
		self.gcode = gcode
		self.pos = 0
		self.paso = 200
		self.fin = 200

	def getCharAtpos(self):
		if len(self.gcode) < self.pos:
			print(self.name)
			print("mygcode"	)
			print(self.gcode)
			return self.gcode[self.pos]

	def aumentarPos(self):
		if (self.fin + self.paso < len(self.gcode)):
			self.pos = self.fin
			self.fin = self.fin + self.paso
		else:
			self.pos = self.fin
			self.fin = len(self.gcode) - 1

