package org.jgame.server.core;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.channels.FileChannel;

enum Packets
{
    PACKET_MOVE_GAME_OBJECT     ((short)0x0003, 30),
    PACKET_SPAWN_GAME_OBJECT    ((short)0x0010, 28);

    public short id;
    public int size;
    private Packets(short id, int size)
    {
        this.id = id;
        this.size = size;
    }
}

/**
 * Created by Bokatov on 20.01.2015.
 */
public class EntryPoint
{
    static
    {
        System.loadLibrary("Integration");
    }

    private static final String PIPE_FILE_NAME = "\\\\.\\pipe\\jserver_pipe";

    public static void main(String[] args) throws IOException
    {
        System.out.println("Test #1 - Java=>C++ - Start");

        byte[] bb = Packet.generateMovementPacket(3, new Vector3D(15.05f, -1.03f, 10.07f), new Vector3D(1.05f, 1f, 0.5f));
        sendMessageToEngine(bb);

        System.out.println("Test #1 - Java=>C++ - End");

        /////////////////////////////////////////////

        System.out.println("Test #2 - C++=>Java - Start");

        while(true)
        {
            byte[] received = readMessageFromEngine();
            if(received.length == 0)
                continue;
            Packet pkt = new Packet(received);
            System.out.println("Received pkt = " + pkt.toString());
        }

        //System.out.println("Test #2 - C++=>Java - End");
    }

    /**
     * Sends packet to C++ engine's queue.
     * @param data
     */
    public static native void sendMessageToEngine(byte[] data);

    /**
     * Reads packet from C++ engine's queue.
     * @return
     */
    public static native byte[] readMessageFromEngine();

    /**
     * For test
     */
    static class Vector3D
    {
        float x, y, z;
        Vector3D(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }
    }

    /**
     * For testing purpose, should be generated in Unity (?)
     */
    static class Packet
    {
        short opcodeID;
        int objectID;
        byte[] data;

        Packet(byte[] packet)
        {
            ByteBuffer bb = ByteBuffer.wrap(packet).order(ByteOrder.LITTLE_ENDIAN);
            opcodeID = bb.getShort();
            objectID = bb.getInt();
            data = new byte[bb.remaining()];

            int i = 0;
            while(i < data.length)
            {
                data[i] = bb.get();
                i++;
            }
        }

        // total size: 2+4+4+4+4+4+4+4=30
        // p >> opcodeID >> objectID >> posX >> posY >> posZ >> rotX >> rotY >> rotZ;
        //          2          4          4       4       4       4       4       4
        static byte[] generateMovementPacket(int objID, Vector3D pos, Vector3D rot)
        {
            ByteBuffer bb = ByteBuffer.allocate(Packets.PACKET_MOVE_GAME_OBJECT.size);
            bb.putShort(Packets.PACKET_MOVE_GAME_OBJECT.id);
            bb.putInt(objID);
            bb.putFloat(pos.x);
            bb.putFloat(pos.y);
            bb.putFloat(pos.z);
            bb.putFloat(rot.x);
            bb.putFloat(rot.y);
            bb.putFloat(rot.z);
            return bb.array();
        }

        public String toString()
        {
            return "[id="+opcodeID+",obj="+objectID+",data_length="+data.length+"]";
        }
    }
}
