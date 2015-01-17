package org.jgame.protocol.data;

import org.jgame.protocol.Command;
import org.jgame.protocol.data.impl.ByteArrayData;
import org.jgame.protocol.data.impl.RegistrationData;

/**
 * Created by dgroup on 17.01.15.
 */
public class DataBuilder {
    private DataBuilder(){
    }

    public static Data build(Command cmd, byte[] data) {
        switch (cmd){
            case REGISTRATION:
                return new RegistrationData(data);

            default:
                return new ByteArrayData(data);
        }
    }
}
