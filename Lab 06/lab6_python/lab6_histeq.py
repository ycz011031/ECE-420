# ECE420 - Spring2017
# Lab6 - Part 1: Histogram Equilization

import numpy 
from scipy import misc
import matplotlib.pyplot as plt
import copy
from imageio import imread
import numpy as np

# Implement This Function

def get_hist (pic,M,N):
    hist_out = np.zeros(65535)
    for i in range(M):
        for j in range(N):
            itr = pic[i][j]
            hist_out[itr] +=1
    return hist_out

def get_cdf(a):
    cdf = np.zeros(65535)
    result = 0
    for i in range(len(a)):
        result += a[i] 
        cdf[i] = result
    return cdf

def get_h(cdf,M,N):
    h = np.zeros(65535)
    cdf_min = 0
    for i in range (len(cdf)):
        if not(cdf[i] == 0):
            cdf_min = 0
            break
    for j in range(len(cdf)):
        result = int(((cdf[j]-cdf_min)/(M*N+1))*(65535-1))
        h[j] = result
    return h


def histeq(pic):
    # Follow the procedures of Histogram Equalizaion
    # Modify the pixel value of pic directly
    [M,N] = pic.shape
    hist_cnt = get_hist(pic,M,N)
    print ("hist generation complete")

    cdf = get_cdf(hist_cnt)
    print ("cdf generation complete")

    h = get_h(cdf,M,N)
    print ("h generation complete")


    for k in range(M):
        for l in range(N):
            itr = pic[k][l]
            pic[k][l] = h[itr]


    return pic

# Histogram Equilization
eco_origin = imread('eco.tif')
eco_histeq = copy.deepcopy(eco_origin)
# Call to histeq to perform Histogram Equilization
eco_histeq = histeq(eco_histeq)
# Show the result in two windows
fig_eco_origin = plt.figure(1)
fig_eco_origin.suptitle('Original eco.tif', fontsize=14, fontweight='bold')
plt.imshow(eco_origin,cmap='gray',vmin = 0, vmax = 65535)
fig_eco_histeq = plt.figure(2)
fig_eco_histeq.suptitle('Histrogram Equalized eco.tif', fontsize=14, fontweight='bold')
plt.imshow(eco_histeq,cmap='gray',vmin = 0, vmax = 65535)
plt.show()