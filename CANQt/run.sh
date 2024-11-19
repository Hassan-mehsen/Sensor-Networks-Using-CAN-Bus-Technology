#!/bin/bash


sudo ip link set can0 up type can bitrate 500000 
make
./CANQt
echo "Let' go!"
candump can0





