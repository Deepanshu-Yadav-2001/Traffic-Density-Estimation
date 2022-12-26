# -*- coding: utf-8 -*-
"""
Created on Wed Mar 10 22:48:59 2021

@author: Deepanshu Yadav
"""

import matplotlib.pyplot as plt
import numpy as np

queue =[]
dynamic = []
time = []

queue1 =[]
dynamic1 = []
time1 = []

f = open("myfile800-800.txt", "r")
g = open("frame5.txt", "r")
for x in f:
  dyn = float(x[-8:])
  dynamic.append(dyn)
  qu = float(x[-20:-12])
  queue.append(qu)
  tm = float(x[0:-24])
  time.append(tm)

for x in g:
  dyn = float(x[-8:])
  dynamic1.append(dyn)
  qu = float(x[-20:-12])
  queue1.append(qu)
  tm = float(x[0:-24])
  time1.append(tm)
  
diffd = []
for i in range(382):
    m1 = np.interp(i,time,dynamic)
    m2 = np.interp(i,time1,dynamic1)
    diffd.append(abs(m1-m2))

qued = []
k = 0
for i in range(len(time1)):
    j = time1[i]
    m1 = np.interp(j,time,queue)
    m2 = np.interp(j,time1,queue1)
    qued.append(abs(m1-m2))
    k += abs(m1-m2)
    
for i in range(len(time1)):
    j = time1[i]
    m1 = np.interp(j,time,dynamic)
    m2 = np.interp(j,time1,dynamic1)
    qued.append(abs(m1-m2))
    k += abs(m1-m2)
print(k/len(time1))    

plt.plot( time , queue , label = "Queue density",color='b')

plt.plot( time , dynamic , label = "Dynamic density",color='g')

plt.show()
