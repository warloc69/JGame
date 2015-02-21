package org.jgame.udp;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Scanner;

/**
 * Created by Bokatov on 20.01.2015.
 */
public class UDPJavaServer
{
    /// loads C++ dynamic library
    static { System.loadLibrary("Integration"); }
    /// sends packets to C++ engine's queue.
    public static native void sendMoveMessageToEngine(byte[] data);
    public static native void sendFireMessageToEngine(byte[] data);
    public static native void sendSpawnMessageToEngine(byte[] data);
    /// reads packet from C++ engine's queue.
    public static native byte[] readMessageFromEngine();

    /// contains information about all connected unity clients and its session keys
    private static final HashMap<String,SessionData> sessionMap = new HashMap();

    /// entry point for server
    public static void main(String[] args) throws IOException
    {
        final int readPort = 6670;
        final int sendPort = 6669;
        final int bufferSize = 0xFF;
        final InetAddress clientIPAddress = InetAddress.getByName("localhost");

        ///// separate thread for read packets from engine
        ///// C++ => Java => C#
        final DatagramSocket client = new DatagramSocket();
        Thread tEngineThread = new Thread(new Runnable() {
            public void run() {
                while(true) {
                    try {
                        byte[] arr = readMessageFromEngine();
                        if (arr.length == 0)
                            continue;
                        Packet pkt = new Packet(arr, true);
                        byte[] data = pkt.convertForClient();
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
                        /// receive packet from unity client
                        server.receive(unityInUdpPacket);

                        /// validate
                        byte[] arr = unityInUdpPacket.getData();
                        if(arr[arr[0] + 3] != '\n')
                        {
                            System.out.println("Broken packet, incorrect EOF");
                            continue;
                        }

                        /// parse
                        Packet pkt = new Packet(arr, false);
                        System.out.println("Incoming packet :: id=" + pkt.id);

                        // handle client packets
                        switch(pkt.id)
                        {
                            case Packets.C_PKT_AUTH_REQUEST:
                            {
                                byte authRequest = pkt.data[40];
                                byte[] login = Arrays.copyOfRange(pkt.data, 0, 20);
                                byte[] password = Arrays.copyOfRange(pkt.data, 21, 40);
                                InetAddress ip = unityInUdpPacket.getAddress();

                                byte[] sessionKey = new byte[32];
                                byte authResponse = handleAuthRequest(authRequest, login, password, ip, sessionKey);

                                Packet unityOutUdpPacket = Packet.generateAuthResponsePacket(authResponse, sessionKey);
                                byte[] data = unityOutUdpPacket.convertForClient();
                                DatagramPacket dp = new DatagramPacket(data, data.length, ip, sendPort);
                                server.send(dp);
                            } break;

                            case Packets.CE_PKT_GO_MOVE:
                            case Packets.CE_PKT_GO_FIRE:
                            case Packets.C_PKT_GO_PARAM_UPDATE:
                            case Packets.C_PKT_PLAYER_ACTION:
                            case Packets.CE_PKT_GO_SPAWN:
                            case Packets.CE_PKT_GO_DESTROY:
                                break;

                            default:
                                System.out.println("Unknown packet or received not from unity :: id="+pkt.id);
                                break;
                        }
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
        client.close();
        server.close();
    }

    private static byte handleAuthRequest(byte authRequest, byte[] login, byte[] password, InetAddress unityClientAddress, byte[] sessionKey) {
        String input = (new String(login)).trim() + (new String(password)).trim();
        String md5 = Utils.md5(input);

        SessionData session = sessionMap.get(new String(login).trim());

        switch(authRequest)
        {
            case 0: {// connection
                if (session == null || !session.encryptedPassword.equals(md5))
                    return AuthorizationResults.AUTH_NOT_REGISTERED;
                if (session.isBanned)
                    return AuthorizationResults.AUTH_BANNED;
                if (session.sessionKey != null && !unityClientAddress.getHostAddress().equals(session.ipAddress))
                    return AuthorizationResults.AUTH_ALREADY_CONNECTED;
                byte[] key = generateSessionKey(login, md5.getBytes());;
                for(int i = 0; i < key.length; i++)
                    sessionKey[i] = key[i];
                session.sessionKey = new String(sessionKey).trim();
            } break;

            case 1: {// registration
                if (session != null)
                    return AuthorizationResults.AUTH_ALREADY_EXIST;
                String loginStr = new String(login).trim();
                SessionData newSession = new SessionData();
                newSession.login = loginStr;
                newSession.encryptedPassword = md5;
                newSession.ipAddress = unityClientAddress.getHostAddress();
                newSession.clientGUID = 5; //TODO
                newSession.isBanned = false;

                sessionMap.put(loginStr, newSession);
            } break;
        }

        return authRequest == 0 ? AuthorizationResults.AUTH_CONNECTION_SUCCESS : AuthorizationResults.AUTH_REGISTRATION_SUCCESS;
    }

    private static byte[] generateSessionKey(byte[] login, byte[] password) {
        String input = (new String(login)).trim() + (new String(password)).trim() + String.valueOf(System.currentTimeMillis());
        String md5 = Utils.md5(input);
        return md5.getBytes();
    }

    private static class SessionData {
        public String login;
        public String encryptedPassword;
        public String ipAddress;
        public String sessionKey;
        public int clientGUID;
        public boolean isBanned;
    }
}
