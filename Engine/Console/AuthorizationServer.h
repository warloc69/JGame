
#pragma once

#include "UDPServer.h"

#include "..\GameController\Packet.h"

#include <hash_map>

enum AuthorizationResult
{
    AUTH_CONNECTION_SUCCESS    = 1,
    AUTH_ALREADY_CONNECTED     = 2,
    AUTH_BANNED                = 3,
    AUTH_NOT_REGISTERED        = 4,
    AUTH_DISCONNECTED          = 5,
    AUTH_REGISTRATION_SUCCESS  = 6,
    AUTH_ALREADY_EXIST         = 7,
	AUTH_RECONNECT			   = 8,
};

struct UDPSession
{
	std::string login;
	std::string password;
	std::string session_key;
	udp::endpoint unity_end_point;
	uint64 last_action_time;
};

class AuthorizationServer : UDPServer
{
	public:
		AuthorizationServer(short port, asio::io_service& service);
		~AuthorizationServer();

		void handle_packet();

	private:
		std::hash_map<std::string,UDPSession*> m_sessions;
		UDPSession* findByLogin(std::string login);

		void handleAuthRequestPacket(Packet& p);
};