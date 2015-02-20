package org.jgame.server.core;

import java.io.*;
import java.net.*;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.*;

/**
 * Created by Bokatov on 20.01.2015.
 */
public class EntryPoint
{
    static { System.loadLibrary("Integration"); }

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

    public static void main(String[] args) throws IOException
    {
        final int readPort = 6670;
        final int sendPort = 6669;
        final int bufferSize = 0xFF;

        ///// separate thread for read packets from engine
        ///// C++ => Java => C#
        final DatagramSocket client = new DatagramSocket();
        final InetAddress clientIPAddress = InetAddress.getByName("localhost");
        Thread tEngineThread = new Thread(new Runnable() {
            public void run() {
                while(true) {
                    try {
                        byte[] arr = readMessageFromEngine();
                        if (arr.length == 0)
                            continue;
                        Packet pkt = new Packet(arr, true);
                        byte[] data = pkt.generateForClient();
                        DatagramPacket dp = new DatagramPacket(data, data.length, clientIPAddress, sendPort);
                        client.send(dp);
                    } catch (IOException e) {
                        e.printStackTrace();
                    } finally {
                        try {
                            Thread.sleep(10);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }
                }
            }});
        tEngineThread.start();

        ///// separate thread for read packets from Unity clients
        ///// C# => Java => C++
        final DatagramSocket server = new DatagramSocket(readPort);
        byte[] buffer = new byte[bufferSize];
        DatagramPacket unityInUdpPacket = new DatagramPacket(buffer, buffer.length);
        Thread tUnityThread = new Thread(new Runnable() {
            public void run() {
                while(true) {
                    try {
                        server.receive(unityInUdpPacket);
                        byte[] arr = unityInUdpPacket.getData();
                        // validate 1st data size byte and last '\n' byte
                        if(arr[arr[0] + 3] != '\n')
                        {
                            System.out.println("Broken packet, incorrect EOF");
                            continue;
                        }

                        Packet pkt = new Packet(arr, false);
                        System.out.println("Incoming packet :: id=" + pkt.id);

                        InetAddress unityClientAddress = unityInUdpPacket.getAddress();
                        Packet unityOutUdpPacket = Packet.handle(pkt, unityClientAddress);
                        if(unityOutUdpPacket != null) {
                            byte[] data = unityOutUdpPacket.generateForClient();
                            DatagramPacket dp = new DatagramPacket(data, data.length, unityClientAddress, sendPort);
                            server.send(dp);
                            continue;
                        }

                        int unityClientID = 1; // TODO: find unityClientID by unityClientAddress and sessionKey
                        sendMessageToEngine(pkt.generateForEngine(unityClientID));
                    } catch (IOException e) {
                        e.printStackTrace();
                    } finally {
                        try {
                            Thread.sleep(10);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }
                }
            }});
        tUnityThread.start();

        ///// java server console
        Scanner sc = new Scanner(System.in);
        while(sc.hasNextLine()) {
            String msg = sc.nextLine();
        }

        tEngineThread.interrupt();
        tUnityThread.interrupt();
    }

    /**
     * Client class. Creates thread for self purpose.
     *//*
    private static class Client {
        private int id;
        private Socket socket;
        private InputStream in;
        private OutputStream out;
        private long lastActionTime;
        private final long INACTIVE_TIMEOUT = 1 * 60 * 1000;
        private Thread tThread;

        public Client(int id, Socket socket) {
            this.id = id;
            this.socket = socket;
            final Client pointer = this;
            tThread = new Thread(new Runnable() {
                public void run() { pointer.run(); }});
        }

        public void start() {
            tThread.start();
        }

        public void stop() {
            try {
                socket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        public boolean isStopped() {
            return socket == null || socket.isClosed();
        }

        private void run() {
            try {
                in = socket.getInputStream();
                out = socket.getOutputStream();

                System.out.println("Connected client :: ip = " + socket.getInetAddress() + ", id = " + id);
                lastActionTime = System.currentTimeMillis();

                // reading packets from client
                while(!socket.isClosed())
                {
                    long diff = System.currentTimeMillis() - lastActionTime;
                    if(diff > INACTIVE_TIMEOUT)
                        break;

                    int len = in.read();
                    if(len == -1)
                        break;
                    if(len <= 0)
                        continue;

                    byte[] arr = new byte[len];
                    in.read(arr);
                    if(in.read() != '\n')
                        throw new IOException("Broken packet, incorrect EOF");

                    lastActionTime = System.currentTimeMillis();

                    Packet pkt = new Packet(arr, false);
                    System.out.println("Incoming packet :: id=" + pkt.id);

                    switch(pkt.id)
                    {
                        case 1:
                        case 2:
                            Packet p = Packet.generateSessionKeyPacket(pkt.data);
                            sendPacket(p.generateForClient());
                            continue;
                    }

                    // TODO: define which packets should be sent to engine
                    sendMessageToEngine(pkt.generateForEngine(id));
                }
            } catch (SocketException e) {
                System.out.println(e.getMessage());
            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                try {
                    if(in != null) in.close();
                    if(out != null) out.close();
                    System.out.println("Client stopped :: id = " + id);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

        public void sendPacket(byte[] arr) throws IOException {
            out.write(arr);
            out.flush();
            System.out.println("Sent packet to client id=" + id);
        }
    }
*/
    /**
     * Server class. Creates thread for self purpose
     *//*
    private static class Server {
        private int port;
        private ServerSocket serverSocket;
        private HashMap<Integer,Client> clientMap;
        private Thread tThread;

        public Server(int port) {
            this.port = port;
            this.clientMap = new HashMap();

            final Server pointer = this;
            tThread = new Thread(new Runnable() {
                public void run() { pointer.run(); }});
        }

        public int getPort() {
            return port;
        }

        public void start() {
            tThread.start();
        }

        private void run() {
            try {
                serverSocket = new ServerSocket(port);

                while(!serverSocket.isClosed()) {
                    Socket clientSocket = serverSocket.accept();
                    int id = generateClientGUID();

                    Client client = new Client(id, clientSocket);
                    client.start();

                    clientMap.put(id, client);
                }
            } catch (SocketException e) {
                System.out.println(e.getMessage());
            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                stop();
            }
        }

        public void stopClients() {
            clientMap.values().forEach(EntryPoint.Client::stop);
        }

        public void stop() {
            stopClients();
            try {
                serverSocket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        private int generateClientGUID() {
            int i = 1;

            while(i < clientMap.size() + 1) {
                if(clientMap.get(i) == null) {
                    return i;
                }
                i++;
            }
            return i;
        }

        public void sendPacket(int clientID, byte[] arr) throws IOException {
            if(clientID == 0) {
                for(Client client : clientMap.values())
                    client.sendPacket(arr);
                return;
            }

            Client client = clientMap.get(clientID);
            if(client == null)
                return;
            client.sendPacket(arr);
        }

        public void update() {
            Set<Map.Entry<Integer,Client>> map = clientMap.entrySet();
            Iterator<Map.Entry<Integer,Client>> itr = map.iterator();
            while(itr.hasNext()) {
                Map.Entry<Integer,Client> e = itr.next();
                Client client = e.getValue();
                if(client == null || client.isStopped()) {
                    itr.remove();
                    System.out.println("Removed client from map :: id = " + e.getKey());
                    continue;
                }
            }
        }
    }
*/

    public interface Packets {
        short C_PKT_REGISTER_USER   = 1;
        short C_PKT_AUTHORIZE_USER	= 2;
        short CE_PKT_GO_MOVE		= 3;
        short CE_PKT_GO_FIRE		= 4;
        short C_PKT_GO_PARAM_UPDATE	= 5;
        short C_PKT_PLAYER_ACTION	= 6;
        short S_PKT_GEN_SESSION_KEY = 7;


        short E_PKT_GO_SPAWN		= 10;
        short CE_PKT_GO_DESTROY		= 11;
    }

    /**
     * Packet class - wrapper for byte array
     */
    public static class Packet {
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

        public byte[] generateForEngine(int clientID)
        {
            int size = Short.BYTES + Integer.BYTES + data.length;
            ByteBuffer bb = ByteBuffer.allocate(size).order(ByteOrder.LITTLE_ENDIAN);
            bb.putShort(id);
            bb.putInt(clientID);
            bb.put(data);
            return bb.array();
        }

        public byte[] generateForClient()
        {
            int size = 1 + Short.BYTES + data.length + 1;
            ByteBuffer bb = ByteBuffer.allocate(size).order(ByteOrder.LITTLE_ENDIAN);
            bb.put((byte)(data.length));
            bb.putShort(id);
            bb.put(data);
            bb.put((byte)'\n');
            return bb.array();
        }

        public static Packet handle(Packet in, InetAddress unityClientAddress)
        {
            switch(in.id)
            {
                // java handler
                case Packets.C_PKT_REGISTER_USER:
                case Packets.C_PKT_AUTHORIZE_USER:
                    return generateSessionKeyPacket(in.data, unityClientAddress);

                // engine handler
                case Packets.CE_PKT_GO_MOVE:
                case Packets.CE_PKT_GO_FIRE:
                case Packets.C_PKT_GO_PARAM_UPDATE:
                case Packets.C_PKT_PLAYER_ACTION:
                case Packets.E_PKT_GO_SPAWN:
                case Packets.CE_PKT_GO_DESTROY:
                    break;

                // unity handler
                case Packets.S_PKT_GEN_SESSION_KEY:
                    break;

                default:
                    System.out.println("Unknown packet :: id="+in.id);
                    break;
            }

            return null;
        }

        private static Packet generateSessionKeyPacket(byte[] data, InetAddress unityClientAddress)
        {
            System.out.println(unityClientAddress.toString());

            ByteBuffer bb = ByteBuffer.wrap(data).order(ByteOrder.LITTLE_ENDIAN);
            byte[] part1 = new byte[10]; // first 10 bytes of login
            byte[] part2 = new byte[10]; // last 10 bytes of login
            byte[] part3 = new byte[10]; // last 10 bytes of login
            byte[] part4 = new byte[10]; // last 10 bytes of login
            bb.get(part1);
            bb.get(part2);
            bb.get(part3);
            bb.get(part4);

            bb = ByteBuffer.allocate(24).order(ByteOrder.LITTLE_ENDIAN);
            bb.put((byte)20);
            bb.putShort((short)7);
            bb.put(part3);
            bb.put(part1);
            bb.put((byte)'\n');
            return new Packet(bb.array(), false);
        }
    }
}
