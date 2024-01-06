# Automatic MOSFET measurement system using remote controlled lab equipment

(WIP)

## Hardware prerequisites:

The app requires:

- a programmable DC power supply,

- a programmable multimeter, capable of measuring resistance.

Each of the devices has to have a communication protocol compliant with the IEE-488.3 standard.

This software is being tested on:

- KORAD KA3005P programmable DC power supply,

- SDM3055 programmable multimeter.

## Building and running 

```
mkdir build

cd build 

cmake ..

make -j`nproc`
```
