
using System;

public enum AuthorizationResults : byte
{
	AUTH_CONNECTION_SUCCESS    = 1,
	AUTH_ALREADY_CONNECTED     = 2,
	AUTH_BANNED                = 3,
	AUTH_NOT_REGISTERED        = 4,
	AUTH_DISCONNECTED          = 5,
	AUTH_REGISTRATION_SUCCESS  = 6,
	AUTH_ALREADY_EXIST         = 7,
}
