#!/bin/python

# Compute cartoon from normal + albedo

# Light
light_position = [3, 2, 5]

# Highlight and shadow
shadow_factor = 0.1
highlight_factor = 0.97

# Outline
outline_threshold = 0.08
outline_color = [0, 0, 0]

# Posterize
posterize_factor = 3

# Albedo image
albedo_image = "albedo.png"

# Normal image
normal_image = "normal.png"

# Output image
output = "cartoon.png" 

from PIL import Image
import math

albedo = Image.open(albedo_image)
normal = Image.open(normal_image)
size = albedo.size
a = albedo.load()
n = normal.load()
p = posterize_factor
l = light_position

lm = math.sqrt(l[0]*l[0]+l[1]*l[1]+l[2]*l[2])
l[0] /= lm
l[1] /= lm
l[2] /= lm

out = Image.new('RGB', size)
o = out.load()
for i in range(size[0]):
		for j in range(size[1]):

			# Kernel edge detect
			i0 = max(i-1, 0)
			i1 = min(i+1, size[0]-1)
			j0 = max(j-1, 0)
			j1 = min(j+1, size[1]-1)
			f = 4*(a[i, j][0]+a[i, j][1]+a[i, j][2])
			f -= (a[i0, j][0]+a[i0, j][1]+a[i0, j][2])
			f -= (a[i1, j][0]+a[i1, j][1]+a[i1, j][2])
			f -= (a[i, j0][0]+a[i, j0][1]+a[i, j0][2])
			f -= (a[i, j1][0]+a[i, j1][1]+a[i, j1][2])

			if abs(f/1024) < outline_threshold:

				# Normal lighting
				s = l[0]*(n[i, j][0]/127-1)+l[1]*(n[i, j][1]/127-1)+l[2]*(n[i, j][2]/127-1)

				if s > highlight_factor:
					s = 1.5
				elif s < -shadow_factor:
					s = 0.5
				else:
					s = 1.0

				o[i, j] = (int(round(a[i, j][0]*s/p)*p), int(round(a[i, j][1]*s/p)*p), int(round(a[i, j][2]*s/p)*p))
			else:
				o[i, j] = (outline_color[0], outline_color[1], outline_color[2])

out.save(output)
