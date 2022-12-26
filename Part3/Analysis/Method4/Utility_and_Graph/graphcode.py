# -*- coding: utf-8 -*-
"""
Created on Wed Mar 31 21:37:30 2021

@author: Deepanshu Yadav
"""
import matplotlib.pyplot as plt

threads = [1,2,3,4,5]
x = [75+901,78+540,76+505,70+451,82+409]
y = [float('inf'),2369.601138333268,1574.8239318036474,1350.7797917601163,1324.3447700425272]

plt.plot(x,y,linestyle='-',marker='o')
for i in range(len(threads)):
    plt.annotate(str(threads[i])+" threads",(x[i],y[i]))
plt.xlabel('Runtime(in seconds)')
plt.ylabel('Utility Metric')
plt.title("Method4")
plt.show()