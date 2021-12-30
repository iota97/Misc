# Diametro circonferenza circoscritta, poligono regolare

import math

lato = 12.7
n_lati = 36
diametro = lato/math.sin(math.pi/n_lati)

print(diametro)