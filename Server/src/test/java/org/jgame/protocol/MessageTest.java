package org.jgame.protocol;

import org.jgame.protocol.data.impl.ByteArrayData;
import org.jgame.protocol.data.impl.RegistrationData;
import org.junit.Test;

import static org.jgame.protocol.Command.FIRE;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

/**
 * Created by dgroup on 17.01.15.
 */
public class MessageTest {

    @Test
    public void dataConvertation(){
        Message msg = new MessageImpl(FIRE, 1, new RegistrationData("Tom", "12345"));

        RegistrationData data = msg.data(RegistrationData.class);
        assertNotNull("Data can't be a null", data);
        assertEquals("User's aren't match", data.getUser(), "Tom");
    }

    @Test(expected = ClassCastException.class)
    public void wrongDataConvertation(){
        new MessageImpl(FIRE, 1, new RegistrationData("Tom", "123"))
            .data(ByteArrayData.class);
    }
}
