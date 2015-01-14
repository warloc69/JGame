package org.jgame.server.core;

import java.io.*;
import java.net.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class SimpleServer implements Closeable {
    private static final Logger LOG = LoggerFactory.getLogger(SimpleServer.class);

    private int port;
    private ServerSocket server;


    public SimpleServer(int port) throws IOException {
        setPort(port);
        init();
    }


    public Socket acceptClient() throws IOException {
        Socket s = server.accept();
        LOG.debug("Client accepted as {}", s);
        return s;
    }



    public int getPort() {
        return port;
    }
    public void setPort(int port) {
        if (isFreePort(port)) {
            this.port = port;
        } else {
            throw new IllegalPortRangeException(port);
        }
    }
    private boolean isFreePort(int port){
        return (port >= 48654 && port <= 48999) || (port >= 49001 && port <= 49150);
    }


    private void init() throws IOException {
        server = new ServerSocket(getPort());
    }


    // Obsolete hello-world method, should be removed
    public static void main(String h[]) {
        try {
            ServerSocket ss = new ServerSocket(6669);
            Socket s = ss.accept();
            System.out.println("Client Accepted");
            BufferedReader br = new BufferedReader(new InputStreamReader(s.getInputStream()));
            System.out.println(br.readLine());
            PrintWriter wr = new PrintWriter(new OutputStreamWriter(s.getOutputStream()), true);
            wr.println("O Da Detka");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void close() throws IOException {
        if (server != null) {
            server.close();
        }
    }
}