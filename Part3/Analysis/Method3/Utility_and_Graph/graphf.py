# -*- coding: utf-8 -*-
"""
Created on Wed Mar 31 22:33:41 2021

@author: Deepanshu Yadav
"""
import matplotlib.pyplot as plt

threads = [1,2,3,4,6,7,9]
x = [177+76,150+76,150+66,127+67,123+67,125+64,122+66]
y = [float('inf'),33.285911057394486,19.554204887239663,14.823788094264005,11.098826983593481,11.13512045439756,6.956976088503074]

plt.plot(x,y,linestyle='-',marker='o')
for i in range(len(threads)):
    plt.annotate(str(threads[i])+" threads",(x[i],y[i]))
plt.xlabel('Runtime(in seconds)')
plt.ylabel('Utility Metric')
plt.title("Method3")
plt.show()