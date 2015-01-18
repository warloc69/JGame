package org.jgame.protocol;

import org.jgame.protocol.data.impl.RegistrationData;
import org.jgame.protocol.stub.ClientStub;
import org.jgame.server.core.Client;
import org.jgame.server.core.IllegalPortRangeException;
import org.jgame.server.core.Server;
import org.jgame.server.core.ServerImpl;
import org.junit.Test;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;

import static org.jgame.protocol.Command.*;
import static org.jgame.protocol.Utils.toShort;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;

/**
 * @author dubinka on 14.01.2015.
 */
public class ServerImplTest {
    private static final Logger LOG = LoggerFactory.getLogger(ServerImplTest.class);
    private static final int TWO_SECONDS = 2 * 1000;

    @Test(expected = IllegalPortRangeException.class)
    public void portRange() throws IOException {
        new ServerImpl(1);
    }

    @Test(timeout = TWO_SECONDS )
    public void smoke() throws IOException {
        int port = 48670;
        try (Server gameServer = new ServerImpl( port )){
            assertEquals("Port's aren't match", port, gameServer.getPort());

            runClientOn(port);

            try(Client client = gameServer.acceptClient()){

                // Registration request
                Message regRequest = client.receive();
                RegistrationData data = regRequest.data(RegistrationData.class);

                // Registration process
                if (ClientStub.USER_ID == toShort(regRequest.userId())) {
                    LOG.debug("User registered: {}", data.getUser());
                    client.send(new MessageImpl(REGISTRATION_RESPONSE, ClientStub.USER_ID, data));

                    // Some action under registered user
                    Message action = client.receive();
                    assertEquals("Message type is wrong", ACTION, toCommand(action.command()));

                } else
                    fail("User with id " + toShort(regRequest.userId()) + " not found");
            }
        }
    }


    private void runClientOn(int port) {
        new Thread( new ClientStub(port), "Client on "+port )
            .start();
    }
}
