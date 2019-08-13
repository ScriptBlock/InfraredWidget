# Purpose built IR transmitter

This is a little purpose built widget that I built to plug into my virtual pincab that powers the primary [TV](https://www.amazon.com/gp/product/B07DK5PZFY/ref=ppx_yo_dt_b_asin_title_o05_s00?ie=UTF8&psc=1) on and off.  I did this for a few reasons:

 - The TVs default state is off, so if I remove power from the TV when I shut down the cabinet I have to explicitly turn the TV back on with a remote.
 - If I leave the TV with power, it takes 15 minutes minimum for the TV to go to sleep.  And then when I power the cab back on, I have to again explicitly power the TV back on.
 - I wanted to make a thing.

Note: If you are using a [pinscape board](https://os.mbed.com/users/mjr/code/Pinscape_Controller_V2/) MJR has built in this functionality already.  You are probably better off using his solution.

This repo has 3 different assets:
 1. The VS Code project is the codebase for the microcontroller to listen and respond on serial to your vpin computer.
 2. [3D Models]("3d models") are a simple little case to hold your components.  You just hot glue the components in there and screw the case top on with little M2x6 screws.
 3. [PinUPMenu.exe startup script](scripts)

The components I used are
 - [Arduino Nano V3](https://robotdyn.com/nano-v3-atmega-328-usb-ttl-ch340g-micro-usb-with-cable-not-soldered.html)
 - [IR Transmitter](https://www.amazon.com/gp/product/B01E20VQD8/ref=ppx_yo_dt_b_asin_title_o02_s00?ie=UTF8&psc=1)
 - A couple of female/female dupont wires

Hookup is trivial.  
 - DATA on the transmitter to D3 on the nano.
 - VCC on the transmitter to 5V on the nano.
 - GND on the transmitter to GND on the nano.

Use platformio to build the code to your nano.  You will need to record which COM port the nano gets assgined for serial IO.

The code allows a configurable startup delay via serial, or you can just modify the code to hardcode a delay.  Just connect to the COM port at 19200 and send a string formatted D00001 thru D99999 and that many milliseconds delay will happen before the power signal gets sent to the TV.

You can refer to the powershell script in the [scripts](scripts) directory.  This little guy will launch PinUPMenu, get the process ID and then waits until that process is finished.  Once done, it will send a 'P' via serial to the widget, which will then send a power command to the TV.  You will have to modify the COM port referenced in the script with the COM port in use on your system.

The workflow I use is:
 1. This script is launched when windows starts up (using the shell:startup folder).  
 2. You play pinball
 3. I exit PinUp using one of my pincab buttons
 4. The script sends the TV power command which puts the TV into standby
 5. I physically press the power off button on the cab.  
 5a. You COULD add in a shutdown command to this script which would shut down windows and power off the TV in response to PinUP closing.  But my button-happy guests tend to accidentally exit PinUP.  So I don't do that.

