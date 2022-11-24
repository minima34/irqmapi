#include <stdio.h>

struct irq_mmap
{
	int irqs;//4
	int *rp; //read ptr
	int *wp; //write ptr
	int *events;
	int ready;
};

struct irq_mmap *mmap = 0;
char *mmap_byte = 0;

int main()
{
	mmap = malloc(sizeof(struct irq_mmap));
	memset(mmap,0,sizeof(struct irq_mmap));

	mmap_byte = (char*)mmap;

	printf("mmap      is  @ %p\n", (int*)mmap+1);
	printf("mmap + 4bytes(int) is @ %p\n\n", mmap_byte+4);

	printf("mmap_byte is  @ %p\n", mmap_byte);
	mmap_byte+=4;
	printf("mmap + 4bytes(int) is @ %p\n\n", mmap_byte);

	printf("lets see what happens mmap+1\n\n");

	mmap->events = (int*)mmap+1;
	printf("mmap_events @ %p\n", mmap->events);

	return 0;
}
