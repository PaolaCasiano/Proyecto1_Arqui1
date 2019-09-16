from __future__ import absolute_import, division, print_function, unicode_literals
import os
import sys
import cv2 as cv
import argparse
import termcolor
import ast
import copy

import numpy
#import matplotlib
#import matplotlib.pyplot
import imageio
import PIL.Image
import PIL.ImageDraw


class ImageToGcode():
    def __init__(self):
        print('Generated!')

    def imprimir1(self,
                 img,
                 spread, #extension decimal
                 nozzles, #entero
                 area, #decimal
                 feedrate, #avance decimal
                 offsets, #arreglo?
                 verbose=False):
        #self.img = cv.LoadImageM(img)
        self.img = cv.imread(img)
        self.cols, self.rows, self.channel = self.img.shape
        self.output = ""
        self.outFile = os.path.splitext(os.path.abspath(img))[0] + ".gco"
        self.spread = spread
        self.nozzles = int(nozzles)
        self.increment = spread/nozzles
        self.printArea = area
        self.feedrate = feedrate
        self.red = (0.0, 0.0, 255.0)
        self.green = (0.0, 255.0, 0.0)
        self.blue = (255.0, 0.0, 0.0)
        self.black = (0.0, 0.0, 0.0)
        self.offsets = offsets 
        #self.debug_to_terminal()
        self.make_gcode()


    def make_gcode(self):
        #self.output = "M106"  # Start Fan
        self.output += 'N10 (MyText)\n'
        self.output += 'N20 (M)\n'
        self.output += 'N30 G0 Z2\n'
        self.output += 'N40 G0 G21 X0 Y0\n'
        line = 5
        nozzleFirings = [0 for x in range(0, self.cols)]
        nozzleFirings = [copy.copy(nozzleFirings) for x in range(0, 4)]
        #scan = range(0, self.rows)
        scan = list(range(0, self.rows))
        scan.reverse()
        for y in scan:
            for x in range(0, self.cols):
                #color = cv.Get2D(self.img, y, x)
                color = self.img[y, x]
                if (color == self.red).all():
                    nozzleFirings[0][x] += 1 << y % self.nozzles
                elif (color == self.green).all():
                    nozzleFirings[1][x] += 1 << y % self.nozzles
                elif (color == self.blue).all():
                    nozzleFirings[2][x] += 1 << y % self.nozzles
                elif (color == self.black).all():
                    nozzleFirings[3][x] += 1 << y % self.nozzles
                else:
                    pass
                    
            if y % 12 == 0 and y > 0:
                for headNumber, headVals in enumerate(nozzleFirings):
                    for column, firingVal in enumerate(headVals):
                        if firingVal:
                            currentOffset = self.offsets[headNumber]
                            self.output += "N"+str(line)+"0 G1 X"+str(self.increment*column-currentOffset[0])+" Y"+str(y/12*self.spread-currentOffset[1])+" F"+str(self.feedrate)+"\n"
                            line= line+1
                            self.output += "N"+str(line)+"0 "+"M400\n"
                            line= line+1
                            self.output += "N"+str(line)+"0 "+"M700 P"+str(headNumber)+" S"+str(firingVal)+"\n"
                            line= line+1
                            #self.output += "G1X"+str(self.increment*column-currentOffset[0])+"Y"+str(y/12*self.spread-currentOffset[1])+"F"+str(self.feedrate)+"\n"
                            #self.output += "M400\n"
                            #self.output += "M700 P"+str(headNumber)+" S"+str(firingVal)+"\n"
                #print(str(nozzleFirings))
                nozzleFirings = [0 for x in range(0, self.cols)]
                nozzleFirings = [copy.copy(nozzleFirings) for x in range(0, 4)]
        f = open(self.outFile, 'w')
        f.write(self.output)
        f.close()
        #print(self.output)

    def debug_to_terminal(self):
        print("Rows: "+str(self.rows))
        print("Cols: "+str(self.cols))
        print("Spread: "+str(self.spread)+"mm")
        print("Nozzles: "+str(self.nozzles))
        print("Print Area: "+str(self.printArea)+"mm")
        rowStr = ""
        for y in range(0, self.rows):
            rowStr = ""
            for x in range(0, self.cols):
                #color = cv.Get2D(self.img, y, x)
                color = self.img[y, x]
                #if color != [225, 225, 255]:
                #print(color)
                #print(self.red)
                #print(self.green)
                #print(self.blue)
                #print(self.black)
                if (color == self.red).all():
                    rowStr += termcolor.colored(" ", 'white', 'on_red')
                elif (color == self.green).all():
                    rowStr += termcolor.colored(" ", 'white', 'on_green')
                elif (color == self.blue).all():
                    rowStr += termcolor.colored(" ", 'white', 'on_blue')
                elif (color == self.black).all():
                    rowStr += " "
                else:
                    rowStr += termcolor.colored(" ", 'white', 'on_white')
            print(rowStr)


    def printImage(self, name, path): # Metodo para pasar de imagen a gcode
        print('entro a print Image')
        print(path)
        #path = "imagenpantalla.png"
        try:
            f = open(name+'.nc', 'r')
            f.close
        except:
           # print(fileName+" ")
            f = open(name+'.nc', 'w')
            f.close
        #else:
            #print("error al imprimir imagen")
            #print(f)
            """
            answer = input(
                fileName+" El archivo YA EXISTE, desea reescribirlo? (S/n): ")
            if (answer == 's')or(answer == 'S')or(answer == ''):
                     f = open(fileName, 'w')
                print(fileName+' Sera sobreescrito')
                f.close
            elif answer == 'n'or(answer == 'N'):
                raise NameError("Path INCORRECTO")
            else:
                raise NameError("Opcion INCORRECTA")
                """
            #return f
        # CONVIERTIENDOLO A  8BIT GREYSCALE
        try:
            img = imageio.imread(path, as_gray=True, pilmode="RGB")

        except:
            raise NameError("Something is wrong with image. Probably path")


        # ARCHIVO DONDE SE VA A GUARDAR EL GCODE
        #f = fileDialog(name+".nc")

        try:
            x_offset_mm = float(0)
            y_offset_mm = float(0)
            output_image_horizontal_size_mm = float(400)
            pixel_size_mm = float(0.2)
            feedrate = int(100)
            max_laser_power = int(255)
            number_of_colours = int(5)

        except:
            raise NameError("NO INGRESO NUMEROS COMO PARAMETROS")

        # REDIMENSIONANDO LA IMAGEN
        y_size_input = len(img)
        x_size_input = len(img[0])

        # CALCULANDO LA ESCALA
        x_size_output = output_image_horizontal_size_mm/pixel_size_mm
        scale = x_size_output/x_size_input

        # REDIMENSIONANDO LA IMAGEN
        img = PIL.Image.fromarray(img,)
        img = img.resize((int(scale*x_size_input), int(scale*y_size_input)))
        img = numpy.asarray(img)

        # CALCULANDO TAMAÑO DE IMAGEN
        y_size_output = len(img)
        x_size_output = len(img[0])

        # negative for laser etching 
        img=numpy.subtract(255,img)

        # set max value of image colour to number of colours 
        number_of_colours -= 1
        img = numpy.rint(numpy.multiply(img, number_of_colours/255))

        #GUARDAR PREVIEW
        img_out=numpy.empty((x_size_output,y_size_output))
        img_out=numpy.rint(numpy.multiply(img, 255/number_of_colours))
        img_out = img_out.astype(numpy.uint8)
        imageio.imwrite('out_img.png',img_out) #preview de la imagen pero en escala de grises

        #CONVERTIR A feedrates
        img = numpy.rint(numpy.multiply(img, max_laser_power/number_of_colours))

        # display preview before processing - requires closing plot window before proceeding 
        # img2=numpy.subtract(number_of_colours,img)
        # matplotlib.pyplot.imshow(img2, cmap='gray')
        # matplotlib.pyplot.show()

        # VOLTEAR PARA QUE SEA MAS SIMPLE?
        img=numpy.flip(img,0)

        ########################### G CODE #################################
        f.write(" \n") 

        for y in range(y_size_output):
            prev_power=int(0)
          
            if 1-y%2:
                
                for x in range(x_size_output):
                    if (x == 0  and img[y][x] != 0): #first point, diffrent from 0
                        f.write("G0 X"+str(round(x*pixel_size_mm+x_offset_mm,4))+" Y" + str(round(y*pixel_size_mm+y_offset_mm,4))+"\n")                                                                                                              
                        f.write("M3 S"+str(int(img[y][x]))+"\n")                                                                     
                        prev_power = int(img[y][x])
                    elif x==(x_size_output-1):#eol
                        if (prev_power==0):
                            f.write("M5 S0\n")
                        else:
                            f.write("G1 X"+str(round((x)*pixel_size_mm+x_offset_mm,4))+" Y" + str(round(y*pixel_size_mm+y_offset_mm,4))+"\n")      
                            f.write("M5 S0\n")
                        prev_power=0
                    elif (prev_power != img[y][x]):#different power
                        if (prev_power==0): #transition from 0 to higher power
                            f.write("G0 X"+str(round((x-1)*pixel_size_mm+x_offset_mm,4))+" Y" + str(round(y*pixel_size_mm+y_offset_mm,4))+"\n")      
                            f.write("M3 S"+str(int(img[y][x]))+"\n") 
                            prev_power = int(img[y][x])
                        if(prev_power != 0):# transition from some power to another
                            f.write("G1 X"+str(round((x-1)*pixel_size_mm+x_offset_mm,4))+" Y" + str(round(y*pixel_size_mm+y_offset_mm,4))+"\n")      
                            f.write("M3 S"+str(int(img[y][x]))+"\n")  
                            prev_power = int(img[y][x])
            else:
                # prev_power=int(0)
                for x in reversed(range(x_size_output)):
                    if (x == x_size_output-1  and img[y][x] != 0): #first point, diffrent from 0
                        f.write("G0 X"+str(round(x*pixel_size_mm+x_offset_mm,4))+" Y" + str(round(y*pixel_size_mm+y_offset_mm,4))+"\n")                                                                                                              
                        f.write("M3 S"+str(int(img[y][x]))+"\n")                                                                     
                        prev_power = int(img[y][x])
                    elif x==0:#eol
                        if (prev_power==0):
                            f.write("M5 S0\n")
                        else:
                            f.write("G1 X"+str(round((x)*pixel_size_mm+x_offset_mm,4))+" Y" + str(round(y*pixel_size_mm+y_offset_mm,4))+"\n")      
                            f.write("M5 S0\n")
                        prev_power=0
                    elif (prev_power != img[y][x]):#different power
                        if (prev_power==0): #transition from 0 to higher power
                            f.write("G0 X"+str(round((x)*pixel_size_mm+x_offset_mm,4))+" Y" + str(round(y*pixel_size_mm+y_offset_mm,4))+"\n")      
                            f.write("M3 S"+str(int(img[y][x]))+"\n")                                                                     
                            prev_power = int(img[y][x])
                        if(prev_power != 0):# transition from some power to another
                            f.write("G1 X"+str(round((x)*pixel_size_mm+x_offset_mm,4))+" Y" + str(round(y*pixel_size_mm+y_offset_mm,4))+"\n")      
                            f.write("M3 S"+str(int(img[y][x]))+"\n")                                                                     
                            prev_power = int(img[y][x])
        f.close()
        print("GCODE GENERADO CON EXITO xd")


    def printImageStr(self, name, path): # Metodo para pasar de imagen a gcode
        print('entro a print Image Str')
        print(path)
        strImage = ""

        try:
            img = imageio.imread(path, as_gray=True, pilmode="RGB")

        except:
            raise NameError("Something is wrong with image. Probably path")


        # ARCHIVO DONDE SE VA A GUARDAR EL GCODE
        #f = fileDialog(name+".nc")

        try:
            x_offset_mm = float(0)
            y_offset_mm = float(0)
            output_image_horizontal_size_mm = float(10)
            pixel_size_mm = float(0.2)
            feedrate = int(100)
            max_laser_power = int(255)
            number_of_colours = int(5)

        except:
            raise NameError("NO INGRESO NUMEROS COMO PARAMETROS")

        # REDIMENSIONANDO LA IMAGEN
        y_size_input = len(img)
        x_size_input = len(img[0])

        # CALCULANDO LA ESCALA
        x_size_output = output_image_horizontal_size_mm/pixel_size_mm
        scale = x_size_output/x_size_input

        # REDIMENSIONANDO LA IMAGEN
        img = PIL.Image.fromarray(img,)
        img = img.resize((int(scale*x_size_input), int(scale*y_size_input)))
        img = numpy.asarray(img)

        # CALCULANDO TAMAÑO DE IMAGEN
        y_size_output = len(img)
        x_size_output = len(img[0])

        # negative for laser etching 
        img=numpy.subtract(255,img)

        # set max value of image colour to number of colours 
        number_of_colours -= 1
        img = numpy.rint(numpy.multiply(img, number_of_colours/255))

        #GUARDAR PREVIEW
        img_out=numpy.empty((x_size_output,y_size_output))
        img_out=numpy.rint(numpy.multiply(img, 255/number_of_colours))
        img_out = img_out.astype(numpy.uint8)
        imageio.imwrite('out_img.png',img_out) #preview de la imagen pero en escala de grises

        #CONVERTIR A feedrates
        img = numpy.rint(numpy.multiply(img, max_laser_power/number_of_colours))

        # display preview before processing - requires closing plot window before proceeding 
        # img2=numpy.subtract(number_of_colours,img)
        # matplotlib.pyplot.imshow(img2, cmap='gray')
        # matplotlib.pyplot.show()

        # VOLTEAR PARA QUE SEA MAS SIMPLE?
        img=numpy.flip(img,0)

        ########################### G CODE #################################
        strImage += (" \n") 

        for y in range(y_size_output):
            prev_power=int(0)
          
            if 1-y%2:
                
                for x in range(x_size_output):
                    if (x == 0  and img[y][x] != 0): #first point, diffrent from 0
                        strImage += ("G0 X"+str(round(x*pixel_size_mm+x_offset_mm,4))+" Y" + str(round(y*pixel_size_mm+y_offset_mm,4))+"\n")                                                                                                              
                        strImage += ("M3 S"+str(int(img[y][x]))+"\n")                                                                     
                        prev_power = int(img[y][x])
                    elif x==(x_size_output-1):#eol
                        if (prev_power==0):
                            strImage += ("M5 S0\n")
                        else:
                            strImage += ("G1 X"+str(round((x)*pixel_size_mm+x_offset_mm,4))+" Y" + str(round(y*pixel_size_mm+y_offset_mm,4))+"\n")      
                            strImage += ("M5 S0\n")
                        prev_power=0
                    elif (prev_power != img[y][x]):#different power
                        if (prev_power==0): #transition from 0 to higher power
                            strImage += ("G0 X"+str(round((x-1)*pixel_size_mm+x_offset_mm,4))+" Y" + str(round(y*pixel_size_mm+y_offset_mm,4))+"\n")      
                            strImage += ("M3 S"+str(int(img[y][x]))+"\n") 
                            prev_power = int(img[y][x])
                        if(prev_power != 0):# transition from some power to another
                            strImage += ("G1 X"+str(round((x-1)*pixel_size_mm+x_offset_mm,4))+" Y" + str(round(y*pixel_size_mm+y_offset_mm,4))+"\n")      
                            strImage += ("M3 S"+str(int(img[y][x]))+"\n")  
                            prev_power = int(img[y][x])
            else:
                # prev_power=int(0)
                for x in reversed(range(x_size_output)):
                    if (x == x_size_output-1  and img[y][x] != 0): #first point, diffrent from 0
                        strImage += ("G0 X"+str(round(x*pixel_size_mm+x_offset_mm,4))+" Y" + str(round(y*pixel_size_mm+y_offset_mm,4))+"\n")                                                                                                              
                        strImage += ("M3 S"+str(int(img[y][x]))+"\n")                                                                     
                        prev_power = int(img[y][x])
                    elif x==0:#eol
                        if (prev_power==0):
                            strImage += ("M5 S0\n")
                        else:
                            strImage += ("G1 X"+str(round((x)*pixel_size_mm+x_offset_mm,4))+" Y" + str(round(y*pixel_size_mm+y_offset_mm,4))+"\n")      
                            strImage += ("M5 S0\n")
                        prev_power=0
                    elif (prev_power != img[y][x]):#different power
                        if (prev_power==0): #transition from 0 to higher power
                            strImage += ("G0 X"+str(round((x)*pixel_size_mm+x_offset_mm,4))+" Y" + str(round(y*pixel_size_mm+y_offset_mm,4))+"\n")      
                            strImage += ("M3 S"+str(int(img[y][x]))+"\n")                                                                     
                            prev_power = int(img[y][x])
                        if(prev_power != 0):# transition from some power to another
                            strImage += ("G1 X"+str(round((x)*pixel_size_mm+x_offset_mm,4))+" Y" + str(round(y*pixel_size_mm+y_offset_mm,4))+"\n")      
                            strImage += ("M3 S"+str(int(img[y][x]))+"\n")                                                                     
                            prev_power = int(img[y][x])
        print("STR GCODE GENERADO CON EXITO xd")
        return strImage



    def coordenadasapng(self, nombre, cadena):
        #cadena = "153,228;153,229;152,230;152,230;152,231;152,231;152,231;152,231;152,231;152,231;152,231;152,231;152,232;152,231;152,231;152,231;152,231;152,231;152,231;152,231;152,231;152,229;152,228;152,225;152,223;154,220;155,218;155,217;156,215;157,213;158,211;158,209;160,20698,120;98,122;99,123;99,124;99,126;99,128;99,129;99,133;99,136;99,137;99,140;99,142;100,144;100,146;101,150;101,152;102,155;102,158;102,159;103,161;104,163;105,166;107,168;108,169;132,131;133,133;134,135;134,138;135,140;136,144;136,147;137,149;138,154;140,158;141,161;142,163;143,166;144,167;145,168;147,171;148,172;60,181;60,182;60,184;61,185;62,186;65,188;67,190;68,191;70,193;72,194;74,195;80,201;84,204;87,206;92,209;94,211;97,212;99,213;104,215;107,215;110,215;126,216;129,217;134,218;138,218;142,218;145,217;149,218;155,217;160,215;163,215;175,211;178,210;182,208;186,206;188,204;193,200;194,198;196,196;197,193;197,190;197,187;197,185;197,184;197,181;196,180;"
        img = PIL.Image.new("RGB", (500,500), "white")
        draw = PIL.ImageDraw.Draw(img)
        dotSize = 5
        coordenadas1 = cadena.split(";")
        for x in coordenadas1[:-1]:
            coordenadas2=x.split(",")
            if(len(coordenadas2)==2):
                x = int(coordenadas2[0])
                y = int(coordenadas2[1])
                draw.ellipse([x,y,x+dotSize-1,y+dotSize-1], fill="black")
        img.save(nombre)

if __name__ == "__main__":
    #Setup Command line arguments
    parser = argparse.ArgumentParser(prog="image_to_gcode.py",
                                     usage="%(prog)s [options] input...",
                                     description="Convert bitmaps to gcode."
                                     )
    parser.add_argument("-o", "--output",
                        default=sys.stdout,
                        help="Output file, defaults to stdout"
                        )
    parser.add_argument("-s", "--spread",
                        default="3.175",
                        help="Nozzle spread (mm). Default: %(default)s"
                        )
    parser.add_argument("-n", "--nozzles",
                        default="12",
                        help="Nozzle count. Default: %(default)s"
                        )
    parser.add_argument("-a", "--area",
                        default="[1000, 1000]",
                        help="Print area in millimeters. Default: %(default)s"
                        )
    parser.add_argument("-f", "--feedrate",
                        default="1000",
                        help="Print feedrate. Default: %(default)s"
                        )
    parser.add_argument("input",
                        help="input file, defaults to stdin"
                        )
    parser.add_argument('--version',
                        action='version',
                        version="%(prog)s 0.0.1-dev"
                        )
    parser.add_argument("-r", "--red",
                        default="[0, 0]",
                        help="Head offset in millimeters. Default: %(default)s"
                        )
    parser.add_argument("-g", "--green",
                        default="[30.5, 0.1]",
                        help="Head offset in millimeters. Default: %(default)s"
                        )
    parser.add_argument("-b", "--blue",
                        default="[0, 0]",
                        help="Head offset in millimeters. Default: %(default)s"
                        )
    parser.add_argument("-k", "--black",
                        default="[0, 0]",
                        help="Head offset in millimeters. Default: %(default)s"
                        )

    #Always output help by default
    if len(sys.argv) == 1:
        parser.print_help()
        sys.exit(0)  # Exit after help display

    args = parser.parse_args()
    offsets = [ast.literal_eval(args.red),
               ast.literal_eval(args.green),
               ast.literal_eval(args.blue),
               ast.literal_eval(args.black)
               ]

    imageProcessor = ImageToGcode(img=args.input,
                                  spread=float(args.spread),
                                  nozzles=float(args.nozzles),
                                  area=ast.literal_eval(args.area),
                                  feedrate=float(args.feedrate),
                                  offsets=offsets
                                  )



    
"""
import numpy
import matplotlib
import matplotlib.pyplot
import imageio
import PIL.Image
import sys
"""

    
        



"""
img = PIL.Image.new("RGB", (400,400), "white")
draw = PIL.ImageDraw.Draw(img)

coords = [(100,70), (220, 310), (200,200)]
dotSize = 5

for (x,y) in coords:
    draw.ellipse([x,y,x+dotSize-1,y+dotSize-1], fill="black")

img.save('imagenpantalla.png')
img.show()
"""
