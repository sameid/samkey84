#samkeys84

###Custom firmware written for the Lofree Flow 2 (84-key) keyboard, since Lofree bricked my keyboard!

What's working:

- All keys
- Function Layer
- Media Keys
- Backlight (On/Off only)
- Caps Lock light

Work in progress:

- Bluetooth LE
- 2.4GHz Dongle
- Side Touch Bar
- Battery Indicator

Why?

I was loving my new keyboard from Lofree! This was my first purchase from the company. Although the keyboard felt great, it was having connectivity issues over bluetooth and the wireless dongle. So I did some digging, and looks like they had a firmware update. I followed the instructions and it bricked the keyboard! To the point I could not enter DFU mode to flash new firmware. I contacted support, and followed their instructions on getting the keyboard up and running, but to no avail! So they agreed to send me a new one.

Although I'm happy to be receiving a new one, I thought why can't I just write my own firmware, it's confirmed that the keyboard is QMK Firmware compatible. So I set off on to an adventure. I was able to get the board up and running with basic minimal functionality, by tracing pins on the board. Now I have a functioning keyboard, albeit without wireless. I am working on the wireless functionality, but it will take me sometime to reverse it.
