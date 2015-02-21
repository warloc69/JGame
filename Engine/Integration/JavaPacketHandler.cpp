
#include "JavaPacketHandler.h"
#include "Shared.h"

// converts pointer to array to vector
template <typename T>
std::vector<T> ARRAY_TO_VECTOR(T* v, size_t N)
{
	std::vector<T> result;
	for(size_t i = 0; i < N; i++)
		result.push_back(v[i]);
	return result;
};

JNIEXPORT void JNICALL Java_org_jgame_udp_UDPJavaServer_sendMoveMessageToEngine(JNIEnv* pJniEnv, jclass pJClass, jbyteArray arr)
{
	//printf("Java_org_jgame_udp_UDPJavaServer_sendMoveMessageToEngine :: start\n");

	// convert incoming java array to byte vector
    int len = pJniEnv->GetArrayLength(arr);
    uint8* buf = new uint8[len];
    pJniEnv->GetByteArrayRegion(arr, 0, len, reinterpret_cast<jbyte*>(buf));
	std::vector<uint8> dataVector = ARRAY_TO_VECTOR(buf, len);
	delete [] buf;

	// make a packet
	Packet p;
	p << dataVector;

	// send packet to queue
	addPacketToEngineQueue(p, QueueTypes::IN_QUEUE_MOVEMENT);

	//printf("Java_org_jgame_udp_UDPJavaServer_sendMoveMessageToEngine :: end\n");
}

JNIEXPORT void JNICALL Java_org_jgame_udp_UDPJavaServer_sendFireMessageToEngine(JNIEnv* pJniEnv, jclass pJClass, jbyteArray arr)
{
	//printf("Java_org_jgame_udp_UDPJavaServer_sendFireMessageToEngine :: start\n");

	// convert incoming java array to byte vector
    int len = pJniEnv->GetArrayLength(arr);
    uint8* buf = new uint8[len];
    pJniEnv->GetByteArrayRegion(arr, 0, len, reinterpret_cast<jbyte*>(buf));
	std::vector<uint8> dataVector = ARRAY_TO_VECTOR(buf, len);
	delete [] buf;

	// make a packet
	Packet p;
	p << dataVector;

	// send packet to queue
	addPacketToEngineQueue(p, QueueTypes::IN_QUEUE_COLLISION);

	//printf("Java_org_jgame_udp_UDPJavaServer_sendFireMessageToEngine :: end\n");
}

JNIEXPORT void JNICALL Java_org_jgame_udp_UDPJavaServer_sendSpawnMessageToEngine(JNIEnv* pJniEnv, jclass pJClass, jbyteArray arr)
{
	//printf("Java_org_jgame_udp_UDPJavaServer_sendSpawnMessageToEngine :: start\n");

	// convert incoming java array to byte vector
    int len = pJniEnv->GetArrayLength(arr);
    uint8* buf = new uint8[len];
    pJniEnv->GetByteArrayRegion(arr, 0, len, reinterpret_cast<jbyte*>(buf));
	std::vector<uint8> dataVector = ARRAY_TO_VECTOR(buf, len);
	delete [] buf;

	// make a packet
	Packet p;
	p << dataVector;

	// send packet to queue
	addPacketToEngineQueue(p, QueueTypes::IN_QUEUE_SPAWN);

	//printf("Java_org_jgame_udp_UDPJavaServer_sendSpawnMessageToEngine :: end\n");
}

JNIEXPORT jbyteArray JNICALL Java_org_jgame_udp_UDPJavaServer_readMessageFromEngine(JNIEnv* pJniEnv, jclass pJClass)
{
	//printf("Java_org_jgame_udp_UDPJavaServer_readMessageFromEngine :: start\n");

	// read packet
	Packet pkt;
	if(!readPacketFromEngineQueue(pkt, QueueTypes::OUT_QUEUE))
		return pJniEnv->NewByteArray(0);

	// convert packet to java byte array
	int len = pkt.size();
	jbyteArray arr = pJniEnv->NewByteArray(len);
	uint8* buf = pkt.getBuffer();
	pJniEnv->SetByteArrayRegion(arr, 0, len, reinterpret_cast<jbyte*>(buf));

	//printf("Java_org_jgame_udp_UDPJavaServer_readMessageFromEngine :: end\n");
	return arr;
}
