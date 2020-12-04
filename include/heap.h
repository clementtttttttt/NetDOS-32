
#ifndef heap_h_i
#define heap_h_i
typedef unsigned int uint32;
typedef unsigned int uintptr;
typedef struct _KHEAPBLOCKSS {
	struct _KHEAPBLOCKSS	*next;
	uint32					top;
	uint32					max;
	uintptr					size;			/* total size in bytes including this header */
} KHEAPBLOCKSS;

typedef struct _KHEAPSS {
	KHEAPBLOCKSS			*fblock;
	uint32					bsize;
} KHEAPSS;

void k_heapSSInit(KHEAPSS *heap, uint32 bsize) ;
int k_heapSSAddBlock(KHEAPSS *heap, uintptr addr, uint32 size) ;
void *k_heapSSAlloc(KHEAPSS *heap, uint32 size) ;

void k_heapSSFree(KHEAPSS *heap, void *ptr) ;
    
#endif
