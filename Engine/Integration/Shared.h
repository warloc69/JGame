
#pragma once

#include "baseTypes.h"
#include "Packet.h"
#include "Queue.h"

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
using namespace boost::interprocess;

#define MAX_QUEUE 5
#define SHARED_MEMORY_FILE ".shared_memory"

DLL_EXPORT void addPacketToEngineQueue(Packet p);

struct shared_memory_buffer
{
	Queue<Packet,MAX_QUEUE> queue;
};

