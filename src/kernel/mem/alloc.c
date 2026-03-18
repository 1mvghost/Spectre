#include <debug.h>
#include <util.h>
#include <pmm.h>
#include <vmm.h>
#include <alloc.h>

#define ALIGN		16

#define REG_FREE 	0x0
#define REG_ALLOC	0x1

struct HeapChunk {
	size_t 		   	  Size;
	u64 		      Type;
	struct HeapChunk  *Prev;
	struct HeapChunk  *Nxt;
};
static struct HeapChunk *hp = 	0;

/* request pages */
bool spallocReq(size_t pages) {
	//debug("spalloc: page request!\n");
	void* p = VIRT(pmmAlloc(pages));
	if(!p) {
		debug("spalloc: cannot get a new page! probably ran out of memory!\n");	
		return 0;
	}
	struct HeapChunk *h = p;
	h->Size  	 = (pages*PAGE_SIZE)-sizeof(struct HeapChunk);
	h->Type  	 = REG_FREE;
	h->Nxt 	 	 = 0;
	h->Prev		 = 0;
	/* if not initalized */
	if(!hp) {
		hp=p;
	} else {
		/* extend */
		struct HeapChunk *cur = hp;
		while(cur && cur->Nxt) {
			cur=cur->Nxt;
		}

		cur->Nxt = p;
		cur->Nxt->Prev = cur;
	}
	return 1;
}
size_t spalign(size_t size){
	if((size % ALIGN) != 0) {
		size = ((size/ALIGN)+1)*ALIGN;
	}
	return size;
}

void* malloc(size_t size) {
	/* the actual thing. */
	if(!size) return 0;
	if(size == 0) return 0;
	size = spalign(size);

	size_t pages = ((size+sizeof(struct HeapChunk)) / PAGE_SIZE);
	if(((size+sizeof(struct HeapChunk)) % PAGE_SIZE) != 0) {
		++pages;
	}

	struct HeapChunk *fr = 0;

	if(!hp) {
		/* alloc the heap if didnt already */
		if(!spallocReq(pages)) return 0;
	}
	struct HeapChunk *cur = hp;

	while(cur) {
		if(cur->Type==REG_FREE && (cur->Size == size || cur->Size >= size+sizeof(struct HeapChunk))) {
			fr=cur;
			break;
		}
		cur=cur->Nxt;
	}

	if(!fr) {
		/* didnt find a heap chunk big enough ... */
		if(!spallocReq(pages)) {
			return 0;
		}
		struct HeapChunk *l = hp;
		while(l && l->Nxt) {
			l=l->Nxt;
		}
		fr=l;
	}

	//debug("%x\n",fr);
	/* we now have a chunk */
	if(fr->Size==size || fr->Size-size <= sizeof(struct HeapChunk)) {
		fr->Type = REG_ALLOC;
		return (void*)fr+sizeof(struct HeapChunk);
	}

	/* split */
	struct HeapChunk *nxt = fr->Nxt;

	size_t l = (size_t)fr+sizeof(struct HeapChunk)+size;
	struct HeapChunk *c = (void*)l;

	c->Prev 		= fr;
	c->Nxt 			= 0;	
	if(nxt) c->Nxt 	= nxt;

	c->Size 		= fr->Size-size-sizeof(struct HeapChunk);
	c->Type 		= REG_FREE;
	
	if(nxt) nxt->Prev = c;

	fr->Nxt 		= c;
	fr->Size 		= size;
	fr->Type 		= REG_ALLOC;


	return (void*)fr+sizeof(struct HeapChunk);
}

void free(void* addr) {
	if(!addr) return;

	/* double free check */
	struct HeapChunk *ch = addr-sizeof(struct HeapChunk);

	struct HeapChunk *cur = hp;
	while(cur){
		if(cur == addr-sizeof(struct HeapChunk)) {
			goto free;
		}
		cur=cur->Nxt;
	}
	goto dfree;
free:
	if(ch->Type == REG_FREE) {
		goto dfree;
	}
	ch->Type = REG_FREE;
	if(ch->Prev && ch->Prev->Type == REG_FREE) {
		/* when addresses are not contiguous! */
		if((void*)ch->Prev+sizeof(struct HeapChunk)+ch->Prev->Size == (void*)ch) {
			struct HeapChunk *prev = ch->Prev;
			struct HeapChunk *nxt = ch->Nxt;

			prev->Size += ch->Size+sizeof(struct HeapChunk);
			prev->Nxt = 0;
			if(nxt) {
				nxt->Prev = prev;
				prev->Nxt = nxt;
			}
			memset(ch,0,sizeof(struct HeapChunk));
			ch = prev;
		}
	}
	if(ch->Nxt && ch->Nxt->Type == REG_FREE) {
		/* when addresses are not contiguous! */
		if((void*)ch->Nxt-sizeof(struct HeapChunk)-ch->Size != (void*)ch) {
			return;
		}
		struct HeapChunk *nxt = ch->Nxt;
		struct HeapChunk *nxtt = nxt->Nxt;

		ch->Size += nxt->Size+sizeof(struct HeapChunk);
		ch->Nxt  = 0;
		if(nxtt) {
			nxtt->Prev = ch;
			ch->Nxt = nxtt;
		}
		memset(nxt,0,sizeof(struct HeapChunk));
	}

	return;
dfree:
	debug("spalloc: double free!! %x\n",addr);
	return;

}
void* calloc(size_t size) {
	if(!size) return 0;
	if(size == 0) return 0;
	size = spalign(size);

	void* m = malloc(size);
	memset(m,0,size);
	
	return m;
}

void* realloc(void* addr, size_t size) {
	if(!size) return 0;
	if(size == 0) return 0;
	size = spalign(size);

	free(addr);
	void* m = malloc(size);

	return m;

}

void spdmp() {
	int m=0;
	struct HeapChunk *cur = hp;
	while(cur) {
		printf(INFO,"spalloc: %x > size:%d type:%d next:%x prev:%x\n",cur,cur->Size,cur->Type,cur->Nxt,cur->Prev);
		m+=cur->Size+sizeof(struct HeapChunk);
		cur=cur->Nxt;
	}
	
	debug("--- %d\n",m);
}