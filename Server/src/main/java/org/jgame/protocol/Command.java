package org.jgame.protocol;

/**
 * It describe all commands.
 * Each command took 1 byte, until byte range enough.
 * After that int range will be used automatically.
 *
 * Created by dgroup on 17.01.15.
 */
public enum Command {
    REGISTRATION(0),
    CONNECTION  (1),
    FIRE        (4),
    ACTION      (6),

    // Another types will be added later

    REGISTRATION_RESPONSE(7),
    UNRECOGNIZED(-128);

    private byte commandId;

    Command(int cmd){
        this((byte) cmd);
    }
    Command(byte cmd){
        this.commandId = cmd;
    }


    public static Command toCommand(int command) {
        if (values().length < command || command < 0)
            return UNRECOGNIZED;
        return detect( command );
    }

    public static Command toCommand(byte[] command) {
        if (command.length == 0)
            return UNRECOGNIZED;
        return detect(command[0]);
    }

    private static Command detect(int command){
        for(Command c : values())
            if (c.commandId == command)
                return c;
        return UNRECOGNIZED;
    }

    public byte[] toByteArray(){
        return new byte[]{ commandId };
    }

}
