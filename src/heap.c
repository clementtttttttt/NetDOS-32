#include "heap.h" 
void k_heapSSInit(KHEAPSS *heap, uint32 bsize) {
	heap->fblock = 0;
	heap->bsize = bsize;
}

int k_heapSSAddBlock(KHEAPSS *heap, uintptr addr, uint32 size) {
	KHEAPBLOCKSS		*b;
	uint32				x;
	uint32				*stack;
	uint32				stacksz;
	uint32				slotres;
	
	b = (KHEAPBLOCKSS*)addr;
	b->next = heap->fblock;
	heap->fblock = b;
	
	b->size = size;
	
	size = size - sizeof(KHEAPBLOCKSS);
	
	b->max = size / (heap->bsize);
	
	stacksz = b->max * 4;
	slotres = (stacksz / heap->bsize) * heap->bsize < stacksz ? (stacksz / heap->bsize) + 1 : stacksz / heap->bsize;
	
	b->top = slotres;
	
	stack = (uint32*)&b[1];
	for (x = slotres; x < b->max; ++x) {
		stack[x] = x * heap->bsize;
	}
		
	return 1;
}

void *k_heapSSAlloc(KHEAPSS *heap, uint32 size) {
	KHEAPBLOCKSS		*b;
	uintptr				ptr;
	uint32				*stack;
	
	/* too big */
	if (size > heap->bsize) {
		return 0;
	}
	
	for (b = heap->fblock; b; b = b->next) {
		if (b->top != b->max) {
			stack = (uint32*)&b[1];
			ptr = stack[b->top++];
			ptr = (uintptr)&b[1] + ptr;
			return (void*)ptr;
		}
	}
	
	/* no more space left */
	return 0;
}

void k_heapSSFree(KHEAPSS *heap, void *ptr) {
	KHEAPBLOCKSS		*b;
	uintptr				_ptr;
	uint32				*stack;
	
	/* find block */
	_ptr = (uintptr)ptr;
	for (b = heap->fblock; b; b = b->next) {
		if (_ptr > (uintptr)b && _ptr < ((uintptr)b + b->size))
			break;
	}
	
	/* might want to catch this somehow or somewhere.. kinda means corruption */
	if (!b)
		return;
	
	/* get stack */
	stack = (uint32*)&b[1];
	/* normalize offset to after block header */
	/* decrement top index */
	/* place entry back into stack */
	stack[--b->top] = _ptr - (uintptr)&b[1];
	return;
}
