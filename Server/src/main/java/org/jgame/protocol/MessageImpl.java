package org.jgame.protocol;

import org.apache.commons.lang3.Validate;
import org.jgame.protocol.data.Data;

/**
 * Created by dgroup on 17.01.15.
 */
public class MessageImpl implements Message {
    private Command cmd;
    private Data dataBlock;
    private short user;

    public MessageImpl(Command cmd, int userId, Data data){
        this(cmd, (short) userId, data);
    }

    public MessageImpl(Command cmd, short userId, Data data){
        Validate.notNull(cmd,       "Command can't be a null");
        Validate.isTrue (userId > 0,"User id can't be less than 0");
        Validate.notNull(data,      "Data can't be a null");

        this.cmd = cmd;
        this.user = userId;
        this.dataBlock = data;
    }

    @Override
    public byte[] command() {
        return cmd.toByteArray();
    }

    @Override
    public byte[] userId() {
        return new byte[] { (byte)((user & 0xFF00) >> 8),
                            (byte)( user & 0x00FF) };
    }

    @Override
    public byte[] data() {
        return dataBlock.toByteArray();
    }

    /**
     * Allows to convert data into particular implementation of {@link org.jgame.protocol.data.Data}
     *
     * @param type expected implementation of {@link org.jgame.protocol.data.Data} (or itself)
     * @throws java.lang.ClassCastException when expected type is wrong
     */
    @Override
    public <T> T data(Class<T> type) {
        return type.cast(dataBlock);
    }


    @Override
    public String toString() {
        return "MessageImpl{ command=" + cmd + ", userId=" + user + '}';
    }
}