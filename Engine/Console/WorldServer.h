
#pragma once

#include "UDPServer.h"

class WorldServer : UDPServer
{
	public:
		WorldServer(short port, boost::asio::io_service& service);

		void handle_packet();
};