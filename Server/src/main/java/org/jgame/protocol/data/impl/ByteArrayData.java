package org.jgame.protocol.data.impl;

import org.jgame.protocol.data.Data;

import java.util.Arrays;

/**
 * Created by dgroup on 17.01.15.
 */
public class ByteArrayData implements Data {

    private byte[] data;

    public ByteArrayData(byte[] data) {
        init(data);
    }

    @Override
    public byte[] toByteArray() {
        return data;
    }

    @Override
    public void init(byte[] data) {
        this.data = data;
    }

    @Override
    public String toString() {
        return "ByteArrayData{data=" + Arrays.toString(data) + '}';
    }
}