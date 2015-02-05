
#include "Shared.h"

void addPacketToEngineQueue(Packet in, QueueTypes queueType)
{
	printf("addPacketToEngineQueue :: start\n");

	try {
		// open shared memory file
		shared_memory_object shm(open_only, SHARED_MEMORY_FILE, read_write);
		// map region
		mapped_region region(shm, read_write);
		// get memory region address
		void* addr = region.get_address();
		// cast data from region to memory buffer
		shared_memory_buffer* data = static_cast<shared_memory_buffer*>(addr);
		// queue is already threadsafe
		switch(queueType)
		{
			case IN_QUEUE_MOVEMENT:
				data->inMovementQueue.push(in);
				break;
			case IN_QUEUE_COLLISION:
				data->inCollisionQueue.push(in);
				break;
			case OUT_QUEUE:
				data->outQueue.push(in);
				break;
		}
	} catch(interprocess_exception &ex) {
		printf("interprocess_exception = %s\n", ex.what());
	}

	printf("addPacketToEngineQueue :: end\n");
}

bool readPacketFromEngineQueue(Packet& p, QueueTypes queueType)
{
	//printf("readPacketFromEngineQueue :: start\n");

	try {
		// open shared memory file
		shared_memory_object shm(open_only, SHARED_MEMORY_FILE, read_write);
		// map region
		mapped_region region(shm, read_write);
		// get memory region address
		void* addr = region.get_address();
		// cast data from region to memory buffer
		shared_memory_buffer* data = static_cast<shared_memory_buffer*>(addr);
		// queue is already threadsafe
		switch(queueType)
		{
			case IN_QUEUE_MOVEMENT:
				return data->inMovementQueue.pop(p);
			case IN_QUEUE_COLLISION:
				return data->inCollisionQueue.pop(p);
			case OUT_QUEUE:
				return data->outQueue.pop(p);
		}
	} catch(interprocess_exception &ex) {
		printf("interprocess_exception = %s\n", ex.what());
	}

	//printf("readPacketFromEngineQueue :: false end\n");
	return false;
}
