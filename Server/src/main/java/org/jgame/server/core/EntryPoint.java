package org.jgame.server.core;

import org.jgame.protocol.Message;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.nio.ByteBuffer;

/**
 * Created by Bokatov on 20.01.2015.
 */
public class EntryPoint
{
    static {
        System.loadLibrary("Integration");
    }

    public static void main(String[] args) throws IOException
    {
        Server gameServer = new ServerImpl(49001);
        Client c = gameServer.acceptClient();
        System.out.println("Accepted client");

        // total size: 2+4+4+4+4+4+4+4=30
        // p >> opcodeID >> objectID >> posX >> posY >> posZ >> rotX >> rotY >> rotZ;
        //          2          4          4       4       4       4       4       4
        short opcodeID = 3;
        int objectID = 3;
        float posX = 15.05f;
        float posY = -1.03f;
        float posZ = 10.07f;
        float rotX = 1.05f;
        float rotY = 1.00f;
        float rotZ = 0.50f;


        ByteBuffer bb = ByteBuffer.allocate(30);
        bb.putShort(opcodeID);
        bb.putInt(objectID);
        bb.putFloat(posX);
        bb.putFloat(posY);
        bb.putFloat(posZ);
        bb.putFloat(rotX);
        bb.putFloat(rotY);
        bb.putFloat(rotZ);

        //while(System.in.read() != -1) {
            sendMessageToEngine(bb.array());
        //}

    }

    public static native void sendMessageToEngine(byte[] data);
}
