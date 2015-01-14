package org.jgame.server.core;

import org.apache.commons.lang3.exception.ExceptionUtils;
import org.junit.Test;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.*;
import java.net.Socket;

import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

/**
 * @author dubinka on 14.01.2015.
 */
public class SimpleServerTest {
    private static final Logger LOG = LoggerFactory.getLogger(SimpleServer.class);
    private static final int TWO_SECONDS = 2 * 1000;

    @Test(expected = IllegalPortRangeException.class)
    public void portRange() throws IOException {
        new SimpleServer(1);
    }

    @Test(timeout = TWO_SECONDS )
    public void smoke(){
        int port = 48670;
        try (SimpleServer gameServer = new SimpleServer(port)){
            runClientOn(port);

            // Socket (and it's I/O streams) should be moved to separate class - Client or something like it
            try(Socket s = gameServer.acceptClient();
                DataInputStream in = new DataInputStream(new BufferedInputStream(s.getInputStream()));
                DataOutputStream out= new DataOutputStream(new BufferedOutputStream(s.getOutputStream()))){

                assertTrue("Greeting messages aren't match", "Hi server".equals(in.readUTF()));
                out.writeUTF("Who are you at all?");
                out.flush();
                assertTrue("Messages aren't match", "I'm your client".equals(in.readUTF()));
            }
        } catch (IOException e) {
            fail( ExceptionUtils.getStackTrace(e) );
        }
    }

    private void runClientOn(int port) {
        new Thread(() -> {
            try(Socket s = new Socket("localhost", port);
                DataInputStream in = new DataInputStream(new BufferedInputStream(s.getInputStream()));
                DataOutputStream out = new DataOutputStream(new BufferedOutputStream(s.getOutputStream()))){

                out.writeUTF("Hi server");
                out.flush();

                if ("Who are you at all?".equals(in.readUTF())){
                    out.writeUTF("I'm your client");
                    out.flush();
                }
                LOG.debug("I'm done");

            } catch (IOException e) {
                LOG.error("Communication failed", e);
            }
        }, "Client on " + port).start();
    }
}
