package org.jgame.protocol;

import org.apache.commons.lang3.Validate;
import org.jgame.protocol.data.Data;

/**
 * Created by dgroup on 17.01.15.
 */
public class MessageImpl implements Message {
    private Command command;
    private Data data;
    private short userId;

    public MessageImpl(Command comm, int userId, Data data){
        this(comm, (short) userId, data);
    }

    public MessageImpl(Command command, short userId, Data data){
        Validate.notNull(command,   "Command can't be a null");
        Validate.isTrue (userId > 0,"User id can't be less than 0");
        Validate.notNull(data,      "Data can't be a null");

        this.command = command;
        this.userId = userId;
        this.data = data;
    }

    @Override
    public byte[] command() {
        return command.toByteArray();
    }

    @Override
    public byte[] userId() {
        return new byte[] { (byte) ((userId & 0xFF00) >> 8),
                            (byte) (userId & 0x00FF) };
    }

    @Override
    public byte[] data() {
        return data.toByteArray();
    }

    /**
     * Allows to convert data into particular implementation of {@link org.jgame.protocol.data.Data}
     *
     * @param type expected implementation of {@link org.jgame.protocol.data.Data} (or itself)
     * @throws java.lang.ClassCastException when expected type is wrong
     */
    @Override
    public <T> T data(Class<T> type) {
        return type.cast(data);
    }


    @Override
    public String toString() {
        return "MessageImpl{ command=" + command + ", userId=" + userId + '}';
    }
}