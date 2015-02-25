
#include "AuthorizationServer.h"

#include <Wincrypt.h>

#pragma comment(lib, "GameController.lib")
#include "..\GameController\Macro.h"
#include "..\GameController\Packets.h"

/// Генерирует MD5 закодированную строку по входному массиву байтов
std::string md5(std::vector<uint8> bytes)
{
    HCRYPTPROV hCryptProv;
    if(!CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0))
    {
         printf("Acquisition of context failed.\n");
         return NULL;
    }

    HCRYPTHASH hHash;
    if(!CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hHash))
    {
        printf("Error during CryptBeginHash!\n");
        return NULL;
    }

	if(!CryptHashData(hHash, bytes.data(), bytes.size(), 0))
    {
        printf("Error during CryptHashData!\n");
        return NULL;
    }

	DWORD count = 0;
    if(!CryptGetHashParam(hHash, HP_HASHVAL, NULL, &count, 0))
    {
        printf("Error during CryptGetHashParam!\n");
        return NULL;
    }

    uint8* hash_value = static_cast<uint8*>(malloc(count + 1));
    ZeroMemory(hash_value, count + 1);
 
    if(!CryptGetHashParam(hHash, HP_HASHVAL, (BYTE*)hash_value, &count, 0))
    {
        printf("Error during CryptGetHashParam!\n");
        return NULL;
    }

    if(hHash)
       CryptDestroyHash(hHash);

    if(hCryptProv)
       CryptReleaseContext(hCryptProv,0);

	CHAR dig[] = "0123456789abcdef";
	std::string buf;
	for (DWORD i = 0; i < 16; i++)
	{
		buf += dig[hash_value[i] >> 4];
		buf += dig[hash_value[i] & 0xf];
	}
 
	delete [] hash_value;

    return buf;
}

/// Получает текущее время в миллисекундах
uint64 get_current_time()
{
	boost::posix_time::ptime current_time = boost::posix_time::second_clock::local_time();
	uint64 ctime = current_time.time_of_day().total_milliseconds();
	return ctime;
}

/// Конструктор сервера авторизации
AuthorizationServer::AuthorizationServer(short port, asio::io_service& service) : UDPServer(port, service)
{
}

/// Деструктор сервера авторизации
AuthorizationServer::~AuthorizationServer()
{
	SAFE_DELETE_HASH_MAP_V(m_sessions);
}

/// Поиск UDP сессии по логину
UDPSession* AuthorizationServer::findByLogin(std::string login)
{
	std::hash_map<std::string,UDPSession*>::iterator itr = m_sessions.begin();
	while(itr != m_sessions.end())
	{
		if(itr->first == login)
			return itr->second;
		itr++;
	}
	return NULL;
}

/// Асинхронно отправляет пакет адресу пришедшего пакета
/// Если входящий пакет пришел с адреса сервера, отправка осуществляется на порт сервера +1
void AuthorizationServer::send_packet(Packet& p)
{
	/// use next +1 port if server and client are on the same machine
	udp::endpoint ep = end_point.address().to_string() == "127.0.0.1"
		? udp::endpoint(end_point.address(), m_port+1)
		: end_point;

	m_socket.async_send_to(buffer(p.getBuffer(), p.size()), ep,
		bind(&UDPServer::on_send_packet, (UDPServer*)this, placeholders::error, bytes_transferred));
}

/// Обрабатывает получаемые пакеты авторизации
void AuthorizationServer::handle_packet()
{
	/// Парсим шапку массива (размер и номер пакета)
	uint8 data_size = packet[0];
	uint16 packet_id = (packet[2] << 8) | packet[1];

	printf("Incoming authorization packet :: packet_id=%d\n", packet_id);

	/// Обворачиваем данные массива в пакет
	Packet p = Packet(data_size);
	p.writeArray(packet, 3, data_size + 3);

	switch(packet_id)
	{
		case C_PKT_AUTH_REQUEST:
			handleAuthRequestPacket(p);
			break;

		default:
			printf("Unhandled packet id (%d)", packet_id);
			break;
	}
}

/// На входе логин, пароль и байт запроса.
/// 1. Получаем информацию об аккаунте по логину (TODO: нужно будет вытягивать это из базы данных)
/// 2. Генерируем сервер-сайд пароль по логину и паролю от клиента
/// 3. Байт запроса = 0 (подключение)
///    - если аккаунт не существует либо пароль неверный - байт ответа = 4 (не зарегистрированный)
///    - если сессионный ключ сгенерированный, но айпишник подключения другой - байт ответа = 2 (уже подключен)
///    - если сессионный ключ сгенерированный, айпишник одинаковый - байт ответа = 8 (переподключен)
///    - в противном случае генерируем сессионный ключ (логин + пароль + трекущее время в миллисекундах) - байт ответа = 1 (подключен)
/// 4. Байт запроса = 1 (регистрация)
///    - если аккаунт существует - байт ответа = 7 (аккаунт занят)
///    - в противном случае создаем аккаунт - байт ответа = 6 (зарегистрирован)
void AuthorizationServer::handleAuthRequestPacket(Packet& p)
{
	/// parse packet
	std::vector<uint8> login = p.readArray(20);
	std::vector<uint8> passw = p.readArray(32);
	uint8 authRequest = p.readByte();

	/// prepare data
	std::string lg(login.begin(), login.end());
	trim(lg);
	std::string ps(passw.begin(), passw.end());
	trim(ps);
	printf("login=%s,pass=%s\n", lg.c_str(), ps.c_str());

	/// find existing session/account
	UDPSession* session = findByLogin(lg);

	/// generate server-side password
	std::string input = lg + ps;
	std::vector<uint8> data(input.begin(), input.end());
	std::string s_password = md5(data);
	printf("s_password=%s\n", s_password.c_str());

	AuthorizationResult authResponse;
	switch(authRequest)
	{
		/// connection
		case 0:
		{
            if(!session || session->password != s_password)
                authResponse = AuthorizationResult::AUTH_NOT_REGISTERED;
			else if (session->session_key.size() > 0)
			{
				if(end_point.address() != session->unity_end_point.address())
					authResponse = AuthorizationResult::AUTH_ALREADY_CONNECTED;
				else if(end_point != session->unity_end_point)
					authResponse = AuthorizationResult::AUTH_RECONNECT;
			}
			else
			{
				authResponse = AuthorizationResult::AUTH_CONNECTION_SUCCESS;

				/// generate session key
				std::string time_string = std::to_string(get_current_time());
				std::string inputTimed = input + time_string;
				std::vector<uint8> dataTimed(inputTimed.begin(), inputTimed.end());
				std::string key = md5(dataTimed);

				session->session_key = key;
				session->last_action_time = get_current_time();
			}
		} break;

		/// registration
		case 1:
		{
			authResponse = session ? AuthorizationResult::AUTH_ALREADY_EXIST : AuthorizationResult::AUTH_REGISTRATION_SUCCESS;
			if(!session)
			{
				session = new UDPSession();
				session->login = lg;
				session->password = s_password;
				session->unity_end_point = end_point;
				session->last_action_time = get_current_time();

				m_sessions.insert(std::pair<std::string,UDPSession*>(lg, session));
			}
		} break;
	}

	printf("authResponse = %d\n", authResponse);
	
	/// build packet for client
	Packet response;
	if(session && !authRequest) /// connection request with existing account/session
	{
		response = Packet(37);
		response << (uint8)  33;
		response << (uint16) S_PKT_AUTH_RESPONSE;
		response << (uint8)  authResponse;
		response.writeString(session->session_key);
		response.finalize();
	}
	else
	{
		response = Packet(5);
		response << (uint8)  1;
		response << (uint16) S_PKT_AUTH_RESPONSE;
		response << (uint8)  authResponse;
		response.finalize();
	}

	/// send to client
	send_packet(response);
}