
package org.jgame.udp;

public interface Packets {
    short C_PKT_AUTH_REQUEST    = 1;
    short S_PKT_AUTH_RESPONSE   = 2;
    short CE_PKT_GO_MOVE		= 3;
    short CE_PKT_GO_FIRE		= 4;
    short C_PKT_GO_PARAM_UPDATE	= 5;
    short C_PKT_PLAYER_ACTION	= 6;


    short CE_PKT_GO_SPAWN		= 10;
    short CE_PKT_GO_DESTROY		= 11;
}

