package org.jgame.protocol.io;

import org.jgame.protocol.Message;
import org.jgame.protocol.MessageImpl;
import org.jgame.protocol.data.impl.RegistrationData;
import org.junit.Test;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;

import static org.jgame.protocol.Command.FIRE;
import static org.jgame.protocol.Command.toCommand;
import static org.jgame.protocol.Utils.toShort;
import static org.junit.Assert.*;

/**
 * Created by dgroup on 17.01.15.
 */
public class IOHelperTest {
    private static final Logger LOG = LoggerFactory.getLogger(IOHelperTest.class);

    @Test
    public void sendMessagePerOperation() throws IOException {
        Message message = build();
        try(ByteArrayOutputStream out = new ByteArrayOutputStream()){

            IOHelper.sendMessage(out, message.command());
            try(ByteArrayInputStream in = new ByteArrayInputStream( out.toByteArray() )){
                byte[] comm = IOHelper.read( in );
                assertArrayEquals("Commands arrays aren't match", comm, FIRE.toByteArray());
                assertTrue("Transformation doesn't work", FIRE == toCommand(comm));
            }

            out.reset();
            IOHelper.sendMessage(out, message.userId());
            try(ByteArrayInputStream in = new ByteArrayInputStream( out.toByteArray() )){
                byte[] user = IOHelper.read( in );
                assertTrue("User byte array can't be empty", user.length > 0);
                assertArrayEquals("User byte arrays aren't match", message.userId(), user);
                assertTrue("User id aren't match", toShort(user) == toShort(message.userId()));
            }

            out.reset();
            IOHelper.sendMessage(out, message.data());
            try(ByteArrayInputStream in = new ByteArrayInputStream( out.toByteArray() )){
                byte[] data = IOHelper.read( in );
                assertTrue("Data byte array can't be empty", data.length > 0);
                assertArrayEquals("Data's byte arrays aren't match", message.data(), data);
            }
        }
    }

    @Test
    public void sendMessage() throws IOException {
        Message request = build();

        ByteArrayOutputStream out = new ByteArrayOutputStream();
        IOHelper.sendMessage(out, request.command());
        IOHelper.sendMessage(out, request.userId());
        IOHelper.sendMessage(out, request.data());

        ByteArrayInputStream in = new ByteArrayInputStream( out.toByteArray() );
        Message received = IOHelper.readMessage( in );

        assertNotNull("Received message can't be a null", received);
        assertTrue("Commands aren't match", FIRE == toCommand(received.command()));
        assertArrayEquals("User id aren't match", request.userId(), received.userId());
        assertArrayEquals("Data aren't match", request.data(), received.data());
    }

    private Message build(){
        return new MessageImpl(FIRE, 100, new RegistrationData("Tom", "123"));
    }
}
