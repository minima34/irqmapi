/*
 *
 *    A C H O 
 *  D R I V E R
 * 
 */

struct irqmap_events
{
    //wr ptr
    int *wp;
    //rd ptr
    int *rp;
    int i_ev;
    
    //
    // no sleep, sleep kills + sleep healths
    // day                     night
    // obrushta se neshto prizmata s narastvaneto na umorata 
    //
};

struct irq_mmap
{
	int irqs;
    //4 //void (*irqh)(int);
    struct irqmap_events *ives;
    //dedicated to my sis
    int *events;
};

