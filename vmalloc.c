#include "vm.h"
#include "vmlib.h"
#include "stdio.h"
struct block_header* best_fit(struct block_header*, size_t);
void block_split(struct block_header*, size_t);
void set_footer(struct block_header*);

void *vmalloc(size_t size)
{
	if(size == 0){
		return NULL;
	}
	
	size_t size_with_header = size + sizeof(struct block_header);
	
	size_t size_aligned = ROUND_UP(size_with_header, BLKALIGN);
	//printf("The value of size is: %zu\n", size);
	//printf("The value of size w/ header is: %zu\n", size_with_header);	
	printf("The value of size is: %zu\n", size_aligned); 

	
	struct block_header* header = best_fit(heapstart, size_aligned);
	if(header == NULL){
		return NULL;
	}


	printf("The value of block_header after best fit: %zu\n", (BLKSZ(header)));	
	
	header->size_status = header->size_status | VM_BUSY;
	if((BLKSZ(header)) > size_aligned){
		//printf("The value of size aligned and blksize: %zu %zu \n", size_aligned, BLKSZ(header));
		block_split(header, size_aligned);
	}			
	
	return header + 1; 

}

void set_footer(struct block_header* block){
	struct block_footer* footer = (struct block_footer*)((char*)(block) + BLKSZ(block) - sizeof(struct block_footer));
	footer->size = BLKSZ(block);
}

void block_split(struct block_header* header, size_t size){
	struct block_header* next_block = (struct block_header*)((char*) header + size);
		
//	printf("The value of size is: %p %p\n", header, next_block);
	
	next_block->size_status = BLKSZ(header) - size;
	next_block->size_status = next_block->size_status & ~VM_BUSY;
	next_block->size_status = next_block->size_status | VM_PREVBUSY;
	
	int prev_busy = header->size_status & VM_PREVBUSY;
	header->size_status = size;
	header->size_status = header->size_status | VM_BUSY;

	if(prev_busy){
		header->size_status = header->size_status | VM_PREVBUSY;
	}
	set_footer(next_block);
}

struct block_header* best_fit(struct block_header* header, size_t size){
	struct block_header* current = header;
	struct block_header* goal = NULL;
	size_t current_size;
		
	while(current->size_status != VM_ENDMARK){
		current_size = BLKSZ(current);
		int isallocated = current->size_status & VM_BUSY;
		
		if(current_size == size && !isallocated){
			return current;
		} else if(current_size > size && (goal == NULL || BLKSZ(goal) > current_size) && !isallocated){
			goal = current;
		}
		current = (struct block_header*)((char*)current + current_size);
	}
	
	return goal;
}
