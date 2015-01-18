package org.jgame.protocol.stub;

import org.apache.commons.lang3.exception.ExceptionUtils;
import org.jgame.protocol.Command;
import org.jgame.protocol.Message;
import org.jgame.protocol.MessageImpl;
import org.jgame.protocol.data.impl.RegistrationData;
import org.jgame.protocol.io.IOHelper;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

import static org.jgame.protocol.Command.ACTION;
import static org.jgame.protocol.Command.toCommand;
import static org.jgame.protocol.io.IOHelper.sendMessage;
import static org.junit.Assert.fail;

/**
 * This is simple client stub just for testing activities
 *
 * Created by dgroup on 17.01.15.
 */
public class ClientStub implements Runnable {
    private static final Logger LOG = LoggerFactory.getLogger(ClientStub.class);
    public static final int USER_ID = 100;

    private final int port;

    public ClientStub(int port){
        this.port = port;
    }

    @Override
    public void run() {
        try(Socket srv = new Socket("localhost", port)){
            InputStream inp = srv.getInputStream();
            OutputStream out = srv.getOutputStream();

            // Registration request
            RegistrationData reqData = new RegistrationData("Tom", "12345");
            Message reqRequest   = new MessageImpl(Command.REGISTRATION, USER_ID, reqData);
            send(out, reqRequest);
            LOG.debug("Registration request has been sent, waiting for response...");

            Message reqResponse = IOHelper.readMessage(inp);
            if (Command.REGISTRATION_RESPONSE != toCommand( reqResponse.command() ))
                stopTesting("Wrong server's response: " + toCommand( reqResponse.command() ));

            LOG.debug("You have successfully registered, let's send some action");
            send(out, new MessageImpl(ACTION, USER_ID, new SomeActionData()));

        } catch (IOException e) {
            fail( ExceptionUtils.getStackTrace(e) );
        }
    }

    private void send(OutputStream out, Message message) throws IOException {
        sendMessage(out, message.command());
        sendMessage(out, message.userId());
        sendMessage(out, message.data());
    }

    private void stopTesting(String message){
        throw new IllegalStateException(message);
    }
}
