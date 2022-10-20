#!/bin/python

depth_image = "depth.png"
output = "stereogram.png" 

from PIL import Image
import math, random

depth = Image.open(depth_image)
size = depth.size
d = depth.load()

out = Image.new('RGB', size)
o = out.load()

EYE_SEPARATION = 5000.0
OBSERVER_DISTANCE = 12000.0

for i in range(size[0]):
		for j in range(size[1]):

			y = i
			for _ in range(64):
				z = (d[y, j][0]/255.0 + 0.5)*200
				s = math.floor(EYE_SEPARATION*z/(z+OBSERVER_DISTANCE))
				if (s < 1.0 or y < 0.0):
					break
				y = math.floor(y-s)

			random.seed(y+j*12341145+331211)
			c = random.randint(0, 255)
			o[i, j] = (c, c, c)

out.save(output)
