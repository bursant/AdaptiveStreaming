import matplotlib.pyplot as plt
import time as t 
import numpy as np
import calendar
import matplotlib.animation as animation

from dateutil import parser

logpath = "G:\Shared\dash.log"
logfile = open(logpath, "r")

line = logfile.readline()
line = line.strip("\n")
basetimestring, seq, band = line.split(";")
basetime_seconds = calendar.timegm(parser.parse(basetimestring).utctimetuple())
basetime_millis = int(basetimestring.split(".")[1][:3])
basetime = basetime_seconds*1000+basetime_millis
prevband = int(band)/1024.0

x = [0]
y = [prevband]
maxx = 1
maxy = prevband*1.1

fig = plt.figure()
plt.axis([0,maxx,0,maxy])
plt.ion()
plt.show()
plt.ylabel("bandwidth [kbps]")
plt.xlabel("time [s]")
plt.title("Adaptaion")

while True:
    line = logfile.readline()
    if line == "":
        continue
    line = line.strip("\n")
    timestring, seq, band = line.split(";")
    
    time_seconds = calendar.timegm(parser.parse(timestring).utctimetuple())
    time_millis = int(timestring.split(".")[1][:3])
    time = time_seconds*1000+time_millis - basetime
    seq = int(seq)
    band = int(band)/1024.0
    
    maxx = max(maxx, time*1.1)
    maxy = max(maxy, band*1.1)
    
    x.append(time)
    y.append(prevband)
    x.append(time)
    y.append(band)
    prevband = band

    plt.axis([0,maxx,0,maxy])
    plt.plot(x, y, "r")
    plt.draw()
    t.sleep(0.5)