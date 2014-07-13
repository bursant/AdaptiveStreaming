'''
Created on 13 lis 2013

@author: bursant
'''

import numpy as np
import matplotlib.pyplot as plt
import math

def ControlModule(proportional_factor, q_tks, q_ref, delta, q_tkprevs, Vl, v_kprev, W):
    exp = math.exp(proportional_factor*(q_tks-q_ref))
    Fq = 2 * exp / (exp+1)
    Ft = delta / (delta-(q_tks-q_tkprevs))
    Fv = Vl/(v_kprev+W) + W/(Vl+W)
    return Fq*Ft #*Fv

def SwitchingLogic(F, T_predicted, T_prevpredicted, q_tks, q_ref, counter, m, v_kprev, reps):
    vdashk = F*T_predicted
    if q_tks < q_ref/2:
        return max([rep for rep in reps if rep < T_prevpredicted]+[reps[0]]), counter
    elif vdashk > v_kprev:
        counter += 1
        if counter > m:
            return max([rep for rep in reps if rep < T_predicted]+[reps[0]]), 0
    elif vdashk < v_kprev:
        counter = 0
    return v_kprev, counter
    
def readBandwidth(filename):
    bandlistplot = [[], []]
    bandlist = []
    bandfile = open(filename, 'rU')
    for line in bandfile.readlines():
        time, band = line.split(':')
        band = int(band.replace("\n",""))
        start, stop = time.split('-')
        for i in range(int(start), int(stop)+1):
            bandlistplot[0].append(i)
            bandlistplot[1].append(band)
        for i in range(int(start)+1, int(stop)+1):
            bandlist.append(band)
    return bandlist, bandlistplot

def readRepresentations(filename, segments):
    repfile = np.loadtxt(filename)
    [segmentDuration, reps] = repfile[0], repfile[1:]
    segmentDuration = int(segmentDuration)
    fx = range(0, segments*segmentDuration, segmentDuration)
    return segmentDuration, fx, reps

def representationDataConversion(fx, fy):
    tmptx = []
    tmpty = []
    for index in range(0, len(fx)):
        tmptx.append(fx[index])
        tmpty.append(fy[index])
        if index+1 < len(fx) and fy[index] != fy[index+1]:
            tmptx.append(fx[index])
            tmpty.append(fy[index+1]) 
    return tmptx, tmpty

def plotBandwidth(bandlistplot, ax1):
    ax1.set_xlabel('time (s)', fontdict=font)
    ax1.set_ylabel('bandwidth (kbps)', fontdict=font, color='b')
    ax1.plot(bandlistplot[0], bandlistplot[1], 'b--')

def plotReps(fx, fy, ax2):
    ax2.set_xlabel('time (s)', fontdict=font)
    ax2.set_ylabel('representations (kbps)', fontdict=font, color='g')
    ax2.plot(fx, fy, 'g')

if __name__ == '__main__':
    
    segments = 150
    font = {'family' : 'serif', 'color'  : 'darkred', 'weight' : 'normal', 'size'   : 16 }
    fig, ax1 = plt.subplots()
    ax2 = ax1.twinx()
    
    #Bandwidth data
    bandlist, bandlistplot = readBandwidth("bandwidth1")
    
    #Bandwidth plot
    plotBandwidth(bandlistplot, ax1)

    #Representation data
    delta, fx, reps = readRepresentations("representation1", segments)
    fy = []
    
    #Segments computation   
    bufferContents = 10
    bufferSize = 40.0
    prevTimer = 0.0
    timer = 0.0 #ms
    
    proportional_factor = 1
    q_ref = bufferSize*0.5*delta
    W = 2
    q_tks = bufferContents*delta
    v_kprev = reps[0]
    q_tkprevs = q_tks
    counter = 0
    T_prevpredicted = 300
    T_predicted = 300
    m = 3
    v_k = reps[0]
    
    for segmentIndex in range (0, segments):   
    
        q_tks = bufferContents*delta - (timer - prevTimer)
        F = ControlModule(proportional_factor, q_tks, q_ref, delta, q_tkprevs, reps[-1], v_kprev, W)
        v_k, counter = SwitchingLogic(F, T_predicted, T_prevpredicted, q_tks, q_ref, counter, m, v_kprev, reps)
    
        segmentLength = delta*v_k    
        
        cummulativeTimer = 0.0
        
        while segmentLength > 0:
            second = int(np.floor(timer))
            segmentLength = segmentLength - bandlist[second]/1000.0
            timer += 0.001
            cummulativeTimer += 0.001
            if timer - prevTimer >= delta:
                bufferContents -= 1
                if bufferContents < 0: 
                    print "Buffer is empty!!!"
                prevTimer = timer
        bufferContents += 1
        fy.append(v_k)
        print "rep (kbps): " + str(v_k) + " buffer: " + str(bufferContents) + " timer: " + str(timer)
        
        tmp = T_prevpredicted
        T_prevpredicted = T_predicted
        T_predicted = (delta*v_k/cummulativeTimer + tmp)/2
        v_kprev = v_k
        q_tkprevs = q_tks
    
    #Representation step chart data conversion          
    fx, fy = representationDataConversion(fx, fy)
    
    #Segments plot
    plotReps(fx, fy, ax2)
    
    #Set scale
    limit = max(fy+bandlistplot[1])*1.05
    ax1.set_ybound(0, limit)
    ax2.set_ybound(0, limit)
    #ax1.set_xbound(0, 250)
    #ax2.set_xbound(0, 250)
    
    #Show plots
    plt.show()