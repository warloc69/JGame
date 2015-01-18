package org.jgame.protocol.stub;

import org.jgame.protocol.data.Data;

import java.util.Arrays;

/**
 * Created by dgroup on 17.01.15.
 */
public class SomeActionData implements Data {

    private byte[] data;

    @Override
    public byte[] toByteArray() {
        return Arrays.copyOf(data, data.length);
    }

    @Override
    public void init(byte[] data) {
        this.data = Arrays.copyOf(data, data.length);
    }
}
