#!/bin/python

# Compute light from normal + albedo

# Using more step will make a rotating light series
step = 1

# Albedo image
albedo_image = "albedo.png"

# Normal image
normal_image = "normal.png"

# .png and frame number added automatically
output = "out" 

# Light position (Color and rotation must be AT LEAST the size of position)
position = [[-10, 0, 3]]

# Light color
color = [[255, 255, 255], [0, 0, 128]]

# Speed of rotation, 1 mean it will perform 360 degree over all the steps
rotation_speed = [1, -1.2, 0]

from PIL import Image
import math
import multiprocessing as mp

albedo = Image.open(albedo_image)
normal = Image.open(normal_image)
size = albedo.size
light = Image.new('RGB', size, (0, 0, 0))
a = albedo.load()
n = normal.load()
limg = light.load()

def add_light(x, y, z, r, g, b):
	mag = math.sqrt(x*x+y*y+z*z)
	l = [x/mag, y/mag, z/mag]

	for i in range(size[0]):
		for j in range(size[1]):
			li = max(l[0]*(n[i, j][0]/127 - 1) + l[1]*(n[i, j][1]/127 - 1) + l[2]*(n[i, j][2]/127 - 1), 0)
			limg[i, j] = (min(limg[i,j][0]+int(li*r), 255), min(limg[i,j][1]+int(li*g), 255), min(limg[i,j][2]+int(li*b), 255))

def render(path):
	out = Image.new('RGB', size)
	o = out.load()
	for i in range(size[0]):
		for j in range(size[1]):
			o[i, j] = (int(limg[i, j][0]/255*a[i, j][0]), int(limg[i, j][1]/255*a[i, j][1]), int(limg[i, j][2]/255*a[i, j][2]))
	out.save(path)

def do_step(s):
	print("Rendering frame #"+str(s+1))
	for i in range(len(position)):
		f = 2*float(rotation_speed[i])*math.pi/step
		x = position[i][0]*math.cos(s*f) + position[i][1]*math.sin(s*f)
		y = -position[i][0]*math.sin(s*f) + position[i][1]*math.cos(s*f)
		add_light(x, y, position[i][2], color[i][0], color[i][1], color[i][2])
	if step == 1:
		render(output+".png")
	else:
		render(output+str(s)+".png")

if len(position) > len(color) or len(color) > len(rotation_speed):
	print("Error: not enough color or speed for every light!")

else:
	pool = mp.Pool(mp.cpu_count())
	pool.map(do_step, range(step))
	pool.close()
