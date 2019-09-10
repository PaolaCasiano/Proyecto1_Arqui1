class ListaImagenes(object):


	def __init__(self):
		print ('Creo una lista de imagenes')
		self.primero= None
		self.con = 0
		self.ultimo= None

	def estaVacio(self):
		return self.primero== None

	def insertarYa(self, nuevo):
		if self.primero == None:
			self.primero = nuevo
			self.ultimo = self.primero
			self.cont +=1
		else:
			self.ultimo.siguiente = nuevo
			self.ultimo = self.ultimo.siguiente
			self.con += 1

	def tamano(self):
		return sef.con

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

	def buscar(self, palabra):
		print ('entro a buscar')
		cont = 0
		if self.primero ==None:
			self.esta = False
			print ("Lista vacia")
			return self.primero
		else:
			aux = self.primero
			while aux!=None:
				if aux.user == palabra:
					print ('Se encontro')
					self.esta = True
					print ("Se encontro "+str(aux.user)+ " en el indice ",str(cont))
					return aux
					break
				else:
					aux = aux.siguiente
					cont = cont + 1
					self.esta = False
					print ('no esta')
			return None


class NodoI(object):

	def __init__(self, name,base64):
		self.name = name
		self.base64 = base64
		self.siguiente = None
		self.anterior = None

	def getUser(self):
		return str(self.user)

	def getPassword(self):
		return str(self.password)

	def setRoot(self,root):
		self.root = root