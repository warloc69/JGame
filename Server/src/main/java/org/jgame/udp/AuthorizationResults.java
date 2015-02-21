package org.jgame.udp;

public interface AuthorizationResults {
    byte AUTH_CONNECTION_SUCCESS    = 1;
    byte AUTH_ALREADY_CONNECTED     = 2;
    byte AUTH_BANNED                = 3;
    byte AUTH_NOT_REGISTERED        = 4;
    byte AUTH_DISCONNECTED          = 5;
    byte AUTH_REGISTRATION_SUCCESS  = 6;
    byte AUTH_ALREADY_EXIST         = 7;
}
