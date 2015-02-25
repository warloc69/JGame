
#pragma once

#include "UDPServer.h"

#include "..\GameController\Packet.h"

class WorldServer : UDPServer
{
	public:
		WorldServer(short port, asio::io_service& service);

		void handle_packet();
		void send_packet(Packet& p);
};