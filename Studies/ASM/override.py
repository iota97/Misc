import sys
import ctypes

def override(a, b):
	p = ctypes.POINTER(ctypes.c_byte*sys.getsizeof(a))
	f = ctypes.cast(id(a), p)
	f.contents[f.contents[:].index(a)] = b

override(5, 4)
if 2+2 == 5:
	print("Nani")
else:
	print("Ok")