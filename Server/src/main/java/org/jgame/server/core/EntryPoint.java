package org.jgame.server.core;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
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
        final Server server6669 = new Server(6669);
        server6669.start();

        ///// separate thread for read packets from engine
        Thread engineThread = new Thread(new Runnable() {
            public void run() {
                while(true) {
                    try {
                        byte[] arr = readMessageFromEngine();
                        if (arr.length == 0)
                            continue;
                        Packet pkt = new Packet(arr, true);
                        server6669.sendPacket(pkt.clientID, pkt.generateForClient());
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
        engineThread.start();

        ///// java server console
        Scanner sc = new Scanner(System.in);
        while(sc.hasNextLine()) {
            String msg = sc.nextLine();
            if(msg.equals("stop")) {
                server6669.stop();
                break;
            } else if(msg.equals("update")) {
                server6669.update();
            }
        }

        engineThread.interrupt();
    }

    /**
     * Client class. Creates thread for self purpose.
     */
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

    /**
     * Server class. Creates thread for self purpose
     */
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

    /**
     * Packet class - wrapper for byte array
     */
    public static class Packet {
        short id;
        int clientID;
        byte[] data;

        Packet(byte[] arr, boolean enginePkt) {
            ByteBuffer bb = ByteBuffer.wrap(arr).order(ByteOrder.LITTLE_ENDIAN);
            id = bb.getShort();
            if(enginePkt)
                clientID = bb.getInt();
            data = new byte[bb.remaining()];

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
            bb.put((byte)(size-2));
            bb.putShort(id);
            bb.put(data);
            bb.put((byte)'\n');
            return bb.array();
        }
    }
}
