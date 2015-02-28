
#include "WorldServer.h"

#pragma comment(lib, "GameController.lib")
#include "..\GameController\PacketHandler.h"
#include "..\GameController\Macro.h"

/// Конструктор сервера мира
WorldServer::WorldServer(short port, boost::asio::io_service& service) : UDPServer(port, service)
{
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
