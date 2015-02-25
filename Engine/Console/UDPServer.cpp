
#include "UDPServer.h"

/// Преобразование массива в Little/Big Endian
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

/// Конструктор UDP сервера, на входе порт и уникальный IO сервис
UDPServer::UDPServer(short port, asio::io_service& m_service) : m_socket(m_service, udp::endpoint(udp::v4(), port)), m_strand(m_service)
{
	union { unsigned int v; unsigned char c[4]; } u;
	u.v = 0x01;
	m_little_endian = 0x01 != u.c[3];

	m_port = port;
	receive_packet();
}

/// Асинхронное получение пакета
void UDPServer::receive_packet()
{
	m_socket.async_receive_from(
		buffer(packet), 
		end_point, 
		bind(&UDPServer::on_receive_packet, this, placeholders::error, bytes_transferred)); 
}

/// Срабатывает по получению пакета, откладывает обработку пакета и переходит к приёму следующего
void UDPServer::on_receive_packet(const system::error_code& error, size_t n)
{
	m_socket.get_io_service().post(bind(&UDPServer::accept_packet, this, error, n));
	receive_packet();
}

/// Принимает, валидирует, конвертирует пакет
void UDPServer::accept_packet(const system::error_code& error, size_t n)
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

/// Срабатывает по отправке пакета клиенту
void UDPServer::on_send_packet(const system::error_code& error, size_t n)
{
	printf("Packet sent to client (%d bytes)\n", n);
}
