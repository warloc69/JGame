
#pragma once

#include "UDPServer.h"

class WorldServer : UDPServer
{
	public:
		WorldServer(short port, asio::io_service& service);

		void handle_packet();
};