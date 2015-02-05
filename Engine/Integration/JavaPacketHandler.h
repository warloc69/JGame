
#pragma once

/**

All java native methods should be presented here.

**/

#include <jni.h>

extern "C" {

JNIEXPORT void JNICALL Java_org_jgame_server_core_EntryPoint_sendMessageToEngine(JNIEnv *, jclass, jbyteArray);
JNIEXPORT jbyteArray JNICALL Java_org_jgame_server_core_EntryPoint_readMessageFromEngine(JNIEnv *, jclass);

}
