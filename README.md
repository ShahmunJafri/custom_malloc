# custom_malloc  
Custom heap allocator created in C--Implemented the free and malloc functions in C. 
  
This project is a simplified implementation of malloc() and free() in C, designed to simulate memory management through a custom virtual memory allocator. Developed as part of the CSE 29 coursework, it demonstrates core concepts of dynamic memory allocation, block splitting, coalescing, and best-fit strategy.  
  
Custom malloc() (vmalloc): Allocates memory blocks using the best-fit algorithm to minimize fragmentation.  
Custom free() (vmfree): Safely frees allocated memory and merges adjacent free blocks (coalescing).  
Block Headers and Footers: Metadata stored with each block to track size and allocation status.  
Virtual Heap Initialization (vminit)  
Unit Testing (vmtest.c, tests/)  
  
vmalloc.c: Core implementation of the custom memory allocator  
vmfree.c: Handles memory deallocation and coalescing  
vminit.c: Initializes the virtual memory space  
vm.h / vmlib.h: Header files defining macros and data structures  
utils.c: Helper functions  
vmtest.c: Test cases and sample usage  
Makefile: Build configuration  
README.md: Project documentation  
