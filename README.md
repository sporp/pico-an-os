Hello hello,

This is a very silly way to make a rasberry pi pico into a macro to boot into your alternate OS.

![Outie](outie.jpg)

On plugging in the device, or after it has been plugged in and you press the button that shorts pins RUN and GND, it will proceed to do the following:
1) Hammer F2 for about 30 seconds. 
2) Hit F8
3) Hit the down arrow key 3 times
4) Hit enter

Viola. Instead of booting into my beloved, Linux, we are trapped in a room without doors. Only Windows. At present for playing games that require Kernal Anti-Cheat and for Video Editing.

Feel free to edit the "main" funciton "main.c" in /pico_f2_keeb to your requirements.

Dependencies:
```
sudo apt update
sudo apt install -y git cmake build-essential gcc-arm-none-eabi libnewlib-arm-none-eabi
```

Materials:
1) Raspbery Pi Pico
2) Usb-A to Usb micro adapter or cable (male to male)
3) OPTIONAL: A keyboard switch, some wire, soldering iron and solder (if you want to add the optional keyboard switch to active it. Just plugging it in when you hit power on your computer works just fine)

To Build:
1) Make sure you have a path variable that points to the pico sdk in this repo, or your MAKE step is not going to work. If the repo was cloned to your user directory, add this to your .bashrc or .zshrc:
```
export PICO_SDK_PATH="~/pico-an-os/pico-sdk"
```
2) Make your way downtown to directory pico_f2_keeb/build , cmake and make:
```
cd pico_f2_keeb/build
cmake ..
make -j4
```
5) Hold the "bootsel" button down on your pico as you plug it into your computer. It should pop up as a new drive.
6) Drag and drop "pico_f2_keyboard.uf2" onto your newly present pico.

There's an STL file for holding together the usb-a to micro-usb adapter (that I have bent at a right angle), the pic and a keyboard switch. It's a fun little dongle to dangle out the front of your computer!

![Dongle Danglin'](innie.jpg)
