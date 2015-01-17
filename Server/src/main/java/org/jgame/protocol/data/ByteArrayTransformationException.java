package org.jgame.protocol.data;

/**
 * Created by dgroup on 17.01.15.
 */
public class ByteArrayTransformationException extends RuntimeException {

    public ByteArrayTransformationException(String message) {
        super(message);
    }

    public ByteArrayTransformationException(String message, Throwable cause) {
        super(message, cause);
    }

    public ByteArrayTransformationException(Throwable cause) {
        super(cause);
    }

    public ByteArrayTransformationException(String message, Throwable cause, boolean enableSuppression, boolean writableStackTrace) {
        super(message, cause, enableSuppression, writableStackTrace);
    }
}
