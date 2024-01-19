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

``` 
./fetmes --multimeter=<ip of your multimeter> -mmport=<port used by your multimeter> \
--powersupply-serial=<path to the powersupply serial devicee (e.g. /dev/ttACM0)> \ 
--pwbaudrate=<power supply serial baudrate 9600/115200>
``` 

Alternatively if your power supply uses TCP, you may use `--powersuppy=<ip> --psport=<port>` and vice versa.
There's also help list if you have any doubts:

```
./fetmes --help
```

## API

See: src/cmds.h
