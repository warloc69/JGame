package org.jgame.protocol;


import org.jgame.protocol.stub.SomeActionData;
import org.junit.Test;

import static org.jgame.protocol.Command.*;
import static org.junit.Assert.assertTrue;

/**
* Created by dgroup on 17.01.15.
*/
public class CommandTest {

    @Test
    public void intConversation(){
        assertTrue("Registration can't be converted", REGISTRATION == toCommand(0));
        assertTrue("Connection can't be converted", CONNECTION == toCommand(1));
        assertTrue("Fire can't be converted", FIRE == toCommand(4));
    }

    @Test
    public void byteConversation(){
        assertTrue("Registration can't be converted", REGISTRATION == toCommand(new byte[]{0}));
        assertTrue("Connection can't be converted", CONNECTION == toCommand(new byte[]{1}));
        assertTrue("Fire can't be converted", FIRE == toCommand(new byte[]{4}));

    }

    @Test
    public void wrongCommand(){
        assertTrue("Greater index not recognized", UNRECOGNIZED == toCommand(8));
        assertTrue("Negative index not recognized", UNRECOGNIZED == toCommand(-1));
    }

    @Test
    public void conversationFromMessage(){
        Message mes = new MessageImpl(FIRE, 100, new SomeActionData());
        byte[] command  = mes.command();
        assertTrue("Command array can't be empty", command.length != 0);
        assertTrue("Command aren't match", FIRE == toCommand(command));
    }
}
