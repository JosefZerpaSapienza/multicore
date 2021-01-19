#!/bin/bash
#
# Setup script file to setup the vagrant-mpi environment.
#
#

#install mpi
sudo apt install mpich -y
#install vagrant from deb file
sudo apt install ./vagrant_2.2.14_x86_64.deb -y
#run vagrant setup file
vagrant up

#done
echo -e '\n\n\nDone! :D'
