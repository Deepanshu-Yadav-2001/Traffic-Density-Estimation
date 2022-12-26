# -*- coding: utf-8 -*-
"""
Created on Wed Mar 31 19:08:22 2021

@author: Deepanshu Yadav
"""
import matplotlib.pyplot as plt
fo = open("1mtd4dynamic.txt","r")
go = open("1mtd4queue.txt","r")

f = open("5mtd4dynamic.txt","r")
g = open("5mtd4queue.txt","r")

jl = 1/3
time = [i*jl for i in range(1148)]

vald = []
valq = []
for x in fo:
    ls = x.split(',')
    vald.append(float(ls[1]))
for x in go:
    ls = x.split(',')
    valq.append(float(ls[1]))

dyn = []
que = []
for x in f:
    ls = x.split(',')
    dyn.append(float(ls[1]))
for x in g:
    ls = x.split(',')
    que.append(float(ls[1]))
    
ans = 0    
for i in range(len(time)):
    ans += abs(dyn[i]-vald[i])
    ans += abs(que[i]-valq[i])
ut = len(time)/ans
print(ut)

plt.plot(time,dyn)
plt.plot(time,que)
plt.show()