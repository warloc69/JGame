package org.jgame.server.core;

import java.io.Closeable;
import java.io.IOException;

/**
 * Created by dgroup on 17.01.15.
 */
public interface Server extends Closeable {

    public int getPort();
    public Client acceptClient() throws IOException;

}
