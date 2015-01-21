
#include "JavaPacketHandler.h"
#include "Packets.h"
#include "PacketHandler.h"

#include "Logger.h"

//#include "Macro.h"

JNIEXPORT void JNICALL Java_org_jgame_server_core_EntryPoint_sendMessageToEngine(JNIEnv* pJniEnv, jclass pJClass, jbyteArray data)
{
	// TODO: invoke to C++ server thread

	printf("Java_org_jgame_server_core_EntryPoint_sendMessageToEngine");
	Logger::debug(L"Java_org_jgame_server_core_EntryPoint_sendMessageToEngine", true);

	// convert
    int len = pJniEnv->GetArrayLength(data);
    uint8* buf = new uint8[len];
    pJniEnv->GetByteArrayRegion(data, 0, len, reinterpret_cast<jbyte*>(buf));
	std::vector<uint8> dataVector = ARRAY_TO_VECTOR(buf, len);

	Packet p = Packet(len);
	p << dataVector;
	p.resetRead();

	uint16 opcodeID;
	p >> opcodeID;

	Logger::debug(L"opcodeID="+toWString(opcodeID), true);

	switch(opcodeID)
	{
		case PACKET_MOVE_GAME_OBJECT:
			PacketHandler::handleMoveAndRotatePacket(p);
			break;

		default:
			break;
	}

}

