#!/bin/bash

export DEBIAN_FRONTEND=noninteractive

sudo apt-get -y update
sudo apt-get -y upgrade
sudo apt-get -y install gcc

cd /mnt/host
./build.sh

echo 'PATH=$PATH:/mnt/host/bin/' >> /home/vagrant/.bashrc

echo "Test environment setup complete.  Please run the following:"
echo "vagrant ssh"
echo "cd /mnt/host/"
echo "run-bug-demo.sh"
echo "cd /tmp/"
echo "run-bug-demo.sh"
echo ""
echo "Observe that it works on the native filesystem (ext4) but fails on the shared fs (vboxfs)"
