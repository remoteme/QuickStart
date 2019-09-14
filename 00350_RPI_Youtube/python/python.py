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

def endYoutube():
    global process
    subprocess.run(["pkill","-TERM","-P",format(process.pid)])
    #kill(process.pid)
    onYoutube(0)

def youtubeThreadFunction():
    global receiveDeviceConnected

    currentOn = False
    wait=0

    while True:
        logger.info('receiveDeviceConnected {} currentOn {} wait {}'.format(receiveDeviceConnected,currentOn,wait))
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
        time.sleep(1)




def setLed(b):#you call this methind if u want to change variable
    remoteMe.getVariables().setBoolean("led", b)


def onDeviceConnectionChange(deviceId,state):
    global receiveDeviceConnected
    if deviceId == 2:
        if state:
            logger.info("device receive is now ON ")
        else:
            logger.info("device receive is now OFF transmission will end in 20s ")
        receiveDeviceConnected = state

def onUserSyncMessage(senderDeviceId,data):
    reader= remoteMeDataReader.RemoteMeDataReader(data)
    # your reading here
    writer= remoteMeDataWriter.RemoteMeDataWriter()
    # your writting here

    return writer.getBytes()

def onUserMessage(senderDeviceId,data):
    reader= remoteMeDataReader.RemoteMeDataReader(data)
# your readings here

try:
    process =0
    receiveDeviceConnected = False

    logging.basicConfig(level=logging.DEBUG,
                        format='%(asctime)s %(name)-12s %(levelname)-8s %(message)s',
                        datefmt='%d.%m %H:%M',
                        filename="logs.log")

    logger = logging.getLogger('application')

    remoteMe = remoteme.RemoteMe()

    remoteMe.startRemoteMe(sys.argv)


    remoteMe.setUserSyncMessageListener(onUserSyncMessage)
    remoteMe.setUserMessageListener(onUserMessage)
    remoteMe.subscribeDeviceConnectionChangeEvent(onDeviceConnectionChange)

    tempThread = threading.Thread(target = checkTemperature)
    tempThread.start()

    youtubeThread= threading.Thread(target = youtubeThreadFunction)
    youtubeThread.start()

    remoteMe.wait()

finally:
    thread1.stop()
    pass
face