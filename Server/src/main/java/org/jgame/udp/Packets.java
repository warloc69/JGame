
package org.jgame.udp;

public interface Packets {
    short C_PKT_AUTH_REQUEST	= 0x1001;
    short C_PKT_DISCONNECT	    = 0x1002;
    short C_PKT_PLAYER_ACTION	= 0x1003;
    short C_PKT_GO_SPAWN	    = 0x1004;
    short C_PKT_GO_DESTROY	    = 0x1005;
    short C_PKT_GO_MOVE	        = 0x1006;
    short C_PKT_GO_FIRE	        = 0x1007;
    short C_PKT_GO_PARAM_UPDATE	= 0x1008;

    short E_PKT_GO_SPAWN	    = 0x2004;
    short E_PKT_GO_DESTROY	    = 0x2005;
    short E_PKT_GO_MOVE	        = 0x2006;
    short E_PKT_GO_FIRE	        = 0x2007;
    short E_PKT_GO_PARAM_UPDATE	= 0x2008;

    short S_PKT_AUTH_RESPONSE	= 0x4000;
}

