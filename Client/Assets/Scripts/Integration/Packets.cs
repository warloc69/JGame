using System;

public enum Packets : short
{
	C_PKT_AUTH_REQUEST      = 1,
	S_PKT_AUTH_RESPONSE     = 2,
	CE_PKT_GO_MOVE			= 3,
	CE_PKT_GO_FIRE			= 4,
	C_PKT_GO_PARAM_UPDATE	= 5,
	C_PKT_PLAYER_ACTION		= 6,

	
	
	E_PKT_GO_SPAWN			= 10,
	CE_PKT_GO_DESTROY		= 11,
}
