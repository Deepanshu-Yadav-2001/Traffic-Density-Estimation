# -*- coding: utf-8 -*-
"""
Created on Wed Mar 31 12:36:05 2021

@author: Deepanshu Yadav
"""

import matplotlib.pyplot as plt

fo = open("1_3d.txt","r")
go = open("1_3q.txt","r")

f = open("5_3d.txt","r")
g = open("5_3q.txt","r")

jl = 1/3
time = [i*jl for i in range(1,1148)]

vald = []
valq = []
for x in fo:
    vald.append(float(x))
for x in go:
    valq.append(float(x))

dyn = []
que = []
for x in f:
    dyn.append(float(x))
for x in g:
    que.append(float(x))
    
ans = 0    
for i in range(len(time)):
    ans += abs(dyn[i]-vald[i])
    ans += abs(que[i]-valq[i])
ut = len(time)/ans
print(ut)

plt.plot(time,dyn)
plt.plot(time,que)
plt.show()