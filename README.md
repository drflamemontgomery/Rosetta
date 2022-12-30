# Rosetta
A Raspberry Pico project for converting HID devices into Controllers

# Requirements

pico-sdk <a>https://github.com/raspberrypi/pico-sdk</a>

# Installation

``` sh
git clone git@github.com:drflamemontgomery/Rosetta.git --recurse-submodules
cd Rosetta
mkdir build
cd build
cmake ..
make
# Copy UF2 File to Rpi Pico
```

# Usage

Connect D+/D- of 2 usb shields to gp2/gp3 and gp6/gp7 then hook power and ground to the shields
