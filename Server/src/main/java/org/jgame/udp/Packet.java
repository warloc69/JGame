package org.jgame.udp;

import java.net.InetAddress;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Arrays;

/**
 * Packet class - wrapper for byte array
 */
public class Packet {
    short id;
    int clientID;
    byte[] data;

    Packet(byte[] arr, boolean enginePkt) {
        ByteBuffer bb = ByteBuffer.wrap(arr).order(ByteOrder.LITTLE_ENDIAN);
        int size = bb.get();//read size byte
        id = bb.getShort();
        if(enginePkt)
            clientID = bb.getInt();
        data = new byte[size];

        int i = 0;
        while (i < data.length) {
            data[i] = bb.get();
            i++;
        }
    }

    public byte[] convertForEngine(int clientID)
    {
        int size = Short.BYTES + Integer.BYTES + data.length;
        ByteBuffer bb = ByteBuffer.allocate(size).order(ByteOrder.LITTLE_ENDIAN);
        bb.putShort(id);
        bb.putInt(clientID);
        bb.put(data);
        return bb.array();
    }

    public byte[] convertForClient()
    {
        int size = 1 + Short.BYTES + data.length + 1;
        ByteBuffer bb = ByteBuffer.allocate(size).order(ByteOrder.LITTLE_ENDIAN);
        bb.put((byte) (data.length));
        bb.putShort(id);
        bb.put(data);
        bb.put((byte)'\n');
        return bb.array();
    }

    public static Packet generateAuthResponsePacket(byte authResult, byte[] sessionKey)
    {
        ByteBuffer bb = ByteBuffer.allocate(sessionKey.length+5).order(ByteOrder.LITTLE_ENDIAN);
        bb.put((byte)(1+sessionKey.length));
        bb.putShort(Packets.S_PKT_AUTH_RESPONSE);
        bb.put(authResult);
        bb.put(sessionKey);
        bb.put((byte)'\n');
        return new Packet(bb.array(), false);
    }
}
