'''
Created on 2011-12-02

@author: Bobby Wood
'''

## import the serial library
import serial

## Boolean variable that will represent 
## whether or not the arduino is connected
connected = False

## open the serial port that your ardiono 
## is connected to.
ser = serial.Serial("/dev/tty.usbmodemfd121", 9600)

## loop until the arduino tells us it is ready

#while not connected:
#    print "connecting"
#    serin = ser.read()
#    connected = True

print ('connected')

## Tell the arduino to blink!
ser.write('20')

## Wait until the arduino tells us it 
## is finished blinking

#while ser.read() == '1':
#   ser.read()

## close the port and end the program
ser.close()