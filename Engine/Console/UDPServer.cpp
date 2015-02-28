
#include "UDPServer.h"

/// �������������� ������� � Little/Big Endian
template <typename T>
T SWAP_BYTES(T v) 
{
	T retVal;
	char *pVal = (char*)&v;
	char *pRetVal = (char*)&retVal;
	int size = sizeof(T);
	for(int i = 0; i < size; i++)
		pRetVal[size-1-i] = pVal[i];
	return retVal;
}

/// ����������� UDP �������, �� ����� ���� � ���������� IO ������
UDPServer::UDPServer(short port, boost::asio::io_service& m_service) : m_socket(m_service, udp::endpoint(udp::v4(), port)), m_strand(m_service)
{
	union { unsigned int v; unsigned char c[4]; } u;
	u.v = 0x01;
	m_little_endian = 0x01 != u.c[3];

	m_port = port;
	receive_packet();
}

/// ����������� ��������� ������
void UDPServer::receive_packet()
{
	m_socket.async_receive_from(
		buffer(packet), 
		end_point, 
		bind(&UDPServer::on_receive_packet, this, placeholders::error, bytes_transferred)); 
}

/// ����������� �� ��������� ������, ����������� ��������� ������ � ��������� � ����� ����������
void UDPServer::on_receive_packet(const boost::system::error_code& error, size_t n)
{
	m_socket.get_io_service().post(boost::bind(&UDPServer::accept_packet, this, error, n));
	receive_packet();
}

/// ���������, ����������, ������������ �����
void UDPServer::accept_packet(const boost::system::error_code& error, size_t n)
{
	printf("received %d bytes\n", n);

	/// validate packet
	if(packet[0] != (n - 4) || packet[n-1] != (byte)'\n')
	{
		printf("Broken packet, incorrect size or EOF\n");
		return;
	}

	/// convert to little endian
	if(m_little_endian)
		SWAP_BYTES(packet);

	handle_packet();
}

/// ���������� ���������� ����� ������ ���������� ������
/// ���� �������� ����� ������ � ������ �������, �������� �������������� �� ���� ������� +1
void UDPServer::send_packet(Packet& p)
{
	/// use next +1 port if server and client are on the same machine
	udp::endpoint ep = end_point.address().to_string() == "127.0.0.1"
		? udp::endpoint(end_point.address(), m_port+1)
		: end_point;

	m_socket.async_send_to(buffer(p.getBuffer(), p.size()), ep,
		bind(&UDPServer::on_send_packet, (UDPServer*)this, placeholders::error, bytes_transferred));
}

/// ����������� �� �������� ������ �������
void UDPServer::on_send_packet(const boost::system::error_code& error, size_t n)
{
	printf("Packet sent to client (%d bytes)\n", n);
}
