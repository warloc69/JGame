package org.jgame.server.core;

/**
 * @author dgroup on 14.01.2015.
 */
public class IllegalPortRangeException extends RuntimeException {

    public IllegalPortRangeException() {
    }

    public IllegalPortRangeException(int port){
        super(port + "");
    }

    public IllegalPortRangeException(String message) {
        super(message);
    }

    public IllegalPortRangeException(String message, Throwable cause) {
        super(message, cause);
    }

    public IllegalPortRangeException(Throwable cause) {
        super(cause);
    }

    public IllegalPortRangeException(String message, Throwable cause, boolean enableSuppression, boolean writableStackTrace) {
        super(message, cause, enableSuppression, writableStackTrace);
    }
}
