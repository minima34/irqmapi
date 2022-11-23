#include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/of_gpio.h>
#include <linux/sched.h>

//#include <linux/kernel.h>
#include <linux/fs.h>
//#include <linux/cdev.h>
#include <linux/module.h>
#include <asm/uaccess.h> /* for put_user */
#include <linux/slab.h>

/*
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/init.h> 
#include <linux/fs.h> 
*/
#include <linux/mm.h> 
/*
#include <asm/uaccess.h>
*/
#include <linux/workqueue.h>

static void irqmap_send_event(struct work_struct *w);

static struct workqueue_struct *wq = 0;
static DECLARE_DELAYED_WORK(irqmap_work, irqmap_send_event);
static unsigned long qd;


#define MAX_IRQS sizeof(int) //32
#define DEVICE_NAME "irqmap"
#define CLASS_NAME "q_mogu"
#define MMAP_SIZE (PAGE_SIZE *1)   /* max size mmaped to userspace */
#define LAST_MMAP_BYTE (MMAP_SIZE-1)

/*
 *
 *    A C H O 
 *  D R I V E R
 * 
 * 
 */


static int device_open(struct inode *inode, struct file *filp);
static int device_release(struct inode *inode, struct file *filp);
static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset);
static ssize_t device_write(struct file *filp, const char *buf, size_t len, loff_t *off);
static int device_mmap(struct file *filp, struct vm_area_struct *vma);

/*
*/
int irqs[MAX_IRQS];
static char *irq_strs[MAX_IRQS] = {0};
/*
static struct file_operations fops = {
  .read = ,
  .write = ,
  .open = ,
  .release = ,
};
*/
static const struct file_operations fops = {
    .open = device_open,
    .read = device_read,
    .write = device_write,
    .release = device_release,
    .mmap = device_mmap,
    /*.unlocked_ioctl = device_ioctl,*/
    .owner = THIS_MODULE,
};
static int major;
static struct class*  class;
static struct device*  device;
//static int Device_Open = 0;
#define SUCCESS 0
#define BUF_LEN 80
static char msg[BUF_LEN];
static char *msg_Ptr;

//static char *ivs = NULL; 
static DEFINE_MUTEX(device_mutex);

//
// this is the RAM area where we will the queue for the energo protections :D
//
struct prot_queue
{
    int events[1016];
    //
    //1016 ints * 4 bytes == 4064 :)
    //
    // so we can handle 1016 events in this queue :)
    //
    // or events are numbers so lets think it how to encode events to numbers :)
    //
    // ok our queue will start with 'ff' bytes which in 4 bytes system means:
    // ff is 1 byte
    // so ffffffff == 4294967295 , this is the maximum number of unsigned int :)
    // same
    // as
    // zeros, imagine 1016 bytes filled with zeros
    // ok let fill with zeros :D
    //
    // for ur simplyness
    // :)
    //
    // lets fill it 
    // Oh stupid me, its already filled with 00 :D
    // :D
    //
    // so our event will be interrupt number, lets map it out :)
};

struct irq_vals {
    //
	int irqs;                  //4
    void (*irqh)(int);        //8 suppose
    //write pointer add to q
    int *wp;                  //8 -> 20
    //read pointer read from q
    int *rp;                  //8 -> o yea 28 here
    int i_ev;                  //4 -> 32 hihihi govori_mi_finala
    struct prot_queue *q;      //4064
    //
    // no sleep, sleep kills
};

// kvi magiiki a :D
//

//I have 4096 bytes memory in this 1 page map
// sizeof(int) is 4 bytes

static struct irq_vals *ivs;

static void
mykmod_work_handler(struct work_struct *w)
{
    //this will spam a lot
    //pr_info("mykmod work %u jiffies\n", (unsigned)onesec);
    if(*ivs->rp) if(ivs->irqh)
        ivs->irqh(*ivs->rp);
    //podobre da pestim ram otkolkoto cpu   ram -eq water   cpu -eqs electricity kwh  :)))) happy smile :D :))))) //this is the most commented kernel space code :D
}


int int_get_array_index(int val, int *arr, u32 size)
{
	int index;

	// for(i=0; i<size; i++)
		// if(val == *(arr+index*sizeof(int)))
	for(index=0; index<size; index++)
		if(val == *(arr+index))
			return index;
}

//ivs=0+3_last_byte_(4-1)==3
//0//                     /
//1//                  /
//2//               /
//3//            <       #to4no_last_byte
//size 4 bytes
//
// 0 <  0    0    0 
// 1    1 <  1    1
// 2    2    2 <  2
// 3    3    3    3 <
//
// 0 <  0    0    0 
// 1    1 <  1    1
// 2    2    2 <  2
// 3    3    3    3 <
//

void zashtiti_queue_add(int irq_ind)
{
    //lets walk in this memory area :D
    //no lets do it efficently
    // higher KPD
    // with walking pointer :) :D #flow_order :)
    
    // we use the last_mmap_byte here
    *ivs->wp = irq_ind; //assign interrupt value/number :D :)

    //other way
    //ivs->wp == q+(4064-1)
    // but this way its more resource expensive to know bytes of queue
    //
    // haha izgubih se :D <3
    //
    if(ivs->wp == ivs+LAST_MMAP_BYTE)
    {
        //
        //LAST_MMAP_BYTE here
        // reset wp to beggining of queue (addr)
        ivs->wp = ivs->q->events;
        //void * memset ( void * ptr, int value, size_t num );
        // i think its not needed but idk, suppose yes to not lose data :) #will_see_later_I_must_impl_wa_to_see_ram
        //ways :) <#
        //memory map :)
        //steve jobs is my acid friend :)
        printk(KERN_NOTICE "irqmap_ram: LAST_MMAP_BYTE@%p => %x", ivs->wp, *ivs->wp);
        //oyea
        //
    }
    else
        ivs->wp++; //starts with the first byte of the queue, goto next one :D :)

    // we ready here :)
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

	ivs->irqs |= 1 << irq_ind;

	//local_irq_save(flags);
  	//led_toggle = (0x01 ^ led_toggle);                             // toggle the old value
  	//gpio_set_value(GPIO_21_OUT, led_toggle);                      // toggle the GPIO_21_OUT
	//pr_info("Interrupt Occurred : GPIO_21_OUT : %d ",gpio_get_value(GPIO_21_OUT));
  	//local_irq_restore(flags);

    //userspace interrupt
    //if(ivs->irqh) ivs->irqh();

    zashtiti_queue_add(irq_ind);
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

    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        pr_info("irqmap: fail to register major number! %d", major);
		//printk(KERN_ALERT "irqmap: Registering char device failed with %d\n", major);
		//return major;
        ret = major;
        goto out;
    }

    class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(class)){ 
        unregister_chrdev(major, DEVICE_NAME);
        pr_info("irqmap: failed to register device class");
        ret = PTR_ERR(class);
        goto out;
    }

    device = device_create(class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(device)) {
        class_destroy(class);
        unregister_chrdev(major, DEVICE_NAME);
        ret = PTR_ERR(device);
        goto out;
    }

	/*
	printk(KERN_INFO "irqmap: I was assigned major number %d. To talk to\n", major);
	printk(KERN_INFO "irqmap: the driver, create a dev file with\n");
	printk(KERN_INFO "irqmap: 'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, major);
	printk(KERN_INFO "irqmap: Try various minor numbers. Try to cat and echo to\n");
	printk(KERN_INFO "irqmap: the device file.\n");
	printk(KERN_INFO "irqmap: Remove the device file and module when done.\n");
	*/

	/* init this mmap area */
    // ivs = kmalloc(MMAP_SIZE, GFP_KERNEL);
    ivs = kzalloc(MMAP_SIZE, GFP_KERNEL);
    if (ivs == NULL) {
        ret = -ENOMEM; 
        goto out;
    }

    ivs->rp = ivs->q->events;
    ivs->wp = ivs->q->events;

    //sprintf(ivs, "xyz\n");
	//ivs = kzalloc(sizeof(struct irq_vals), GFP_KERNEL);
	
	// ivs->irq1 |= 1 << 2;
	// ivs->irq1 |= 1 << 3;
	// ivs->irq2 |= 1 << 3;
	// ivs->irq2 |= 1 << 4;
	// ivs->irq3 |= 1 << 1;
	// ivs->irq3 |= 1 << 2;
	// ivs->irq4 |= 1 << 9;
	// ivs->irq4 |= 1 << 10;

    //1ms latency?
    qd = msecs_to_jiffies(1);
    // this will spam a lot
    //pr_info("mykmod loaded %u jiffies\n", (unsigned)onesec);

        if (!wq)
                wq = create_singlethread_workqueue("irqmap_q");
        if (wq)
                queue_delayed_work(wq, &irqmap_work, qd);

    mutex_init(&device_mutex);
out: 
    return ret;
	
	//return 0;
}

static int irqmap_remove(struct platform_device *pdev)
{
	int i=0;

	mutex_destroy(&device_mutex); 
    device_destroy(class, MKDEV(major, 0));
	class_unregister(class);
    class_destroy(class);
	unregister_chrdev(major, DEVICE_NAME);

	for(i=0; i<MAX_IRQS; i++)
	{
		if(irqs[i] == -1)
			continue;
		free_irq(irqs[i], NULL);
	}

	kfree(ivs);
	pr_info("irqmap: unregistered!");
	
	return 0;
}

/*
 * Called when a process tries to open the device file, like
 * "cat /dev/irqmap"
 */
static int device_open(struct inode *inode, struct file *filp)
{
	int ret = 0;

if(!mutex_trylock(&device_mutex)) {
        pr_alert("irqmap: device busy!\n");
        ret = -EBUSY;
        goto out;
    }

pr_info("irqmap: Device opened\n");

  //static int counter = 0;
  //if(Device_Open)
  //  return -EBUSY;
  //Device_Open++;
  
  //see here later
  //sprintf(msg, "%p\n", ivs);
  //msg_Ptr = msg;
  
  //try_module_get(THIS_MODULE);
out:
    return ret;
}

/*
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *filp)
{
  //Device_Open--;

mutex_unlock(&device_mutex);
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
 * Called when a process, which already opened the dev file, attempts to read
 * from it.
 */
static ssize_t device_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
    int ret;
    
    if (len > MMAP_SIZE) {
        pr_info("irqmap: read overflow!\n");
        ret = -EFAULT;
        goto out;
    }

    if (copy_to_user(buffer, ivs, len) == 0) {
        pr_info("irqmap: copy %u char to the user\n", len);
        ret = len;
    } else {
        ret =  -EFAULT;   
    } 

out:
    return ret;
}

static ssize_t device_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
    int ret;
 
    if (copy_from_user(ivs, buffer, len)) {
        pr_err("irqmap: write fault!\n");
        ret = -EFAULT;
        goto out;
    }
    pr_info("irqmap: copy %d char from the user\n", len);
    ret = len;

out:
    return ret;
}

static int device_mmap(struct file *filp, struct vm_area_struct *vma)
{
    int ret = 0;
    struct page *page = NULL;
    unsigned long size = (unsigned long)(vma->vm_end - vma->vm_start);

    if (size > MMAP_SIZE) {
        ret = -EINVAL;
        goto out;  
    } 
   
    page = virt_to_page((unsigned long)ivs + (vma->vm_pgoff << PAGE_SHIFT)); 
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