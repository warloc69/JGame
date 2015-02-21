
#pragma once

/**

All java native methods should be presented here.

**/

#include <jni.h>

extern "C" {

JNIEXPORT void JNICALL Java_org_jgame_udp_UDPJavaServer_sendMoveMessageToEngine(JNIEnv *, jclass, jbyteArray);
JNIEXPORT void JNICALL Java_org_jgame_udp_UDPJavaServer_sendFireMessageToEngine(JNIEnv *, jclass, jbyteArray);
JNIEXPORT void JNICALL Java_org_jgame_udp_UDPJavaServer_sendSpawnMessageToEngine(JNIEnv *, jclass, jbyteArray);
JNIEXPORT jbyteArray JNICALL Java_org_jgame_udp_UDPJavaServer_readMessageFromEngine(JNIEnv *, jclass);

}
