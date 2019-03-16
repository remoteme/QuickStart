import logging
import socket
import math
import struct
import sys
import os

os.chdir(sys.argv[1])
sys.path.append('../base')

import remoteme

import Adafruit_PCA9685
import time
import RPi.GPIO as GPIO

motorAIn1 = 25  # GPIO25
motorAIn2 = 8  # GPIO8

motorBIn1 = 24  # 24
motorBIn2 = 23  # 23

motors = [[motorAIn1, motorAIn2], [motorBIn1, motorBIn2]]

motorsPWM = [14, 15]
pwm = None;

def motorForward(motorId):
    GPIO.output(motors[motorId][0], GPIO.LOW)
    GPIO.output(motors[motorId][1], GPIO.HIGH)


def motorBackward(motorId):
    GPIO.output(motors[motorId][0], GPIO.HIGH)
    GPIO.output(motors[motorId][1], GPIO.LOW)


def motorSoftStop(motorId):
    GPIO.output(motors[motorId][0], GPIO.LOW)
    GPIO.output(motors[motorId][1], GPIO.LOW)
	
def setMotor(motorId, speed):
    if speed == 0:
        motorSoftStop(motorId)
    elif speed > 0:
        motorForward(motorId)
    elif speed < 0:
        motorBackward(motorId)
        speed=-speed

    logger.info("set speed {} for motor {} ".format(speed,motorId))
    pwm.set_pwm(motorsPWM[motorId], 0, int(speed))


def setCameraPos(i1, i2):
    remoteMe.getVariables().setSmallInteger2("cameraPos", i1, i2)



def onCameraPosChange(i1, i2):
    global pwm
    logger.info("on camera change {} , {}".format(i1, i2))
    pwm.set_pwm(1, 0, i1)
    pwm.set_pwm(0, 0, i2)
    pass

def onDriveChange(x, y):
    logger.info("on drive change  x {} , y {}".format(x, y))
    global pwm
    
    left=y;
    right=y;
    
    left+=x;
    right-=x;
    
    delta=(left+right)/2;
    
    
    left+=delta;
    right+=delta;
    
	# when your car doesnt drive as suppose try to swich right and left variable below
	# or remove add minuses next to 2
	# another way is to switch cables conencted to motors
    setMotor(0, 2*left)
    setMotor(1, -2*right)
    pass


def setupPWM():
    global pwm
    pwm = Adafruit_PCA9685.PCA9685()
    pwm.set_pwm_freq(80)

def setupPins():
    global GPIO
    GPIO.setmode(GPIO.BCM)  # Broadcom pin-numbering scheme

    for motor in motors:
        for pinId in motor:
            GPIO.setup(pinId, GPIO.OUT)
    
try:

    logging.basicConfig(level=logging.DEBUG,
    format='%(asctime)s %(name)-12s %(levelname)-8s %(message)s',
    datefmt='%d.%m %H:%M',
    filename="logs.log")

    logger = logging.getLogger('application')
    setupPWM()
    setupPins()
    remoteMe = remoteme.RemoteMe()

    remoteMe.startRemoteMe(sys.argv)
    remoteMe.getVariables().observeSmallInteger2("$$camera_variable$$" ,onCameraPosChange);
    remoteMe.getVariables().observeSmallInteger2("$$drive_variable$$" ,onDriveChange);
    remoteMe.wait()

finally:
    pass
