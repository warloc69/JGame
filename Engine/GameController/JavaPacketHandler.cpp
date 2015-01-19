
#include "JavaPacketHandler.h"
#include "Logger.h"

#include "PacketHandler.h"
#include "Packets.h"

JNIEXPORT void JNICALL Java_org_jgame_server_core_SimpleServer_sendPacketToGameServer(JNIEnv* pJniEnv, jclass pJClass, jbyteArray data)
{
	// TODO: invoke to C++ server thread

	printf("Java_org_jgame_server_core_SimpleServer_sendPacketToGameServer");
	Logger::debug(L"Java_org_jgame_server_core_SimpleServer_sendPacketToGameServer", true);

	// convert
    int len = pJniEnv->GetArrayLength(data);
    uint8* buf = new uint8[len];
    pJniEnv->GetByteArrayRegion(data, 0, len, reinterpret_cast<jbyte*>(buf));

	Logger::debug(L"len="+toWString(len), true);

	Packet p = Packet(len);
	p << buf;
	p.resetRead();

	Logger::debug(L"asd", true);

	uint16 opcodeID;
	p >> opcodeID;

	Logger::debug(L"opcodeID="+opcodeID, true);

	switch(opcodeID)
	{
		case PACKET_MOVE_GAME_OBJECT:
			PacketHandler::handleMoveAndRotatePacket(p);
			break;

		default:
			break;
	}

}

