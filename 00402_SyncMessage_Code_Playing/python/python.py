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

def onUserSyncMessage(senderDeviceId,data):
    reader= remoteMeDataReader.RemoteMeDataReader(data)
    double1Value = reader.readDouble()
    double2Value = reader.readDouble()

    logger.info("get message from device {} of length {}".format(senderDeviceId,len(data)))
    logger.info("double1Value '{}' double2Value {}".format(double1Value,double2Value))

    writer= remoteMeDataWriter.RemoteMeDataWriter()
    writer.writeDouble(double1Value+double2Value)
    return writer.getBytes()

try:

    logging.basicConfig(level=logging.DEBUG,
                        format='%(asctime)s %(name)-12s %(levelname)-8s %(message)s',
                        datefmt='%d.%m %H:%M',
                        filename="logs.log")

    logger = logging.getLogger('application')

    remoteMe = remoteme.RemoteMe()

    remoteMe.startRemoteMe(sys.argv)

    remoteMe.setUserSyncMessageListener(onUserSyncMessage)
    remoteMe.wait()

finally:
    pass