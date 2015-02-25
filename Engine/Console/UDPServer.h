
#pragma once

#include <winsock2.h>

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>

using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost::asio::placeholders;
using namespace boost::algorithm;
using system::error_code;

class UDPServer
{
	public:
		void on_send_packet(const system::error_code& error, size_t n);

	protected:
		UDPServer(short port, asio::io_service& service);
		virtual void handle_packet() = 0;

		bool m_little_endian;
		byte packet[0xFF];
		udp::endpoint end_point;

		void receive_packet();
		void on_receive_packet(const system::error_code& error, size_t n);
		void accept_packet(const system::error_code& error, size_t n);

		strand m_strand;
		udp::socket m_socket;
		short m_port;
};
