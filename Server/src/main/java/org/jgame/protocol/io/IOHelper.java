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
 * Created by dgroup on 17.01.15.
 */
public class IOHelper {
    private static final Logger LOG = LoggerFactory.getLogger(IOHelper.class);

    private IOHelper(){
    }

    public static Message readMessage(InputStream in) throws IOException {
        Command cmd = readCommand(in);
        short user  = readUser(in);
        Data data   = readData(cmd, in);
        return new MessageImpl(cmd, user, data);
    }

    static Data readData(Command cmd, InputStream in) throws IOException {
        Data received = DataBuilder.build(cmd, read(in));
        LOG.debug("received: {}", received);
        return received;
    }

    static short readUser(InputStream in) throws IOException {
        return Utils.toShort(read(in));
    }

    static Command readCommand(InputStream in) throws IOException {
        Command comm = Command.toCommand( read(in) );
        LOG.debug("received: {}", comm);
        return comm;
    }

    static byte[] read(InputStream in) throws IOException {
        int len = in.read();
        if (len > 0) {
            byte[] data = new byte[len];
            IOUtils.read(in, data);
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
