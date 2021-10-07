#!/usr/bin/env python
# -*- coding: utf-8 -*-

from z3 import *
def mixOpt(L, I, J, A):
	x = [Real(l) for l in L]
	o = Optimize()

	for j in range(len(J)):
		o.add(sum(x[i]*A[i][j] for i in range(len(I))) <= J[j])

	o.maximize(sum(x[i]*I[i] for i in range(len(I))))
	o.check()
	print(o.model())

mixOpt(["xL", "xP"], [3000, 5000], [11, 70, 18, 145], [[1, 7, 0, 10], [1, 0, 3, 20]])