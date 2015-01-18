package org.jgame.server.core;

import org.apache.commons.lang3.Validate;
import org.jgame.protocol.Message;
import org.jgame.protocol.MessageRules;
import org.jgame.protocol.io.IOHelper;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.Socket;

/**
 * Created by dgroup on 17.01.15.
 */
public class ClientImpl implements Client {
    private static final Logger LOG = LoggerFactory.getLogger(ClientImpl.class);

    private final Socket socket;
    private final BufferedOutputStream toClient;
    private final BufferedInputStream fromClient;

    public ClientImpl(Socket socket) throws IOException {
        Validate.notNull(socket, "Socket can't be a null");
        this.socket = socket;
        toClient    = new BufferedOutputStream(socket.getOutputStream());
        fromClient  = new BufferedInputStream(socket.getInputStream());
    }


    @Override
    public Message receive() throws IOException {
        return IOHelper.readMessage(fromClient);
    }

    @Override
    public void send(Message message) throws IOException {
        MessageRules.notEmpty(message);

        send( message.command() );
        send( message.userId()  );
        send( message.data()    );

        toClient.flush();
        LOG.debug("Message has been sent: {}", message);
    }

    private void send(byte[] message) throws IOException {
        IOHelper.sendMessage(toClient, message);
    }


    @Override
    public void close() throws IOException {
        if (socket != null)
            socket.close();
    }

    @Override
    public String toString() {
        return socket.toString();
    }
}
