/*
 *
 *    A C H O 
 *  D R I V E R
 * 
 */

//
// no sleep, sleep kills + sleep healths
// day                     night
// obrushta se neshto prizmata s narastvaneto na umorata 
//

//4 //void (*irqh)(int);

struct irq_mmap
{
	int irqs;//4
    int ready;//4
    int *rp; //read ptr
    int *wp; //write ptr
    int *events;
};

// struct irqmap_event
// {
//     int irq_ind;
// };

// struct irq_mmap
// {
//     struct irq_vals ivs;
//     // struct irq_queue iqu;
// };