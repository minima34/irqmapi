#include <fcntl.h>  
#include <unistd.h>  
#include <sys/mman.h>  
#include <stdio.h>
#include <string.h>
#include <errno.h>
  
#define DEVICE_FILENAME "/dev/irqmap"   
#define PAGES 1
#define PAGE_SIZE getpagesize()
#define MMAP_BYTES (PAGE_SIZE * PAGES)

struct irq_vals {
	int irqs;                  //4
    void (*irqh)(int);        //8 suppose
    //write pointer add to q
    void *wp;                  //8 -> 20
    //read pointer read from q
    void *rp;                  //8 -> o yea 28 here
    int i_ev;                  //4 -> 32 hihihi govori_mi_finala
    struct prot_queue *q;      //4064
};
static struct irq_vals *ivs;

int bit_test(int bit, int byte)
{
    bit = 1 << bit;
    return(bit & byte);
}
make
void irq_handler(int irq_ind)
{
    //
    //tuka idva na 1ms ama samo ako ima v q-to neshto event :)
    printf("irqmap_queue_event: interrupt happens :D => irq[%d] :)\n", irq_ind);
    // we here mustnt spend time to not miss interrupts
    // this is the place where we must be FAST !
    //
    // fuk muzikata mi idva slaba :@ no amplifier around
    //
    //tanz_#jivota_e_hubav_map :)
    //
}

//
//#mnogo_mu_sa_ebe_na_tui_mom4e xD/
//:D

// razkrasqva go :D da e huu :D
//

void irq_dump()
{
    int i;

    for(i=0;i<32;i++)
    {
        if(bit_test(i, ivs->irqs)) 
            printf("irqs@%d => 1\n", i);
        else
            printf("irqs@%d => 0\n", i);
    }
    printf("\n");
}

int main(void)  
{
	printf("\n");
  
    int fd;  
    int ret;
    char *p = NULL;
    char buff[64];    
  
    fd = open(DEVICE_FILENAME, O_RDWR|O_NDELAY);  
  
//
// cigarrete pause and eat
//
// this tiny cancer is my frined :) <3 
// love my tiny cancer, I need to feed my friend, its hungry veeery
//:D
//
// lets work
//.
// its hungry very its hungry very its hungry very its hungry very its hungry very its hungry very its hungry very its hungry very its hungry very its hungry very 
//its hungry very its hungry very its hungry very its hungry very its hungry very its hungry very its hungry very its hungry very its hungry very its hungry very 
// its hungry very its hungry very its hungry very its hungry very its hungry very its hungry very its hungry very its hungry very its hungry very its hungry very its 
//
// neka da beaty :D :) beauty :)))) :D
//
// lets work 
    if(fd < 0)
    {
        printf("There is a problem with open chardev to file descriptor\n");
        goto out;
    }
 
    ivs = (char*)mmap(0, MMAP_BYTES, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    ivs->irqh = irq_handler;
    irq_dump();
    
    /*
    printf("ivs->irqs == %x\n", ivs->irqs);
    printf("ivs->irq1 == %x\n", ivs->irq1);
    printf("ivs->irq2 == %x\n", ivs->irq2);
    printf("ivs->irq3 == %x\n", ivs->irq3);
    printf("ivs->irq4 == %x\n", ivs->irq4);
    */
    //printf("\n");
    
    munmap(p, MMAP_BYTES);
    close(fd);

//microsoft
// nqma da komentiram

out:
    close(fd);
    return ret;
} 
