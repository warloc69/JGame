package org.jgame.protocol.stub;

import org.jgame.protocol.data.Data;

/**
 * Created by dgroup on 17.01.15.
 */
public class SomeActionData implements Data {
    @Override
    public byte[] toByteArray() {
        return new byte[0];
    }

    @Override
    public void init(byte[] data) {

    }
}
