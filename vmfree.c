#include "vm.h"
#include "vmlib.h"
#include <stdio.h>
/**
 * The vmfree() function frees the memory space pointed to by ptr,
 * which must have been returned by a previous call to vmmalloc().
 * Otherwise, or if free(ptr) has already been called before,
 * undefined behavior occurs.
 * If ptr is NULL, no operation is performed.
 */

void* coalesce(struct block_header**, struct block_header**);
void set_block_footer(struct block_header*);

void vmfree(void *ptr)
{
	if(ptr == NULL){
		return;
	}
	
	struct block_header* curr_block_header = (struct block_header*)((char *)(ptr) - sizeof(struct block_header));
//	size_t size_with_mask = block_ptr->size_status & VM_BLKSZMASK;
//	printf("The value of size is: %zu\n",size_with_mask);

	if(!(curr_block_header->size_status & VM_BUSY)){
		return; 
	}

	if(!(curr_block_header->size_status & VM_PREVBUSY)){
		struct block_footer* prev_footer = (struct block_footer*)((char*)(curr_block_header) - sizeof(struct block_footer));
		struct block_header* prev_header = (struct block_header*)((char*)(ptr) - (prev_footer->size));
		curr_block_header = coalesce(&prev_header, &curr_block_header);
	}

	struct block_header* next_header = (struct block_header*)((char*)(curr_block_header) + BLKSZ(curr_block_header));
	
	if(!(next_header->size_status & VM_BUSY)){
		curr_block_header = coalesce(&curr_block_header, &next_header);
	}
	
	set_block_footer(curr_block_header);

	struct block_header* next_block_after_coalesce = (struct block_header*)((char*) curr_block_header + BLKSZ(curr_block_header));

	if(next_block_after_coalesce->size_status == VM_ENDMARK){
		return;
	}
	
	next_block_after_coalesce->size_status = next_block_after_coalesce->size_status & ~VM_PREVBUSY;

	
}

void set_block_footer(struct block_header* block){
	struct block_footer* footer = (struct block_footer*) (((char *)(block)) + BLKSZ(block) - sizeof(struct block_footer));
	footer->size = BLKSZ(block);
}

void *  coalesce(struct block_header** prev, struct block_header** curr){
	struct block_header* new_block = *prev;
	size_t prev_size_status = new_block->size_status;

	size_t curr_size = BLKSZ(*curr);
	size_t prev_size = BLKSZ(*prev);

	new_block->size_status = curr_size + prev_size; 
	new_block->size_status = new_block->size_status | (prev_size_status & VM_PREVBUSY);

	return  new_block;
}

