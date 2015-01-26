
#include "Shared.h"
#include <assert.h>
#include <Windows.h>

void addPacketToEngineQueue(Packet in)
{
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
		data->queue.push(in);	
	} catch(interprocess_exception &ex) {
		MessageBoxA(NULL, ex.what(), "Error!", MB_OK | MB_ICONERROR | MB_DEFAULT_DESKTOP_ONLY);
	}
}
