sudo apt install cmake clang clang-format cppcheck global build-essential gdb git wiringpi
sudo apt install libpcl-devsdo
Plug in the B5L
sudo modprobe usbserial vendor=0x0590 product=0x00ca
dmesg #get the tty device the driver attached to
sudo chmod 666 /dev/ttyUSB0 #set permissions for the com port


#debug in sudo

#add file named "lldb" to home dir with content:
pkexec /usr/bin/lldb-mi "$@"

#add to launch.json
"externalConsole": false,
"miDebuggerPath": "/home/<username>/lldb",
"MIMode": "lldb",
