import logging
import socket
import threading
import time
import struct
import sys
import os

os.chdir(sys.argv[1])
sys.path.append('../base')

import remoteme
import remoteMeDataReader
import remoteMeDataWriter


counter = 0

def setJust3IntegerVariable():
    while True:
        time.sleep(1)
        millis = int(round(time.time() * 1000))
        logger.info("setting just3IntegerVariable")
        remoteMe.getVariables().setSmallInteger3("just3IntegerVariable", millis%32000,counter*-10,counter)


def onSomeIntegerChanged(number):
    global counter
    counter=number
    remoteMe.getVariables().setBoolean("justBooleanVariable",  counter%2==1)


try:

    logging.basicConfig(level=logging.DEBUG,
                        format='%(asctime)s %(name)-12s %(levelname)-8s %(message)s',
                        datefmt='%d.%m %H:%M',
                        filename="logs.log")

    logger = logging.getLogger('application')

    remoteMe = remoteme.RemoteMe()

    remoteMe.startRemoteMe(sys.argv)

    remoteMe.getVariables().observeInteger("someIntegerVariable" ,onSomeIntegerChanged)

    sampleThread = threading.Thread(target=setJust3IntegerVariable)
    sampleThread.daemon = True
    sampleThread.start()

    remoteMe.wait()

finally:
    pass
