# Linux Command Line Tools

## 0. Linux Basic

### Linux Dictionaries

| Name     | Usage                                                                                                      |
| -------- | ---------------------------------------------------------------------------------------------------------- |
| `/proc`  | Not a real file system, but a virtual one. It is maintain by linux kernel and can communicate with kernel. |
| `/media` | contains the mount point of removable devices. They are mounted automatically at insertion.                |


## 1. File & Navigation

### View dictionary & File

!!! tip "TakeAway"
    ```shell
    pwd   #Print name of current working directory
    cd    #Change directory
    ls    #List directory contents
    head  #Show first 10 line of a file
    tail  #Show last 10 line of a file
    ```

🔘 Use `pwd` to get current dictionary

`pwd` stands for **print working dictionary**.

```shell
$pwd                                 
/home/oysterpus
```

🔘 Use `head`, `tail` to view part of the file

By default, head/tail output 10 lines. Line

🔘 Use `more`, `less`

Once the `less` program starts, we can view the contents of the file. If the file is longer than one page, we can scroll up and down. To exit less, press `q`.


### Create & Modify File/Dictionary

!!! tip "TakeAway"
    ```shell
    touch #Create new file
    shred #Hide file
    nano  #Edit file
    ```

<!-- About File -->
🔘 Use `touch` to create a new file

```shell
touch filename
touch name1 name2 name3 name4 # every name create a single file
touch name{1..10}             # create file with continuous number
touch -d tomorrow filename    # -d to specify file create date
```

🔘 Use `shred` to overwrite and hide a file

```shell
shred filename
```

🔘 Use `nano` and `vim` to edit a file

---

<!-- About Dictionary -->

🔘 Use `mkdir` to create a new dictionary


### Compress File

🔘 Use `zip` to compress file
🔘 Use `unzip` to decompress file

```shell
zip zip_name filename
uzip zip_name 
```

## 2. What Is Running And How To Monitor/Kill

!!! tip "TakeAway"
    ```shell
    ps       #Report a snapshot of current processes
    top      #Display tasks
    jobs     #List active jobs
    bg       #Place a job in the background
    fg       #Place a job in the foreground
    kill     #Send a signal to a process
    killall  #Kill processes by name
    shutdown #Shut down or reboot the system
    ```

When a system starts up, the kernel initiates a few of its own activities as  processes and launches a program called `init`.

`init` returns a series of shell scripts called ***init scripts***.

A program can launch other programs is expressed in the  process scheme as a **parent process** producing a **child process**.

Each process is assigned a number called
**a process ID (PID**).

### Viewing Processes

🔘 Use `ps`

```shell
# ps
  PID TTY          TIME CMD
 5694 pts/6    00:00:00 sudo
 5695 pts/6    00:00:00 go
 5814 pts/6    00:00:00 UTS
 5818 pts/6    00:00:00 sh
10120 pts/6    00:00:00 ps
```

- `TTY` is short for “teletype”  and refers to the **controlling terminal** for the process.
- The `TIME` field is the amount of CPU time consumed by the process.


🔘 Use `pstree`
Outputs a process list arranged in a tree-like pattern showing the parent-child relationships between processes.

```shell
init(Ubuntu-22.(1)─┬─SessionLeader(333)─┬─Relay(335)(334)───docker-desktop-(335)─┬─{docker-desktop-}(336)
                   │                    │                                        ├─{docker-desktop-}(337)
                   │                    │                                        ├─{docker-desktop-}(338)
                   │                    │                                        ├─{docker-desktop-}(339)
                   │                    │                                        ├─{docker-desktop-}(340)
(367)
                   │                    └─Relay(352)(351)───docker(352)─┬─{docker}(353)
                   │                                                    ├─{docker}(354)
                   │                                                    ├─{docker}(355)
                   │                                                    ├─{docker}(356)
                   │                    
                   ├─SessionLeader(390)───Relay(392)(391)─┬─sh(392)───sh(393)───sh(418)───node(422)─┬─node(433)─┬─zsh(3366)───sudo(5693)───sudo(5694)───go(5695)─┬─UTS(5814)─┬─sh(5818)───pstree(5969)
                   │                            
```

🔘 Use `top`

The top program displays a **continuously updating** (by default, every 3 seconds) display of the system processes listed in order of process activity.

### Multiple processes

多进程间通信常用的技术手段包括共享内存、消息队列、信号量.

🔘 Use `ipcs`
　　
- `ipcs -m`　　查看系统共享内存信息
- `ipcs -q`　　查看系统消息队列信息
- `ipcs -s`　　查看系统信号量信息,
- `ipcs [-a]`　系统默认输出信息，显示系统内所有的IPC信息

```shell
# ipcs
------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    

------ Shared Memory Segments --------
key        shmid      owner      perms      bytes      nattch     status      

------ Semaphore Arrays --------
key        semid      owner      perms      nsems
```

🔘 Use `ipcrm`
通过指定ID(`[msq/shm/sem][id/key]`)删除IPC资源，同时将与IPC对象关联的数据一并删除，只有超级用户或IPC资源创建者能够删除。

## 3. Know My System and Get Help
<!-- System Info & State & Help Message -->

### What and Where

🔘 Use `man` to get help message from tools

🔘 Use `whatis` to get a shorter version of `man`

🔘 Use `apropos` to find what tools match your need.

```shell
apropos -a change user                                    
chage (1)            - change user password expiry information
chfn (1)             - change real user name and information
passwd (1)           - change user password
ucf (1)              - Update Configuration File: preserve user changes in configuration files
...
```

## 4. Input/Output, then Redirect

!!! tips "TakeAway"
    ```shell
    cat  #Concatenate files
    sort #Sort lines of text
    uniq #Report or omit repeated lines
    grep #Print lines matching a pattern
    wc   #Print newline, word, and byte counts for each file
    head #Output the first part of a file
    tail #Output the last part of a file
    tee  #Read from standard input and write to standard output and files
    ```

!!! note "Standard Output/Input"
    - Programs such as `ls` actually send their results to a **special file** called standard output (often expressed as `stdout`) and their status messages to another file called standard error (`stderr`).
    - `stdin` default attaches to keyboard

### Reading Files

🔘 Use `cat` to concatenate file

The cat command reads one or more files and copies them to standard output.

```shell
cat filename
cat movie.mpeg.0* > movie.mpeg # join all split media file together
```

### Redirection

🔘 Use `>` and `>>` to redirect output 

`>` **rewrite** a file, while `>>` append the content to the end.

```shell
ls -l /usr/bin > ls-output.txt
```

🔘 Use `2>` to redirect error

```shell
ls -l /bin/usr 2> ls-error.txt
```


🔘 Use `|` to pipeline 2 commands

Read data from standard input and send to
standard output.

```shell
command1 | command2
```

## 5. How Disk Store Media

!!! tip "Takeaway"
    ```shell
    mount                #Mount a file system
    umount               #Unmount a file system
    fsck                 #Check and repair a file system
    fdisk                #Manipulate disk partition table
    mkfs                 #Create a file system
    dd                   #Convert and copy a file
    genisoimage(mkisofs) #Create an ISO 9660 image file
    wodim(cdrecord)      #Write data to optical storage media
    md5sum               #Calculate an MD5 checksum
    ```

### Mounting and Unmounting Storage Devices

- **Mounting**: managing a storage device is **attaching the device** to the **file system tree**.

A file named `/etc/fstab` (short for “file system table”) lists the devices(typically hard disk partitions) that are to be mounted at **boot time**.

`/etc/fstab` contains 6 field:

- `Device`: Actual name of a device
- `Mount point`: The directory where the device is attached to
- `File system type`: Fourth Extended File System(ext4), FAT16(msdos), FAT32 (vfat), NTFS (ntfs), CD-ROM (iso9660)
- `Options`: read-only, gid ...
- `Frequency`: backed up action when `dump` command
- `Order`: checked with the `fsck` command

```shell
LABEL=/12       /        ext4   defaults       1 1
LABEL=/home     /home    ext4   defaults       1 2
LABEL=/boot     /boot    ext4   defaults       1 2
tmpfs           /dev/shm tmpfs  defaults       0 0
devpts          /dev/pts devpts gid=5,mode=620 0 0
sysfs           /sys     sysfs  defaults       0 0
proc            /proc    proc   defaults       0 0
LABEL=SWAP-sda3 swap     swap   defaults       0 0
```

🔘 Use `mount`

`mount` command is used to view the file system that currently mounted.

```shell
> mount                         
none on /mnt/wsl type tmpfs (rw,relatime)
none on /usr/lib/wsl/drivers type 9p (ro,nosuid,nodev,noatime,dirsync,aname=drivers;fmask=222;dmask=222,mmap,access=client,msize=65536,trans=fd,rfd=7,wfd=7)
none on /usr/lib/wsl/lib type overlay (rw,relatime,lowerdir=/gpu_lib_packaged:/gpu_lib_inbox,upperdir=/gpu_lib/rw/upper,workdir=/gpu_lib/rw/work)
/dev/sdc on / type ext4 (rw,relatime,discard,errors=remount-ro,data=ordered)
```

The format of the listing is as follows:

- `device` on `mount_point` type `filesystem_type` (options).


🔘 Use `umount` & `mount` to change mount point

```shell
umount /dev/sdc
mkdir /mnt/cdrom
mount -t iso9660 /dev/sdc /mnt/cdrom
```

Use `-t` to specify the file system type, followed by `[src] [dst]`.

### Creating New File Systems


### Testing and Repairing File Systems

### Moving Data Directly to and from Devices

## 7. Network

!!! tip "Takeaway"
    ```shell
    ping        #Send an ICMP ECHO_REQUEST to network hosts
    traceroute  #Print the route packets trace to a network host
    ip          #Show/manipulate routing, devices, policy routing, and tunnels
    netstat     #Print network connections, routing tables, interface statistics, masquerade connections, and multicast memberships
    ftp         #Internet file transfer program
    wget        #Non-interactive network downloader
    ssh         #OpenSSH SSH client (remote login program)
    ```

### Examining and Monitoring a Network

🔘 Use `ping`

🔘 Use `traceroute`

🔘 Use `ip`(`ifconfig`)

`ip` is the newer version of `ifconfig`.

With ip, we can examine a system’s **network interfaces** and **routing table**.

```shell
#ip a             
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
    inet6 ::1/128 scope host 
       valid_lft forever preferred_lft forever
2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1472 qdisc mq state UP group default qlen 1000
    link/ether 00:15:5d:e9:80:d4 brd ff:ff:ff:ff:ff:ff
    inet 172.20.209.32/20 brd 172.20.223.255 scope global eth0
       valid_lft forever preferred_lft forever
    inet6 fe80::215:5dff:fee9:80d4/64 scope link 
       valid_lft forever preferred_lft forever
```

- 2 interfaces: `lo` is loopback interface,`eth0` is the Ethernet interface.
- In the first line of each interface, the `UP` words indicates the network interface is enabled.
- A valid IP address verifies that the DHCP is working.

### Transporting Files over a Network(`ftp`, `wget`, `curl`)

🔘 Use `ftp` to transfer file via File Transfer Protocol

URIs starting with the protocol `ftp://.`

🔘 Use `curl` to download a file from an url and redirect to another filename

### Secure Communication with Remote Hosts(`ssh`)

🔘 Use `ssh`

SSH consists of two parts. An SSH server runs on the remote host, listening for incoming connections on port 22, while an SSH client is used on the local system to communicate with the remote server.

- It **authenticates** that the remote host is who it says it is (thus preventing so-called MITM attacks).
- It **encrypts** all of the communications between the local and remote hosts.


```shell
$ ssh username@1.1.1.1
```

## 8. How AM I? What Can I DO?

!!! tip "Takeaway"
    ```Shell
    id        #Display user identity
    chmod     #Change a file’s mode
    umask     #Set the default file permissions
    su        #Run a shell as another user
    sudo      #Execute a command as another user
    chown     #Change a file’s owner
    chgrp     #Change a file’s group ownership
    passwd    #Change a user’s password
    ```

### Owners, Group Members, and Everybody Else

🔘 Use `id`
Find out identity. Include 3 fields: `uid`, `gid`, `groups`.

```shell
uid=1000(oysterpus) gid=1000(oysterpus) groups=1000(oysterpus),4(adm),20(dialout),24(cdrom),25(floppy),27(sudo),29(audio),30(dip),44(video),46(plugdev),116(netdev),1001(docker)
```

User accounts are defined in the `/etc/passwd` file, and groups are defined in the `/etc/group` file.


## 9. Miscellaneous
