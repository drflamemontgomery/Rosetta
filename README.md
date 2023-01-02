# Rosetta
The Rosetta project acts as a hardware translator between HID devices and game consoles or PCs. The Rosetta can allow HID devices not specifically designed to work with a console to be translated so that the console recognises the HID device as a valid device. The Rosetta can also be used to remap button layouts, and even create hybrid controllers. For example, eight different controllers can have one button each mapped to the controller the console sees. Therefore a single player game, such as Super Mario, can be played by multiple people working together.

The Rosetta version A1 is based on the RP2040 Raspberry Pi Pico microcontroller. It uses the built in USB port as the HID device port which plugs into the console/PC and it uses the Pico PIO USB library to implement upstream USB host ports. 

The Pico PIO USB library has been forked and modified to fix USB host functionality and add XPAD support. The aim is to merge these back into the main repository soon.

The Rosetta will also implement bluetooth HID functionality through an ESP32 module. The ESP32 was chosen because it implements both bluetooth BLE and bluetooth classic which is required by some older controllers and HID devices.

## State of the project
The Rosetta is currently under development with the base version being the priority. Bluetooth support for the *Rosetta +* will be developed as the secondary goal.

## Future Goals
* Complete alpha release of the base Rosetta software
* Start development on the *Rosetta +* bluetooth drivers
* Add WebHID interface to base Rosetta for creating controller configuration files

## Hardware
The first PCB, Rosetta A1, designed for developing the base Rosetta has been sent for manufacture. The gerber files and schematics will be uploaded soon. The A1 PCB contains a 1x PCIe connector that breaks out some of the pins and buses from teh RP2040. This will allow the development of a daughter board for the ESP32 and debugging ports to aid in the future development of the *Rosetta +*.


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

Connect D+/D- of a usb shield to GPIO2/GPIO3 and hook 5V power and ground to the shield. A second USB shield on GPIO6/GPIO7 is also supported. Schematic coming soon ...
