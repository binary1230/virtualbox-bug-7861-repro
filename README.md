# virtualbox-bug-7861-repro
Uses vagrant to repro a vboxfs bug in https://www.virtualbox.org/ticket/8761

# instructions

Install Vagrant, Virtualbox, etc.

clone this repo somewhere

recommend running the vagrant virtualbox guest additions auto-updater
```vagrant plugin install vagrant-vbguest```

```vagrant box update```

```vagrant up```

follow the instructions from there to test.


# notes

There are 2 repros, bug-demo.c and bin/bug-demo.sh
They do the same thing, but bug-demo.cs is trying to do it in as few syscalls as possible.
It appears that the rename() syscall is triggering the bad behavior.
