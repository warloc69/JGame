package org.jgame.protocol;

/**
 * It describe atomic message, which can be send in scope of one request/response
 *
 * Created by dgroup on 17.01.15.
 */
public interface Message {
    byte[] command();
    byte[] userId();
    byte[] data();

    <T> T data(Class<T> type);
}