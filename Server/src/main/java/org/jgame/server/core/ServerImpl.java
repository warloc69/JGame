package org.jgame.server.core;

import java.io.*;
import java.net.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class ServerImpl implements Server {
    private static final Logger LOG = LoggerFactory.getLogger(ServerImpl.class);

    private ServerSocket server;


    public ServerImpl(int port) throws IOException {
        if (isFreePort( port )) {
            server = new ServerSocket(port);
            LOG.info("Server started: {}", server);
        } else
            throw new IllegalPortRangeException(port);
    }

    private boolean isFreePort(int port){
        return  port >= 48660 && port <= 48999 ||
                port >= 49001 && port <= 49150;
    }



    public Client acceptClient() throws IOException {
        Client client = new ClientImpl(server.accept());
        LOG.debug("Client accepted: {}", client);
        return client;
    }


    @Override
    public int getPort() {
        return server.getLocalPort();
    }

    @Override
    public void close() throws IOException {
        if (server != null) {
            server.close();
        }
    }
}