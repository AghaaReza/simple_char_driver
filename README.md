# simple_char_driver
A minimal Linux kernel module that implements a character device driver. It supports basic read() and write() operations between user space and kernel space.



**Features**

- Dynamically registers a character device with a major number
- Supports open, close, read, and write operations
- Stores messages in a simple in-kernel buffer
- Logs actions to the kernel log (dmesg)
- Fully self-contained and minimal — great for learning or extending

**Build Instructions**

Ensure you have the kernel headers installed. Then:

`make`

**Usage**

1. Insert the kernel module:

`sudo insmod simple_char_driver.ko`

2. Check the assigned major number:

`dmesg | grep "simple_char"`

3. Create the device file:

```
sudo mknod /dev/simple_char c <major_number> 0
sudo chmod 666 /dev/simple_char
```

4. Interact with the device:

```
echo "Hello Kernel" > /dev/simple_char
cat /dev/simple_char
```

5. Remove the module:

`sudo rmmod simple_char_driver`

**Build the module**

Insert the module into the kernel
`sudo insmod simple_char_driver.ko`

Check kernel logs
`dmesg | tail`

Create the device file (replace <major> with the actual number from dmesg)
`sudo mknod /dev/simple_char c <major> 0`

Set permissions
`sudo chmod 666 /dev/simple_char`

Write to device
`echo "Hello Driver" > /dev/simple_char`

Read from device
`cat /dev/simple_char`

Remove the module
`sudo rmmod simple_char_driver`

**Disclaimer**

Tested on recent versions of Ubuntu with a 5.x Linux kernel.
Requires root privileges to insert/remove modules.
For educational purposes — no production guarantees.


**Author**

Developed by Reza Esmaeilzadeh 
Licensed under the GPL v2.
