
#pragma once

#include <jni.h>

extern "C" {

JNIEXPORT void JNICALL Java_org_jgame_server_core_SimpleServer_sendPacketToGameServer(JNIEnv *, jclass, jbyteArray);

}
