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

<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br /><span xmlns:dct="http://purl.org/dc/terms/" property="dct:title">QT Timeclock</span> by <a xmlns:cc="http://creativecommons.org/ns#" href="https://github.com/mstrperson/qt-timeclock" property="cc:attributionName" rel="cc:attributionURL">Jason Cox</a> is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a>.<br />Based on a work at <a xmlns:dct="http://purl.org/dc/terms/" href="https://github.com/mstrperson/qt-timeclock" rel="dct:source">https://github.com/mstrperson/qt-timeclock</a>.
