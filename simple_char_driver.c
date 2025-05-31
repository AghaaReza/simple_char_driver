#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>  // for copy_to_user and copy_from_user

#define DEVICE_NAME "simple_char"
#define BUFFER_LEN 80

static int major;
static char message[BUFFER_LEN];
static int message_len = 0;

// Function prototypes
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char __user *, size_t, loff_t *);

// File operations structure
static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

// Called when the device is opened
static int device_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "simple_char: Device opened\n");
    return 0;
}

// Called when the device is closed
static int device_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "simple_char: Device closed\n");
    return 0;
}

// Called when the device is read from user space
static ssize_t device_read(struct file *filp, char __user *buffer, size_t length, loff_t *offset) {
    int bytes_read = 0;

    if (*offset >= message_len)
        return 0;

    while (length && *offset < message_len) {
        put_user(message[*offset], buffer++);
        length--;
        (*offset)++;
        bytes_read++;
    }

    printk(KERN_INFO "simple_char: Read %d bytes\n", bytes_read);
    return bytes_read;
}

// Called when the device is written to from user space
static ssize_t device_write(struct file *filp, const char __user *buffer, size_t length, loff_t *offset) {
    int i;

    for (i = 0; i < length && i < BUFFER_LEN - 1; i++) {
        get_user(message[i], buffer + i);
    }

    message[i] = '\0';
    message_len = i;

    printk(KERN_INFO "simple_char: Received %s\n", message);
    return length;
}

// Called when the module is loaded
static int __init simple_char_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if (major < 0) {
        printk(KERN_ALERT "simple_char: Failed to register device: %d\n", major);
        return major;
    }

    printk(KERN_INFO "simple_char: Registered with major number %d\n", major);
    return 0;
}

// Called when the module is unloaded
static void __exit simple_char_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "simple_char: Unregistered device\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Reza Esmaeilzadeh");
MODULE_DESCRIPTION("A basic Linux character device driver");

module_init(simple_char_init);
module_exit(simple_char_exit);
