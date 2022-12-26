# -*- coding: utf-8 -*-
"""
Created on Mon Mar 29 08:14:52 2021

@author: Deepanshu Yadav
"""

import matplotlib.pyplot as plt
import numpy as np

metricinv = np.array([0.5634867829119448,0.5278854184829999,0.4802790592850912,0.46564196687009624,0.46564196687009624,0.355313889276373,0.35296126678291145,0.26739750043592,0.22834430950305112,0.22553681517000876])
metric = 1/metricinv
frame = [(800,800),(900,900),(1000,1000),(1000,1200),(1000,1500),(1200,1200),(1200,1500),(1350,1350),(1500,1500),(1700,1700)]
time = [272,328,390,457,580,614,662,741,894,1026]
plt.plot(time,metric,marker='o',linestyle='-',label='ha')

#for i in range(len(time)):
    #plt.annotate(frame[i],(time[i],metric[i]))
plt.xlabel('Runtime(in seconds)')
plt.ylabel('Utility Metric')
plt.show()