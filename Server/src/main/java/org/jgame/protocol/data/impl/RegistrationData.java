package org.jgame.protocol.data.impl;

import org.apache.commons.io.IOUtils;
import org.apache.commons.lang3.Validate;
import org.jgame.protocol.data.ByteArrayTransformationException;
import org.jgame.protocol.data.Data;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;

/**
 * Created by dgroup on 17.01.15.
 */
public class RegistrationData implements Data {
    private String user;
    private String pass;

    public RegistrationData(String user, String pass){
        setUser(user);
        setPass(pass);
    }

    public RegistrationData(byte [] data){
        init(data);
    }


    @Override
    public byte[] toByteArray() {
        try(ByteArrayOutputStream out = new ByteArrayOutputStream()) {

            out.write( user.length()    );
            out.write( user.getBytes()  );
            out.write( pass.length()    );
            out.write( pass.getBytes()  );

            return out.toByteArray();

        } catch (IOException e) {
            throw new ByteArrayTransformationException(e);
        }
    }

    /**
     * TODO deep tests required for this method
     * */
    @Override
    public void init(byte[] data) {
        try(ByteArrayInputStream inp = new ByteArrayInputStream(data)) {

            int lenght = inp.read();
            byte[] login = new byte[lenght];
            IOUtils.read(inp, login);
            setUser(new String(login));

            lenght = inp.read();
            byte[] pass = new byte[lenght];
            IOUtils.read(inp, pass);
            setPass(new String(pass));

        } catch (IOException e) {
            throw new ByteArrayTransformationException(e);
        }
    }

    public String getUser() {
        return user;
    }
    public void setUser(String user) {
        Validate.notBlank(user, "Username can't be empty");
        if (user.length() > 10)
            throw new IllegalArgumentException("Too long user name, maximum is 10 symbols");
        this.user = user;
    }

    public String getPass() {
        return pass;
    }
    public void setPass(String pass) {
        Validate.notBlank(pass, "Password can't be empty");
        if (pass.length() > 5)
            throw new IllegalArgumentException("Too long password, maximum is 5 symbols");
        this.pass = pass;
    }


}
