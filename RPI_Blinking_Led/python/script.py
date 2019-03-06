import logging
import socket

import struct
import sys
import os

os.chdir(sys.argv[1])
sys.path.append('../base')

import remoteme

import RPi.GPIO as GPIO
LED_PIN=18


def setLed(b):
    remoteMe.getVariables().setBoolean("$$led_variable$$", b)


def onLedChange(b):
    GPIO.output(LED_PIN, GPIO.HIGH if b else GPIO.LOW)

    pass


def setupPins():
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(LED_PIN, GPIO.OUT)
    GPIO.output(LED_PIN, GPIO.LOW)
    pass

try:

    logging.basicConfig(level=logging.DEBUG,
    format='%(asctime)s %(name)-12s %(levelname)-8s %(message)s',
    datefmt='%d.%m %H:%M',
    filename="logs.log")

    logger = logging.getLogger('application')
    setupPins()
    remoteMe = remoteme.RemoteMe()

    remoteMe.startRemoteMe(sys.argv)
    remoteMe.getVariables().observeBoolean("$$led_variable$$" ,onLedChange);

    remoteMe.wait()

finally:
    pass