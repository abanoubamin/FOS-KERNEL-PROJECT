#include <inc/memlayout.h>
#include <kern/kheap.h>
#include <kern/memory_manager.h>

//NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)

struct heap_info
{
	uint32 firstadd;
	uint32 lastadd;
};
struct heap_info info[1024 * 1024];
uint32 indexI = 0;
uint32 next = KERNEL_HEAP_START;

void* kmalloc(unsigned int size)
{
	//TODO: [PROJECT 2018 - MS1 - [1] Kernel Heap] kmalloc()
	// Write your code here, remove the panic and write your code

	//kpanic_into_prompt("kmalloc() is not implemented yet...!!");

	//NOTE: Allocation is based on NEXT FIT strategy
	//NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)
	//refer to the project presentation and documentation for details


	//TODO: [PROJECT 2018 - BONUS1] Implement the BEST FIT strategy for Kernel allocation
	// Beside the NEXT FIT
	// use "isKHeapPlacementStrategyBESTFIT() ..." functions to check the current strategy

	//change this "return" according to your answer

	uint32 newsize = ROUNDUP(size, PAGE_SIZE);
	uint32 newsize2 = newsize;
	uint32 x = 0;
	struct Frame_Info *frameInfo2;
	int xd = 0;
	for (uint32 i = next; i <= KERNEL_HEAP_MAX; i += PAGE_SIZE)
	{
		if (i == KERNEL_HEAP_MAX)
		{
			i = KERNEL_HEAP_START;
		}

		if (i == next)
		{
			xd += 1;
		}

		if (xd == 2)
		{
			if (x != newsize2)
			{
				return NULL;
			}
		}

		uint32 *ptr_p_d;
		struct Frame_Info *frameInfo = get_frame_info(ptr_page_directory, (void *)i, &ptr_p_d);
		if (frameInfo == NULL)
		{
			if (x == 0)
			{
				info[indexI].firstadd = i;
			}

			info[indexI].lastadd = i;
			x += PAGE_SIZE;
			newsize -= PAGE_SIZE;
		}
		else
		{
			x = 0;
		}

		if (x == newsize2)
		{
			break;
		}

		if (i == KERNEL_HEAP_MAX)
		{
			i = KERNEL_HEAP_START;
		}
	}

	if (x == newsize2)
	{
		for (uint32 z = info[indexI].firstadd; z <= info[indexI].lastadd; z += PAGE_SIZE)
		{
			int r2 = allocate_frame(&frameInfo2);
			map_frame(ptr_page_directory, frameInfo2, (void*)z, PERM_PRESENT | PERM_WRITEABLE);
		}
	}

	next = info[indexI].lastadd + PAGE_SIZE;
	int tmp = indexI;
	indexI++;
	return (void *)(info[tmp].firstadd);
}

void kfree(void* virtual_address)
{
	//TODO: [PROJECT 2018 - MS1 - [1] Kernel Heap] kfree()
	// Write your code here, remove the panic and write your code

	//panic("kfree() is not implemented yet...!!");

	//you need to get the size of the given allocation using its address
	//refer to the project presentation and documentation for details

	uint32 ck = 0;
	uint32 startAdd;
	uint32 EndAdd;
	for (int i = 0; i < indexI; i++)
	{
		for (uint32 z = info[i].firstadd; z <= info[i].lastadd; z += PAGE_SIZE)
		{
			if ((uint32)virtual_address == z)
			{
				ck = 1;
				startAdd = (uint32)virtual_address;
				EndAdd = info[i].lastadd;
				break;
			}
		}
	}

	if (ck == 1)
	{
		for (uint32 z = startAdd; z <= EndAdd; z += PAGE_SIZE)
		{
			unmap_frame(ptr_page_directory, (void *)z);
			uint32 *ptr_p_t;
			get_page_table(ptr_page_directory, (void *)z, &ptr_p_t);
			ptr_p_t[PTX(z)] = 0;
		}
	}

}

unsigned int kheap_virtual_address(unsigned int physical_address)
{
	//TODO: [PROJECT 2018 - MS1 - [1] Kernel Heap] kheap_virtual_address()
	// Write your code here, remove the panic and write your code

	//panic("kheap_virtual_address() is not implemented yet...!!");

	//return the virtual address corresponding to given physical_address
	//refer to the project presentation and documentation for details

	//change this "return" according to your answer

	struct Frame_Info *frameInfo;
	frameInfo = to_frame_info(physical_address);
	uint32 frameNum = to_frame_number(frameInfo);
	for (int i = 0; i < indexI; i++)
	{
		for (uint32 z = info[i].firstadd; z <= info[i].lastadd; z += PAGE_SIZE)
		{
			uint32 *ptr_p_t = NULL;
			get_page_table(ptr_page_directory, (void *)z, &ptr_p_t);
			if (ptr_p_t != NULL)
			{
				uint32 pnum = ptr_p_t[PTX(z)] >> 12;
				uint32 p = ptr_p_t[PTX(z)] & PERM_PRESENT;
				if (p != 0 && pnum == frameNum)
				{
					return z;
				}
			}
		}
	}
	return 0;
}

unsigned int kheap_physical_address(unsigned int virtual_address)
{
	//TODO: [PROJECT 2018 - MS1 - [1] Kernel Heap] kheap_physical_address()
	// Write your code here, remove the panic and write your code

	//panic("kheap_physical_address() is not implemented yet...!!");

	//return the physical address corresponding to given virtual_address
	//refer to the project presentation and documentation for details

	//change this "return" according to your answer

	uint32 ourpa = 0;
	uint32 *ptr_p_d;
	struct Frame_Info *frameInfo = get_frame_info(ptr_page_directory, (void *)virtual_address, &ptr_p_d);
	if (frameInfo != NULL)
	{
		ourpa = to_physical_address(frameInfo);
	}

	return ourpa;
}


//=================================================================================//
//============================== BONUS FUNCTION ===================================//
//=================================================================================//
// krealloc():

//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to kmalloc().
//	A call with new_size = zero is equivalent to kfree().

void *krealloc(void *virtual_address, uint32 new_size)
{
	//TODO: [PROJECT 2018 - BONUS2] Kernel Heap Realloc
	// Write your code here, remove the panic and write your code

	return NULL;
	panic("krealloc() is not implemented yet...!!");

}
