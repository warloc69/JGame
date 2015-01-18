
#pragma once

//#include <jni.h>

class MovementPacketHandler 
{
	public:
		static void handle(char* data);

	private:
		static void sendPacket();
};
