import os
import subprocess as sb
import serial

port = serial.Serial(
            port='COM5',
            baudrate=9600,
            parity=serial.PARITY_ODD,
            stopbits=serial.STOPBITS_TWO,
            bytesize=serial.SEVENBITS)

f = open("data", "w")
bandwidths = range(50, 500, 50)+range(500, 1000, 100)+range(1000, 10001, 1000)
command = "iperf.exe "


f.write("band\tclients\tthroughput (kbps)\n")
f.flush()


for bandwidth in bandwidths:
    port.write("mls qos aggregate-policer aggpolicer " + str(bandwidth*1000) + " 8000 exceed-action drop\n")
    
    best = 0
    best_client = 0
    for client in range(1, 5):
        process = sb.Popen([command, "-c12.0.0.2 ", "-t10" , "-P" + str(client)], stdout=sb.PIPE)
        out, err = process.communicate()
        summary = out.split("\n")[-2]
        summary = summary.split(" ")[-2]
        if best < float(summary):
            best = float(summary)
            best_client = client
        f.write(str(bandwidth) + "\t" + str(client) + "\t" + str(summary) + "\n")
        f.flush()
    print str(bandwidth) + "\t" + str(best_client) + "\t" + str(best)