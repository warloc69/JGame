package org.jgame.server.core;



import org.jgame.protocol.Message;

import java.io.Closeable;
import java.io.IOException;

/**
 * Created by dgroup on 17.01.15.
 */
public interface Client extends Closeable {

    void send(Message message) throws IOException;
    Message receive() throws IOException;
}
