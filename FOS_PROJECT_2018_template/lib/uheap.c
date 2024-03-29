#include <inc/lib.h>

// malloc()
//	This function use NEXT FIT strategy to allocate space in heap
//  with the given size and return void pointer to the start of the allocated space

//	To do this, we need to switch to the kernel, allocate the required space
//	in Page File then switch back to the user again.
//
//	We can use sys_allocateMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls allocateMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the allocateMem function is empty, make sure to implement it.

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

struct heap_info
{
	uint32 stAddr;
	uint32 lastAddr;
};
struct heap_info info[970000];
uint32 ind = 0;
uint32 next = 0;
uint32 count = (USER_HEAP_MAX - USER_HEAP_START) / PAGE_SIZE;
uint32 arr[131072];
uint32 sizee = (USER_HEAP_MAX - USER_HEAP_START) / PAGE_SIZE;
void* malloc(uint32 size)
{
	//TODO: [PROJECT 2018 - MS2 - [4] User Heap] malloc() [User Side]
	// Write your code here, remove the panic and write your code

	//panic("malloc() is not implemented yet...!!");

	// Steps:
	//	1) Implement NEXT FIT strategy to search the heap for suitable space
	//		to the required allocation size (space should be on 4 KB BOUNDARY)
	//	2) if no suitable space found, return NULL
	//	 Else,
	//	3) Call sys_allocateMem to invoke the Kernel for allocation
	// 	4) Return pointer containing the virtual address of allocated space,
	//

	//This function should find the space of the required range
	// ******** ON 4KB BOUNDARY ******************* //

	//Use sys_isUHeapPlacementStrategyNEXTFIT()
	//to check the current strategy

	uint32 in1 = 0;
	uint32 in2 = 0;
	uint32 newsize = ROUNDUP(size, PAGE_SIZE);
	uint32 newsize2 = newsize;
	if ((newsize2 / PAGE_SIZE) > sizee)
		return NULL;
	uint32 x = 0;
	int xd = 0;
	for (uint32 i = next; i <= count; i++)
	{
		if (i == count)
		{
			i = 0;
			x = 0;
		}

		if (i == next)
		{
			xd += 1;
		}

		if (xd == 2)
		{
			return NULL;
		}

		if (arr[i] == 0)
		{
			if (x == 0)
			{
				info[ind].stAddr = USER_HEAP_START + (i * PAGE_SIZE);
				in1 = i;
			}

			info[ind].lastAddr = USER_HEAP_START + (i * PAGE_SIZE);
			in2 = i;
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
	}

	int ret = sys_isUHeapPlacementStrategyNEXTFIT();
	if (ret != 1)
	{
		panic("Next Fit not working correctly");
	}

	if (x == newsize2)
	{
		sys_allocateMem(info[ind].stAddr, in2 - in1);
		sizee -= (in2 - in1);
		for (uint32 i = in1; i <= in2; i++)
			arr[i] = 1;
	}

	next = in2 + 1;
	int tmp = ind;
	ind++;

	return (void *)(info[tmp].stAddr);
}
// free():
//	This function frees the allocation of the given virtual_address
//	To do this, we need to switch to the kernel, free the pages AND "EMPTY" PAGE TABLES
//	from page file and main memory then switch back to the user again.
//
//	We can use sys_freeMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls freeMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the freeMem function is empty, make sure to implement it.

void free(void* virtual_address)
{
	//TODO: [PROJECT 2018 - MS2 - [4] User Heap] free() [User Side]
	// Write your code here, remove the panic and write your code

	//panic("free() is not implemented yet...!!");

	//cprintf("vir:%x\n",virtual_address);
	//you shold get the size of the given allocation using its address
	//you need to call sys_freeMem()
	//refer to the project presentation and documentation for details

	uint32 ck = 0;
	uint32 StartAddr;
	uint32 EndAddr;
	for (int i = 0; i < ind; i++)
	{
		if ((uint32)virtual_address == info[i].stAddr)
		{
			uint32 start = ((uint32)virtual_address - USER_HEAP_START) / PAGE_SIZE;
			uint32 end = (info[i].lastAddr - USER_HEAP_START) / PAGE_SIZE;
			for (uint32 j = start; j <= end; j++)
			{
				arr[j] = 0;
				sizee++;
			}
			sys_freeMem((uint32)virtual_address, info[i].lastAddr);
			info[i].stAddr = 0;
			info[i].lastAddr = 0;
			break;
		}
	}

}


//==================================================================================//
//============================== BONUS FUNCTIONS ===================================//
//==================================================================================//

//===============
// [2] realloc():
//===============

//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to malloc().
//	A call with new_size = zero is equivalent to free().

//  Hint: you may need to use the sys_moveMem(uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
//		which switches to the kernel mode, calls moveMem(struct Env* e, uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
//		in "memory_manager.c", then switch back to the user mode here
//	the moveMem function is empty, make sure to implement it.

void *realloc(void *virtual_address, uint32 new_size)
{
	//TODO: [PROJECT 2018 - BONUS3] User Heap Realloc [User Side]
	// Write your code here, remove the panic and write your code
	panic("realloc() is not implemented yet...!!");

	return NULL;
}



//==================================================================================//
//============================= OTHER FUNCTIONS ====================================//
//==================================================================================//

void expand(uint32 newSize)
{
}

void shrink(uint32 newSize)
{
}

void freeHeap(void* virtual_address)
{
	return;
}


//=============
// [1] sfree():
//=============
//	This function frees the shared variable at the given virtual_address
//	To do this, we need to switch to the kernel, free the pages AND "EMPTY" PAGE TABLES
//	from main memory then switch back to the user again.
//
//	use sys_freeSharedObject(...); which switches to the kernel mode,
//	calls freeSharedObject(...) in "shared_memory_manager.c", then switch back to the user mode here
//	the freeSharedObject() function is empty, make sure to implement it.

void sfree(void* virtual_address)
{
	//[] Free Shared Variable [User Side]
	// Write your code here, remove the panic and write your code
	//panic("sfree() is not implemented yet...!!");

	//	1) you should find the ID of the shared variable at the given address
	//	2) you need to call sys_freeSharedObject()

}

void* smalloc(char *sharedVarName, uint32 size, uint8 isWritable)
{
	//[[6] Shared Variables: Creation] smalloc() [User Side]
	// Write your code here, remove the panic and write your code
	panic("smalloc() is not implemented yet...!!");

	// Steps:
	//	1) Implement FIRST FIT strategy to search the heap for suitable space
	//		to the required allocation size (space should be on 4 KB BOUNDARY)
	//	2) if no suitable space found, return NULL
	//	 Else,
	//	3) Call sys_createSharedObject(...) to invoke the Kernel for allocation of shared variable
	//		sys_createSharedObject(): if succeed, it returns the ID of the created variable. Else, it returns -ve
	//	4) If the Kernel successfully creates the shared variable, return its virtual address
	//	   Else, return NULL

	//This function should find the space of the required range
	// ******** ON 4KB BOUNDARY ******************* //

	//Use sys_isUHeapPlacementStrategyFIRSTFIT() to check the current strategy

	return 0;
}

void* sget(int32 ownerEnvID, char *sharedVarName)
{
	//[[6] Shared Variables: Get] sget() [User Side]
	// Write your code here, remove the panic and write your code
	panic("sget() is not implemented yet...!!");

	// Steps:
	//	1) Get the size of the shared variable (use sys_getSizeOfSharedObject())
	//	2) If not exists, return NULL
	//	3) Implement FIRST FIT strategy to search the heap for suitable space
	//		to share the variable (should be on 4 KB BOUNDARY)
	//	4) if no suitable space found, return NULL
	//	 Else,
	//	5) Call sys_getSharedObject(...) to invoke the Kernel for sharing this variable
	//		sys_getSharedObject(): if succeed, it returns the ID of the shared variable. Else, it returns -ve
	//	6) If the Kernel successfully share the variable, return its virtual address
	//	   Else, return NULL
	//

	//This function should find the space for sharing the variable
	// ******** ON 4KB BOUNDARY ******************* //

	//Use sys_isUHeapPlacementStrategyFIRSTFIT() to check the current strategy

	return NULL;
}

