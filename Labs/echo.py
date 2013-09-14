#
# This program sends lines from STDIN to the given serial port.
# 
import serial
import sys

if len(sys.argv) <= 1:
    print "Usage: <serial port>"
    print "You can get a list with this command:"
    print "  python -m serial.tools.list_ports"
    sys.exit(1)

port = sys.argv[1]
ser = serial.Serial(port, 9600)

while True:

    for line in iter(sys.stdin.readline, ""):
        print 'SENDING',line
        ser.write(line)

ser.close()
