package org.jgame.protocol;

/**
 * It contains some validation util methods for message
 *
 * Created by dgroup on 17.01.15.
 */
public final class MessageRules {
    private MessageRules() {
    }

    public static void notEmpty(Message message){
        if (message == null)
            throw new IllegalArgumentException("Message can't be a null");

        notEmpty(message.command(), "Command can't be empty");
        notEmpty(message.userId(), "User can't be empty");
        notEmpty(message.data(), "Data can't be empty");
    }

    private static void notEmpty(byte[] command, String errorMessage) {
        if (command.length == 0)
            throw new IllegalArgumentException(errorMessage);
    }
}