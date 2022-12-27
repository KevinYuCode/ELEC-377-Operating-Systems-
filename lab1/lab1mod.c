#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/ktime.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

static int lab1_show(struct seq_file *m, void *v)
{
  /* some code here */


  //Displays the current running process' properties from the PCB
  seq_printf(m, "Current Process PCB Information\n");
  seq_printf(m, "Name = %s\n", current->comm);
  seq_printf(m, "PID = %d\n", current->pid);
  seq_printf(m, "PPID = %d\n", current->parent->pid);

  //Checks the process' state and conditionally displays the state of the process
  if (current->state == TASK_RUNNING)
  {
    seq_printf(m, "State = Running\n");
  }
  else if (current->state == TASK_INTERRUPTIBLE || current->state == TASK_UNINTERRUPTIBLE)
  {
    seq_printf(m, "State = Waiting\n");
  }
  else if (current->state == TASK_STOPPED)
  {
    seq_printf(m, "State = Stopped\n");
  }

  seq_printf(m, "Real UID = %d\n", current->cred->uid.val);
  seq_printf(m, "Effective UID = %d\n", current->cred->euid.val);
  seq_printf(m, "Saved UID = %d\n", current->cred->suid.val);
  seq_printf(m, "Real GID = %d\n", current->cred->gid.val);
  seq_printf(m, "Effective GID = %d\n", current->cred->egid.val);
  seq_printf(m, "Saved GID = %d\n", current->cred->sgid.val);

  return 0;
}

//Used by lab1_fops to call lab1_show
static int lab1_open(struct inode *inode, struct file *file)
{
  return single_open(file, lab1_show, NULL);
}

//Used by lab1_init when the proc file is created to assign the SEQ API commands 
#ifdef HAVE_PROC_OPS
static const struct proc_ops lab1_fops = {
    /* operation mapping */
    .proc_open = lab1_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};
#else
static const struct file_operations lab1_fops = {
    /* operation mapping */
    .owner = THIS_MODULE,
    .open = lab1_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};
#endif

//Creates the proc file and checks to see if there is enough memory to create the file
static int __init lab1_init(void)
{
  /* create proc entry */
  if (proc_create("lab1", 0, NULL, &lab1_fops) == NULL)
  {
    return -ENOMEM;
  }
  printk(KERN_INFO "lab1mod in\n");
  return 0;
}

//Removes the proc file from the PCB
static void __exit lab1_exit(void)
{
  /* remove proc entry */
  remove_proc_entry("lab1", NULL);
  printk(KERN_INFO "lab1mod out\n");
}


//Initializes the module license
MODULE_LICENSE("GPL");
//Calls the module initialization to create the proc file 
module_init(lab1_init);
//Removes the module and deletes the proc file  
module_exit(lab1_exit);
