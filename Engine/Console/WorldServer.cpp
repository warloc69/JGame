
#include "WorldServer.h"

#pragma comment(lib, "GameController.lib")
#include "..\GameController\PacketHandler.h"
#include "..\GameController\Macro.h"

/// ����������� ������� ����
WorldServer::WorldServer(short port, boost::asio::io_service& service) : UDPServer(port, service)
{
}

/// ������������ ���������� ������
void WorldServer::handle_packet()
{
	/// ������ ����� ������� (������ � ����� ������)
	uint8 data_size = packet[0];
	uint16 packet_id = (packet[2] << 8) | packet[1];

	printf("Incoming world packet :: packet_id=%d\n", packet_id);

	/// ������������ ������ ������� � �����
	Packet p = Packet(data_size);
	p.writeArray(packet, 3, data_size + 3);

	/// ���������� ����� �� ���������
	PacketHandler::handle(p);
}
