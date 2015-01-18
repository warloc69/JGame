package org.jgame.protocol.io;

import org.apache.commons.io.IOUtils;
import org.jgame.protocol.Command;
import org.jgame.protocol.Message;
import org.jgame.protocol.MessageImpl;
import org.jgame.protocol.Utils;
import org.jgame.protocol.data.Data;
import org.jgame.protocol.data.DataBuilder;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 * Allows to perform atomic read/write byte operation.
 *
 * Created by dgroup on 17.01.15.
 */
public final class IOHelper {
    private static final Logger LOG = LoggerFactory.getLogger(IOHelper.class);

    private IOHelper(){
    }

    public static Message readMessage(InputStream inp) throws IOException {
        Command cmd = readCommand(inp);
        short user  = readUser(inp);
        Data data   = readData(cmd, inp);
        return new MessageImpl(cmd, user, data);
    }


    static Data readData(Command cmd, InputStream inp) throws IOException {
        Data received = DataBuilder.build(cmd, read(inp));
        LOG.debug("received: {}", received);
        return received;
    }


    static short readUser(InputStream inp) throws IOException {
        return Utils.toShort(read(inp));
    }


    static Command readCommand(InputStream inp) throws IOException {
        Command comm = Command.toCommand(read(inp));
        LOG.debug("received: {}", comm);
        return comm;
    }


    static byte[] read(InputStream inp) throws IOException {
        int len = inp.read();
        if (len > 0) {
            byte[] data = new byte[len];
            IOUtils.read(inp, data);
            return data;
        } else
            return new byte[0];
    }



    public static void sendMessage(OutputStream out, byte[] message) throws IOException {
        out.write(message.length);
        if (message.length > 0) {
            IOUtils.write(message, out);
        }
    }
}
