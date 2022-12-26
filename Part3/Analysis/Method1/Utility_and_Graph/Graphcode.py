# -*- coding: utf-8 -*-
"""
Created on Fri Mar 26 13:19:59 2021

@author: Deepanshu Yadav
"""
import matplotlib.pyplot as plt
import numpy as np

metricinv = np.array([0.028636586192371007,0.03962743176850391,0.04648038604261069,0.050881332926568476,0.05407629092352996,0.05936889377758598,0.06770523420949791,0.06793012900739727,0.06775472512510511,0.08393932231138886,0.08379420816664042,0.0924724016020477,0.09116482891393231,0.0928230213986008])
metric = 1/metricinv
frame = [10,15,20,25,30,35,40,45,50,60,70,80,90,100]
time = [475,322,256,215,197,154,133,125,116,105,114,112,105,100]
plt.plot(time,metric,marker='o',linestyle='-')

#for i in range(len(time)):
    #plt.annotate(frame[i],(time[i],metric[i]))
plt.xlabel('Runtime(in seconds)')
plt.ylabel('Utility Metric')
plt.show()