# Task2_Uboot_Qemu

## U-boot setup with emulated sdcard and Qemu emulator 

- In this README.md I provide steps to use U-boot and set the configuration of it
and generate emulated sdcard and at the end I will use QEMU emulator to see the output.

### First Step: Generate Emulated `Sdcard`

#### 1- Create a directory for the sdcard to put the sd.img in it 
```
mkdir sdcard
```
#### 2- Create file 
```
cd sdcard
```
```
touch sd.img
```
#### 3- Create this file with size 1GB
  - dd: A low-level command used for copying and converting data
  - if: input file 
  - of: output file
  - bs: block size
  - count: how many blocks will generate in the sd.img  
```
dd if=/dev/zero of=sd.img bs=1M count=1024
```
- Put into sd.img file with 1GB of zero characters (@)

#### 4- Creat partition in SD card
```
cfdisk sd.img
```
- cfdisk: Creat header of the memory (MBR) 
  - Select DOS partition table.
     - Create:
        - 200MB Primary Partition (set as bootable, type FAT16).
        - Extended Partition for the remaining space (type EXT4).

![alt text](<Steps_Screenshot/1.png>)

#### 5- Link the SD image with loop devices
```
sudo losetup --partscan -f --show sdcard/sd.imag
```
- This will link the sd.img with loop devices (ex: /dev/loop6)

![alt text](<Steps_Screenshot/2.png>)
#### 6- Create mount points 
- create two directories for boot and root filesystem 
```
mkdir boot rootfs
```

![alt text](<Steps_Screenshot/3.png>)
#### 7- Partitions formating
```
sudo mkfs.vfat -F 16 -n boot /dev/loop6p1
```
- Explanation:
  * mkfs.vfat → Creates a FAT filesystem (typically used for USB drives, SD cards, and boot partitions).
  * -F 16 → Specifies FAT16 as the filesystem type.
  * -n boot → Assigns the volume label boot to the partition.
  * /dev/loop6p1 → The target partition (first partition on the loop device /dev/loop6).
```
sudou mkfs.ext4  -L rootfs /dev/loop6p2
```
- Explanation:
  * mkfs.ext4 → Formats the target partition as an ext4 filesystem (commonly used for Linux root filesystems).
  * -L rootfs → Sets the volume label to rootfs (a typical name for root filesystems).
  * /dev/loop6p2 → Specifies the second partition on the loop device /dev/loop6 as the target.

#### 8- Mount partitions
```
sudo mount /dev/loop6p1  sdcard/boot
```
```
sudo mount /dev/loop6p2  sdcard/rootfs
```
#### 9- Check mount points
- Lists block devices and verifies the mounted partitions.
```
lsblk
```
![alt text](<Steps_Screenshot/4.png>)

#### 10- Add boot content 
- Go to the `boot` directory 
```
cd  boot
```
- Then creat a `zImage` file (fake file)
```
touch zImage
```
- write anything inside this file using `vim` 
```
sudo vim zImage 
```
![alt text](<Steps_Screenshot/5.png>)

- retun back to the `sdcard` directory
```
cd ..
```
- umont boot and rootfs directories
```
sudo umount boot
```
```
sudo umount rootfs
```
![alt text](<Steps_Screenshot/6.png>)
![alt text](<Steps_Screenshot/7.png>)
#### 11- Sync the filesystem
- Ensures all data is written to the SD image.
```
sync 
```

### Second Step:Clone and Build `U-Boot` 

#### 1- Clone U-boot
 - clone the latest version 
```
git clone git@github.com:u-boot/u-boot.git
```
 - checkout to the latest `tag` 
```
cd u-boot
```
```
git chechout v2025.01
```
#### 2- Make configuration based on board 
- Go to configs directory 
```
cd u-boot/configs
```
- Search for specific target 
```
ls | grep vex
```
- make new configuration
```
make vexpress_ca9x4_defconfig
```
![alt text](<Steps_Screenshot/8.png>)
- Applies the default configuration for vexpress_ca9x4.
```
make menuconfig
```
* Navigate to Environment settings.
   * Enable FAT filesystem.
   * Set the block device, partition, and environment:
   * Block device: mmc
   * Partition: 0:1 (FAT partition).

![alt text](<Steps_Screenshot/9.png>)
#### 3- Set up the `cross-compiler`
- Search for specific variable  
```
cat Makefile | grep CROSS_COMPILE
```
- Make a global variable to set cross compiler
```
export CROSS_COMPILE=~/x-tools/arm-omaralaa-linux-gnueabihf/bin/arm-omaralaa-linux-gnueabihf-
```
- Ensures the correct toolchain is used for ARM.

![alt text](<Steps_Screenshot/10.png>)

- Build `U-boot`
```
make -j12
```
### Third Step:Using QEMU emulator 
#### 1- Open QEMU
- Search for specific target 
```
qemu-system-arm -M ? | grep vex*
```
- Select the same target that used with `U-boot`
```
qemu-system-arm -M vexpress-a9 -kernel u-boot -nographic -sd ~/sdcard/sd.img
```
- Explanation:
   * qemu-system-arm → Runs QEMU for ARM architectures.
   * -M vexpress-a9 → Specifies the machine type as VExpress A9, commonly used for ARM Cortex-A9 emulation.
   * -kernel u-boot → Loads U-Boot as the kernel (bootloader for embedded Linux systems).
   * -nographic → Runs without a graphical display (uses serial console output).
   * -sd ~/sdcard/sd.img → Uses sd.img as a virtual SD card (often containing the root filesystem).

- Starts QEMU with the prepared u-boot and SD card.

![alt text](<Steps_Screenshot/11.png>)

#### 2- Interact with U-Boot
- List FAT partition contents:
```
ls mmc 0:1
```
- Get DRAM address:
```
bdinfo
```
- Load zImage into DRAM:
```
fatload mmc 0:1 0x60000000 zImage
```
- Verify zImage in memory:
```
md 0x60000000
```
![Final Result.png](<Final Result.png>)









