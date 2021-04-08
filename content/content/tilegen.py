import struct 

import sys
import os
#Size
#Height
#Tex
#AnimCount
#Anim - startframe/endframe/duration
#Type
#cost

#tilegen --tex=res/residential_towers.png --size=8 --height=16 --cost=50 --tiletype=1 --anim=0,0,1

import argparse
parser = argparse.ArgumentParser()
parser.add_argument('--tex', help='foo help')
parser.add_argument('--icon', help="foo help")
parser.add_argument('--size', help='foo help')
parser.add_argument('--height', help='foo help')
parser.add_argument('--cost', help='foo help')
parser.add_argument('--tiletype', help='foo help')
parser.add_argument('--anim', help='foo help')
parser.add_argument('--out', help='foo help')
args = parser.parse_args()

size = int(args.size)
height = int(args.height)
cost = int(args.cost)
tiletype = int(args.tiletype)
anim = tuple(args.anim.split(","))
tex = open(args.tex, "rb").read()
icon = open(args.icon, "rb").read()

#size = 8
#height = 16
#tex = open(args[1], "rb").read()
#anim = (0,0,1.0)
#tiletype = 0
#cost = 50
texlen = os.path.getsize(args.tex)
iconlen = os.path.getsize(args.icon)
out = struct.pack("4I", size, height, cost, tiletype)

out+= struct.pack("I", texlen)
out+= struct.pack("{}p".format(texlen),tex)

out+= struct.pack("I", iconlen)
out+= struct.pack("{}p".format(iconlen),icon)

out+=struct.pack("I", round(len(anim)/3))
for i in range(len(anim)):
	out+=struct.pack("f", float(anim[i]))

open("{}.tile".format(args.out), "wb").write(out)