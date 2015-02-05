
#pragma once

#include "baseTypes.h"
#include "Packet.h"
#include "Queue.h"

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
using namespace boost::interprocess;


#define SHARED_MEMORY_FILE ".shared_memory"

#define MAX_IN_MOVEMENT_QUEUE		1000
#define MAX_IN_COLLISION_QUEUE		1000
#define MAX_OUT_QUEUE				1000

enum QueueTypes
{
	IN_QUEUE_MOVEMENT = 1,
	IN_QUEUE_COLLISION,
	OUT_QUEUE,
};

struct shared_memory_buffer
{
	Queue<Packet,MAX_IN_MOVEMENT_QUEUE> inMovementQueue;
	Queue<Packet,MAX_IN_COLLISION_QUEUE> inCollisionQueue;
	Queue<Packet,MAX_OUT_QUEUE> outQueue;
};

void addPacketToEngineQueue(Packet p, QueueTypes queueType);
bool readPacketFromEngineQueue(Packet& p, QueueTypes queueType);
