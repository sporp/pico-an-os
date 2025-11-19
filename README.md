# PICO-AN-OS

Hello hello,

This is a very silly way to make a Rasperry Pi Pico into a Keyboard Macro to boot into your side OS.

![Outie](img/outie.jpg)

## OVERVIEW

On plugging in the device, or after it has been plugged in and you press the button that shorts pins RUN and GND, it will proceed to do the following:
1) Hammer F2 for about 30 seconds. 
2) Hit F8
3) Hit the down arrow key 3 times
4) Hit enter

Viola. Instead of booting into my beloved, Linux, we are trapped in a room without doors. Only Windows. At present for playing games that require Kernal Anti-Cheat and for Video Editing.

Feel free to edit the "main" funciton "main.c" in /pico_f2_keeb to your requirements. The "meat" of this program is line 71 to 127

## MATERIALS
1) Raspbery Pi Pico
2) USB-A to USB micro adapter or cable (male to male, AKA hole to hole NOT pole to pole)
3) OPTIONAL: A keyboard switch, some wire, soldering iron and solder (if you want to add the optional keyboard switch to active it. Just plugging it in when you hit power on your computer works just fine)

## DEPENDENCIES
```
sudo apt update
sudo apt install -y git cmake build-essential gcc-arm-none-eabi libnewlib-arm-none-eabi
```

## INSTALLATION STEPS
```
git clone git@github.com:sporp/pico-an-os.git
cd pico-an-os
git submodule update --init --recursive
export PICO_SDK_PATH="$(pwd)/pico-sdk"
```

If you don't want to add the PICO_SDK_PATH each time, add it to your .bashrc or .zshrc

## BUILD AND UPLOAD
1) Make your way downtown to directory pico_f2_keeb/build , cmake and make by following these steps:
```
cd pico_f2_keeb
rm -rf build
mkdir build
cmake ..
make -j4
```
2) Hold the "bootsel" button down on your Pico as you plug it into your computer. It should pop up as a new drive.
3) Drag and drop "pico_f2_keyboard.uf2" onto your Pico.

### 3D MODEL MADE IN OPENSCAD

There's an STL file for holding together:
1) the USB-A to Micro-USB adapter (that I have bent at a right angle)
2) the Pico
3) a keyboard switch

It's a fun little dongle to dangle out the front of your computer!

![Dongle Danglin'](img/innie.jpg)
