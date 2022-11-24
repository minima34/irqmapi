#include "irqmap.h"

#include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/of_gpio.h>
#include <linux/sched.h>

#include <linux/fs.h>

#include <linux/module.h>
#include <asm/uaccess.h> // put_user_func

#include <linux/slab.h>
#include <linux/mm.h> 
//#include <linux/workqueue.h>
#include <linux/wait.h>                 //Required for the wait queues
#include <linux/poll.h>

#define MAX_IRQS sizeof(int) //4*8 32 irqs
#define irqmap_NAME "irqmap"
#define CLASS_NAME "q_mogu"
#define IRQMMAP_PAGES 4
#define IRQMMAP_SIZE (PAGE_SIZE * IRQMMAP_PAGES)   /* max size mmaped to userspace */
#define LAST_MMAP_BYTE (IRQMMAP_SIZE-1)
#define LAST_QUEUE_BYTE (PAGE_SIZE-1)

//Waitqueue
DECLARE_WAIT_QUEUE_HEAD(irqmap_queue);

/*
** Function Prototypes
*/
static int irqmap_open(struct inode *inode, struct file *filp);
static int irqmap_release(struct inode *inode, struct file *filp);
static ssize_t irqmap_read(struct file *filp, char *buffer, size_t length, loff_t *offset);
static ssize_t irqmap_write(struct file *filp, const char *buf, size_t len, loff_t *off);
static int irqmap_mmap(struct file *filp, struct vm_area_struct *vma);
static unsigned int irqmap_poll(struct file *filp, poll_table *wait);
static void irqmap_send_ready_event(void);

/*
*/
int irqs[MAX_IRQS];
static char *irq_strs[MAX_IRQS] = {0};

/*
** File operation sturcture
*/
static const struct file_operations fops = {
    .open = irqmap_open,
    .read = irqmap_read,
    .write = irqmap_write,
    .release = irqmap_release,
    .mmap = irqmap_mmap,
    .poll = irqmap_poll,
    /*.unlocked_ioctl = irqmap_ioctl,*/
    .owner = THIS_MODULE,
};
static int major;
static struct class*  class;
static struct device*  device;
//static int irqmap_Open = 0;

#define SUCCESS 0
#define BUF_LEN 80
static char msg[BUF_LEN];
static char *msg_Ptr;

static DEFINE_MUTEX(irqmap_mutex);
struct irq_mmap *mmap;

//todo
//static bool can_write = false;
//static bool can_read  = false;

// kvi magiiki a :D :D :D :D :D :D
//
//I have 4096 bytes memory in this 1 page map
// sizeof(int) is 4 bytes

int int_get_array_index(int val, int *arr, u32 size)
{
	int index;

	// for(i=0; i<size; i++)
		// if(val == *(arr+index*sizeof(int)))
	for(index=0; index<size; index++)
		if(val == *(arr+index))
			return index;
}

static void irqmap_send_ready_event(void)
{
    //this will spam a lot
    //
    //
    // this to check!!!
    //if(*ives->rp) if(ives->irqh)
    //    ives->irqh(*ives->rp);
    //

    //todo @ finish
    //
    // poll shit here tommorow
    //

    //can_write = true;
    //wake up the waitqueue
    //wake_up(&wait_queue_etx_data);
    //
    mmap->ready=true;
    wake_up(&irqmap_queue);

    //
    //podobre da pestim ram otkolkoto cpu   ram -eq water   cpu -eqs electricity kwh  :)))) happy smile :D :))))) //this is the most commented kernel space code :D
}

static void irqmap_queue_add(int irq_ind)
{
    //this is the interrupt fork
    // you must be ultra fast here

    //lets walk in this memory area :D
    //no lets do it efficently
    // higher KPD
    // with walking pointer :) :D #flow_order :)
    
    // we use the last_mmap_byte here
    
    *mmap->wp = irq_ind; //assign interrupt value/number :D :)

    //other way
    //ives->wp == q+(4064-1)
    // but this way its more resource expensive to know bytes of queue
    //
    // haha izgubih se :D <3
    //
    // if(mmap->.wp == &mmap->queue+LAST_QUEUE_BYTE)
    // |
    if(mmap->wp == mmap+LAST_MMAP_BYTE)
    {
        //
        //LAST_MMAP_BYTE here
        // reset wp to beggining of queue (addr)
        mmap->wp = mmap->events;
        //void * memset ( void * ptr, int value, size_t num );
        // i think its not needed but idk, suppose yes to not lose data :) #will_see_later_I_must_impl_wa_to_see_ram
        //ways :) <#
        //memory map :)
        //steve jobs is my acid friend :)
        //
        printk(KERN_NOTICE "irqmap_ram: LAST_MMAP_BYTE@%p => %x", mmap->wp, *mmap->wp);
        //
        //oyea
        //
    }
    else
        mmap->wp++; //starts with the first byte of the queue, goto next one :D :)

    // we ready, tell the userspace to read :D :)
    //
    irqmap_send_ready_event();
}

static unsigned int irqmap_poll(struct file *filp, struct poll_table_struct *wait)
{
  unsigned int mask = 0;
  //__poll_t mask = 0;
  
poll_wait(filp, &irqmap_queue, wait);
  pr_info("irqmap_poll:\n");
  
  if(mmap->ready)
  {
    mmap->ready=false;
    mask |= (POLLIN | POLLRDNORM);
  }

//    *
//    * The buffer is circular; it is considered full
//    * if "wp" is right behind "rp". "left" is 0 if the
//    * buffer is empty, and it is "1" if it is completely full.
//    *

//todo
  //if(mmap->rp != mmap->wp)
  //  mask |= POLLIN | POLLRDNORM; // readable

  return mask;
}

/// @brief todo area
/// @param setvaneto e napraveno
/// @param ostava clear bate
/// @return YE :)
static irqreturn_t irq_handler (int irq, void *dev_id)
{
	static unsigned long flags = 0;
  
#ifdef EN_DEBOUNCE
   unsigned long diff = jiffies - old_jiffie;
   if (diff < 20)
   {
     return IRQ_HANDLED;
   }
  
  old_jiffie = jiffies;
#endif

	int irq_ind;
	irq_ind = int_get_array_index(irq, irqs, MAX_IRQS);

	printk (KERN_NOTICE "irqmap: interrupt %d fired\n", irq);

	mmap->irqs |= 1 << irq_ind;

	//local_irq_save(flags);
  	//led_toggle = (0x01 ^ led_toggle);                             // toggle the old value
  	//gpio_set_value(GPIO_21_OUT, led_toggle);                      // toggle the GPIO_21_OUT
	//pr_info("Interrupt Occurred : GPIO_21_OUT : %d ",gpio_get_value(GPIO_21_OUT));
  	//local_irq_restore(flags);

    irqmap_queue_add(irq_ind);
    // flowa ko prai :D :D
    //


  	return IRQ_HANDLED;
}

static int irqmap_init_irqs(struct platform_device *pdev)
{
	int i, nirq;

for(i=0; i<MAX_IRQS; i++)
		irqs[i] = -1;

	struct device_node *np=0;
	np = pdev->dev.of_node; //of_property_read_u32(np, "irq_count", &(nirq));
	nirq = of_irq_count(np);
	printk(KERN_NOTICE "irqmap: %u found in dts", nirq);

	if(nirq==0)
		return -EAGAIN;
	//return Try Again
	//xD
	int deb;

	for(i=0; i<nirq; i++)
	{
		irq_strs[i] = kzalloc(64, GFP_KERNEL); //irq = platform_get_irq_byname (pdev, "irqmap_irq");
		irqs[i] = platform_get_irq(pdev, i);

		sprintf(irq_strs[i], "irqmap_irq%d", i+1);

		if(request_irq(irqs[i], &irq_handler, IRQF_TRIGGER_FALLING, irq_strs[i], NULL))
		{
			printk(KERN_ERR "irqmap: cannot register IRQ %d <-- %s\n", irqs[i], irq_strs[i]);
			return -EIO;
		}

		//deb = gpiod_set_debounce(struct gpio_desc *desc, unsigned int debounce)¶
	}

	return 0;
}

static int __init irqmap_probe(struct platform_device *pdev)
{
	int ret = 0;

	ret = irqmap_init_irqs(pdev);
	printk(KERN_NOTICE "irqmap: PAGE_SIZE is %u", PAGE_SIZE);
	if (ret < 0) goto out;

    major = register_chrdev(0, irqmap_NAME, &fops);
    if (major < 0) {
        pr_info("irqmap: fail to register major number! %d", major);
		//printk(KERN_ALERT "irqmap: Registering char device failed with %d\n", major);
		//return major;
        ret = major;
        goto out;
    }

    class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(class)){ 
        unregister_chrdev(major, irqmap_NAME);
        pr_info("irqmap: failed to register device class");
        ret = PTR_ERR(class);
        goto out;
    }

    device = device_create(class, NULL, MKDEV(major, 0), NULL, irqmap_NAME);
    if (IS_ERR(device)) {
        class_destroy(class);
        unregister_chrdev(major, irqmap_NAME);
        ret = PTR_ERR(device);
        goto out;
    }

	/*
	printk(KERN_INFO "irqmap: I was assigned major number %d. To talk to\n", major);
	printk(KERN_INFO "irqmap: the driver, create a dev file with\n");
	printk(KERN_INFO "irqmap: 'mknod /dev/%s c %d 0'.\n", irqmap_NAME, major);
	printk(KERN_INFO "irqmap: Try various minor numbers. Try to cat and echo to\n");
	printk(KERN_INFO "irqmap: the device file.\n");
	printk(KERN_INFO "irqmap: Remove the device file and module when done.\n");
	*/

	/* init this mmap area */
    // mmap = kmalloc(IRQMMAP_SIZE, GFP_KERNEL);
    mmap = kzalloc(IRQMMAP_SIZE, GFP_KERNEL);
    if (mmap == NULL) {
        ret = -ENOMEM; 
        goto out;
    }

    //
    // init queue pointers
    //
    // todo see offsets
    //
    mmap->ready=false;
    mmap->events = (int*)mmap+2;
    mmap->rp = mmap->events;
    mmap->wp = mmap->events;

	
	// ives->irq1 |= 1 << 2;
	// ives->irq1 |= 1 << 3;
	// ives->irq2 |= 1 << 3;
	// ives->irq2 |= 1 << 4;
	// ives->irq3 |= 1 << 1;
	// ives->irq3 |= 1 << 2;
	// ives->irq4 |= 1 << 9;
	// ives->irq4 |= 1 << 10;

    //Initialize wait queue
    //init_waitqueue_head(&irqmap_queue);

    mutex_init(&irqmap_mutex);
out: 
    return ret;
	
	//return 0;
}

static int irqmap_remove(struct platform_device *pdev)
{
	int i=0;

	mutex_destroy(&irqmap_mutex); 
    device_destroy(class, MKDEV(major, 0));
	class_unregister(class);
    class_destroy(class);
	unregister_chrdev(major, irqmap_NAME);

	for(i=0; i<MAX_IRQS; i++)
	{
		if(irqs[i] == -1)
			continue;
		free_irq(irqs[i], NULL);
	}

	kfree(mmap);
	pr_info("irqmap: unregistered!");
	
	return 0;
}

/*
** This function will be called when we open the Device file
*/
static int irqmap_open(struct inode *inode, struct file *filp)
{
	int ret = 0;

if(!mutex_trylock(&irqmap_mutex)) {
        pr_alert("irqmap: device busy!\n");
        ret = -EBUSY;
        goto out;
    }

pr_info("irqmap: Device opened\n");

  //static int counter = 0;
  //if(irqmap_Open)
  //  return -EBUSY;
  //irqmap_Open++;
  
  //see here later
  //sprintf(msg, "%p\n", ives);
  //msg_Ptr = msg;
  
  //try_module_get(THIS_MODULE);
out:
    return ret;
}

/*
** This function will be called when we close the Device file
*/
static int irqmap_release(struct inode *inode, struct file *filp)
{
  //irqmap_Open--;

mutex_unlock(&irqmap_mutex);
    pr_info("device: Device successfully closed\n");
  /*
   * Decrement the usage count, or else once you opened the file, you'll never
   * get rid of the module.
   */
  //module_put(THIS_MODULE);

  return 0;
return 0;
}
// dva returna xD

/*
** This function will be called when we read the Device file
*/
static ssize_t irqmap_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
    int ret;
    
    if (len > IRQMMAP_SIZE) {
        pr_info("irqmap: read overflow!\n");
        ret = -EFAULT;
        goto out;
    }

    if (copy_to_user(buffer, mmap, len) == 0) {
        pr_info("irqmap: copy %u char to the user\n", len);
        ret = len;
    } else {
        ret =  -EFAULT;   
    } 

out:
    return ret;
}

/*
** This function will be called when we write the Device file
*/
static ssize_t irqmap_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
    int ret;
 
    if (copy_from_user(mmap, buffer, len)) {
        pr_err("irqmap: write fault!\n");
        ret = -EFAULT;
        goto out;
    }
    pr_info("irqmap: copy %d char from the user\n", len);
    ret = len;

out:
    return ret;
}

static int irqmap_mmap(struct file *filp, struct vm_area_struct *vma)
{
    int ret = 0;
    struct page *page = NULL;
    unsigned long size = (unsigned long)(vma->vm_end - vma->vm_start);

    if (size > IRQMMAP_SIZE) {
        ret = -EINVAL;
        goto out;  
    } 
   
    page = virt_to_page((unsigned long)mmap + (vma->vm_pgoff << PAGE_SHIFT)); 
    ret = remap_pfn_range(vma, vma->vm_start, page_to_pfn(page), size, vma->vm_page_prot);
    if (ret != 0) {
        goto out;
    }   

out:
    return ret;
}

#if defined(CONFIG_OF)
static const struct of_device_id irqmap_dt_ids[] = {
	{ .compatible = "irqmap" },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, irqmap_dt_ids);
#endif

static struct platform_driver irqmap_driver = {
	.remove		= irqmap_remove,
	.driver		= {
		.name	= "irqmap",
		.of_match_table	= of_match_ptr(irqmap_dt_ids),
	},
};

module_platform_driver_probe(irqmap_driver, irqmap_probe);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ACHO@TID");
MODULE_DESCRIPTION("interrupts userspace mapping driver");