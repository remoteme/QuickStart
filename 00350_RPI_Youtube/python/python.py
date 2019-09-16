import logging
import socket

import struct
import sys
import os
import time

os.chdir(sys.argv[1])
sys.path.append('../base')

import remoteme
import Colorer
import remotemeStruct
import subprocess
import threading
import os
import io
import signal
import psutil
from multiprocessing import Process


def get_cpu_temperature():
    f = open("/sys/class/thermal/thermal_zone0/temp", "r")
    return float(f.readline())/1000


def checkTemperature():
    while True:
        temp = get_cpu_temperature()

        remoteMe.getVariables().setDouble("cpu_temp",round(temp,1) )
        time.sleep(1)


def checkingThread():
    global process
    i=0

    while True:
        time.sleep(0.2)
        line = process.stderr.read(200)
        i=i+1
        if line and 'speed=' in line.decode("utf-8"):
            onYoutube(1)
            return
        if i>100:
            onYoutube(-1)
            return

def onYoutube(b):
    global youtubeState
    youtubeState = b
    remoteMe.getVariables().setInteger("$$youtube_state$$", b)
    if b==1:
        logger.info('Youtube ON')
    elif b==0:
        logger.info('Youtube OFF')
    elif b==-1:
        logger.info('Youtube FAIL')
    elif b==2:
        logger.info('Youtube STARTING')
    elif b==3:
        logger.info('Youtube CLOSING')
    else:
        logger.info('Youtube undefined')

def startYoutube():
    global thread1,process
    #thread1.stop()
    onYoutube(2)
    process = subprocess.Popen(['bash', 'youtuberun.sh'], stdout=subprocess.PIPE,stderr=subprocess.PIPE)
    thread1 = threading.Thread(target = checkingThread)
    thread1.start()
    logger.info("process ID {}".format(process.pid))

def endYoutube():
    global process
    for x in range(0, 3):
        subprocess.run(["pkill","-TERM","-P",format(process.pid)])
        logger.info("pkill -TERM -P {}".format(process.pid))
        time.sleep(0.5)

    onYoutube(0)

def youtubeThreadFunction():
    global receiveDeviceConnected
    global youtubeState
    currentOn = False
    wait=0

    while True:
        logger.info('receiveDeviceConnected {} currentOn {} wait {} youtube state : {}'.format(receiveDeviceConnected,currentOn,wait,youtubeState))
        if not currentOn and receiveDeviceConnected:
            currentOn=True
            startYoutube()
            wait=0
        elif currentOn  and not receiveDeviceConnected:
            if wait	> 10:
                wait= 0
                currentOn = False
                endYoutube()
            else:
                wait=wait+1
        elif currentOn  and youtubeState == -1:
            currentOn = False
            endYoutube()
            wait= 0
        time.sleep(1)




def setLed(b):#you call this methind if u want to change variable
    remoteMe.getVariables().setBoolean("led", b)


def onDeviceConnectionChange(deviceId,state):
    global receiveDeviceConnected
    if deviceId == 2:
        if state:
            logger.info("device receive is now ON ")
        else:
            logger.info("device receive is now OFF transmission will end in 10s ")
        receiveDeviceConnected = state


try:
    process =0
    receiveDeviceConnected = False
    youtubeState = 0
    logging.basicConfig(level=logging.DEBUG,
                        format='%(asctime)s %(name)-12s %(levelname)-8s %(message)s',
                        datefmt='%d.%m %H:%M',
                        filename="logs.log")

    logger = logging.getLogger('application')

    remoteMe = remoteme.RemoteMe()

    remoteMe.startRemoteMe(sys.argv)


    remoteMe.subscribeDeviceConnectionChangeEvent(onDeviceConnectionChange)

    onYoutube(0)
    tempThread = threading.Thread(target = checkTemperature)
    tempThread.start()

    youtubeThread= threading.Thread(target = youtubeThreadFunction)
    youtubeThread.start()

    remoteMe.wait()

finally:
    pass
