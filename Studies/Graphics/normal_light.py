#!/bin/python

#Compute light from normal + albedo

from PIL import Image
import math

light_direction = [0, 0, -10]
light_color = [50, 50, 255]
albedo = Image.open("albedo.png")
normal = Image.open("normal.png")

lc = [light_color[0]/255, light_color[1]/255, light_color[2]/255]
mag = math.sqrt(light_direction[0]*light_direction[0]+light_direction[1]*light_direction[1]+light_direction[2]*light_direction[2])
l = [light_direction[0]/mag, light_direction[1]/mag, light_direction[2]/mag]
size = albedo.size
out = Image.new('RGB', size)
a = albedo.load()
n = normal.load()
o = out.load()

for i in range(size[0]):
	for j in range(size[1]):
		li = l[0]*(n[i, j][0]/255) + l[1]*(n[i, j][1]/255) + l[2]*(n[i, j][2]/255)
		if li < 0:
			li +=1
		o[i, j] = (int(a[i, j][0]*li*lc[0]), int(a[i, j][1]*li*lc[1]), int(a[i, j][2]*li*lc[2]))

out.save("out.png")
