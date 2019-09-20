#!/bin/python

#Compute light from normal + albedo

# Light position
position = [-10, 0, 3]

# Light color
color = [255, 255, 255]

# Using more step will make a rotating light series
step = 0

# .png and frame number added automatically
output = "out" 

from PIL import Image
import math

def render(x, y, z, r, g, b, path):
	albedo = Image.open("albedo.png")
	normal = Image.open("normal.png")

	lc = [r/255, g/255, b/255]
	mag = math.sqrt(x*x+y*y+z*z)
	l = [x/mag, y/mag, z/mag]
	size = albedo.size
	out = Image.new('RGB', size)
	a = albedo.load()
	n = normal.load()
	o = out.load()

	for i in range(size[0]):
		for j in range(size[1]):
			li = l[0]*(n[i, j][0]/128 - 1) + l[1]*(n[i, j][1]/128 - 1) + l[2]*(n[i, j][2]/128 - 1)
			if li < 0:
				li = 0
			o[i, j] = (int(a[i, j][0]*li*lc[0]), int(a[i, j][1]*li*lc[1]), int(a[i, j][2]*li*lc[2]))

	out.save(path)

if (step == 0):
	render(position[0], position[1], position[2], color[0], color[1], color[2], output+".png")
else:
	f = 2*math.pi/step
	for n in range(step):
		x = position[0]*math.sin(n*f) + position[1]*math.cos(n*f)
		y = position[0]*math.cos(n*f) - position[1]*math.sin(n*f)
		z = position[2]
		render(x, y, z, color[0], color[1], color[2], output+str(n)+".png")
