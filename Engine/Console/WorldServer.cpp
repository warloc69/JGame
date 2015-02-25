
#include "WorldServer.h"

#pragma comment(lib, "GameController.lib")
#include "..\GameController\PacketHandler.h"
#include "..\GameController\Macro.h"

/// Конструктор сервера мира
WorldServer::WorldServer(short port, asio::io_service& service) : UDPServer(port, service)
{
}

/// Асинхронно отправляет пакет адресу пришедшего пакета
/// Если входящий пакет пришел с адреса сервера, отправка осуществляется на порт сервера +1
void WorldServer::send_packet(Packet& p)
{
	/// use next +1 port if server and client are on the same machine
	udp::endpoint ep = end_point.address().to_string() == "127.0.0.1"
		? udp::endpoint(end_point.address(), m_port+1)
		: end_point;

	m_socket.async_send_to(buffer(p.getBuffer(), p.size()), ep,
		bind(&UDPServer::on_send_packet, (UDPServer*)this, placeholders::error, bytes_transferred));
}

/// Обрабатывает получаемые пакеты
void WorldServer::handle_packet()
{
	/// Парсим шапку массива (размер и номер пакета)
	uint8 data_size = packet[0];
	uint16 packet_id = (packet[2] << 8) | packet[1];

	printf("Incoming world packet :: packet_id=%d\n", packet_id);

	/// Обворачиваем данные массива в пакет
	Packet p = Packet(data_size);
	p.writeArray(packet, 3, data_size + 3);

	/// Отправляем пакет на обработку
	PacketHandler::handle(p);
}
