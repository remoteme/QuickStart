import logging
import socket

import struct
import sys
import os

os.chdir(sys.argv[1])
sys.path.append('../base')

import remoteme
import remoteMeDataReader
import remoteMeDataWriter

def onUserMessage(senderDeviceId,data):
    reader= remoteMeDataReader.RemoteMeDataReader(data)
    stringValue = reader.readString()
    doubleValue = reader.readDouble()
    logger.info("get message from device {} of length {}".format(senderDeviceId,len(data)))
    logger.info("stringValue '{}' doubleValue {}".format(stringValue,doubleValue))
    writer= remoteMeDataWriter.RemoteMeDataWriter()
    writer.writeString("Received: "+stringValue)
    writer.writeDouble(-2*doubleValue)
    remoteMe.sendUserMessage(senderDeviceId,writer.getBytes())

try:

    logging.basicConfig(level=logging.DEBUG,
                        format='%(asctime)s %(name)-12s %(levelname)-8s %(message)s',
                        datefmt='%d.%m %H:%M',
                        filename="logs.log")

    logger = logging.getLogger('application')

    remoteMe = remoteme.RemoteMe()

    remoteMe.startRemoteMe(sys.argv)

    remoteMe.setUserMessageListener(onUserMessage)
    remoteMe.wait()

finally:
    pass