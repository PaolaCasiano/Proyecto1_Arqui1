from __future__ import absolute_import, division, print_function, unicode_literals
import os
import sys
import cv2 as cv
import argparse
import termcolor
import ast
import copy



class ImageToGcode():
    def __init__(self,
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