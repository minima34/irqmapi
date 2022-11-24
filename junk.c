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


//#include <linux/kernel.h>
//#include <linux/cdev.h>
/*
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/init.h> 
#include <linux/fs.h> 
*/
/*
#include <asm/uaccess.h>
*/

/*
static struct file_operations fops = {
  .read = ,
  .write = ,
  .open = ,
  .release = ,
};
*/

//ives=0+3_last_byte_(4-1)==3
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

/*
** This function will be called when app calls the poll function
*/
// static unsigned int etx_poll(struct file *filp, struct poll_table_struct *wait)
// {
//   __poll_t mask = 0;
// poll_wait(filp, &irqmap_queue, wait);
//   pr_info("irqmap: poll function\n");
//   //if( can_read )
//   //{
//     //can_read = false;
// mask |= ( POLLIN | POLLRDNORM );
//   //}
//   //if( can_write )
//   //{
//   //  can_write = false;
//   //  mask |= ( POLLOUT | POLLWRNORM );
//   //}
//   return mask;
// }

/*
static unsigned int
irqmap_poll(struct file *filp, poll_table *wait)
{
  // hihihiiiii Scull_Pipe *dev = filp->private_data;
  unsigned int mask = 0;

//    *
//    * The buffer is circular; it is considered full
//    * if "wp" is right behind "rp". "left" is 0 if the
//    * buffer is empty, and it is "1" if it is completely full.
//    *
  //int left = (dev->rp + dev->buffersize - dev->wp) % dev->buffersize;

  poll_wait(filp, &dev->inq, wait);
    poll_wait(filp, &dev->outq, wait);

  if (dev->rp != dev->wp) mask |= POLLIN | POLLRDNORM; // readable
    if (left != 1)     mask |= POLLOUT | POLLWRNORM; // writable

  return mask;
}
*/