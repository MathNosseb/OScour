#!/bin/bash

read -p "Tu vas écrire dans SDA (DANGEREUX), <yes>  pour confirmer : " conf

if [ "$conf" = "yes" ]; then
    sudo parted /dev/sda --script mklabel msdos
    sudo parted /dev/sda --script mkpart primary 0% 100%
    sudo parted /dev/sda --script set 1 boot on
    sudo dd if=Binaries/os.bin of=/dev/sda bs=446 count=1 conv=notrunc
    sudo dd if=Binaries/os.bin of=/dev/sda bs=512 skip=1 seek=1 conv=notrunc
    sudo sync
fi