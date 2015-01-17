package org.jgame.protocol.data;

/**
 * It's simple data container for each corresponding {@link org.jgame.protocol.Command}
 *
 * Created by dgroup on 17.01.15.
 */
public interface Data {

    byte[] toByteArray();
    void init(byte[] data);

}
