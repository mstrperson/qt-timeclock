# SignIn Pi

*RFID Card* or *PIN* enabled timeclock software written for the Dublin School FIRST Robotics Team to keep track of team attendance during the robotics season.
Written by Jason Cox

## Hardware

* Adafruit NFC/RFID reader module
* RaspberryPi 2
* 7" Touch-screen
* USB-FTDI module

## Software
Built and compiled on the RaspberryPi so no cross compiling was needed.
Copy the libnfc directory into your /home/pi directory because the code calls the modified nfc-poll example.

* Adafruit NFC/RFID driver for Raspbian (wheezy) [libnfc](https://github.com/nfc-tools/libnfc)
* Qt 4.8
* MySql

## Qt Build Configuration
### Build Steps
* **qmake**: qmake-qt4 Signin.pro -r -spec linux-g++
* **Make**: make -j8 -w in /home/pi/autorun
### Tool Chains
* **Compiler path**: /usr/bin/g++
* **ABI**: arm-linux-generic-elf-32bit
* **Debugger**: /usr/bin/gdb

## Raspbian Configuration

Auto run the Signin application on LXDE startup.  This will automatically restart the program if it is somehow closed by an end-user.

```sh
# /etc/xdg/lxsession/LXDE-pi/autostart

@lxpanel --profile LXDE-pi
@pcmanfm --desktop --profile LXDE-pi
@xscreensaver -no-splash
@/home/pi/autorun/Signin

```

### Configuration file for mysql authentication
write a file `/home/pi/.mysql_auth` containing the following lines:
```sh
<mysql server host name>
<database/user> (assumed to be the same)
<password>
```

