#!/bin/python

# Creare 2 immagini 1.png e 2.png della stessa dimensione, in trasparenza quello che non deve apparire

from PIL import Image
import math

in1 = Image.open("1.png")
in2 = Image.open("2.png")
size = in1.size
i1 = in1.load()
i2 = in2.load()

out = Image.new('RGBA', size)
o = out.load()
for i in range(size[0]):
		for j in range(size[1]):
			if j % 2 == 0:
				if i2[i, j][3] > 127:
					c = (54, 57, 64, 255)
				else:
					c = (0, 0, 0, 0)
			else:
				if i1[i, j][3] > 127:
					c = (0, 0, 0, 255)
				else:
					c = (0, 0, 0, 0)
			o[i, j] = c

out.save("out.png")