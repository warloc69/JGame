
package org.jgame.udp;

import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class Utils {
    public static String md5(String input) {
        try {
            MessageDigest md = MessageDigest.getInstance("MD5");
            md.reset();
            md.update(input.getBytes());
            byte[] d = md.digest();
            BigInteger bi = new BigInteger(1, d);
            String hex = bi.toString(16);
            while(hex.length() < 32) {
                hex = "0" + hex;
            }
            return hex;
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        }

        return "";
    }
}
