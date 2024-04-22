# WS2812 1.0

This project is to demonstrate how to use multi PIO programs on RP2040. I use a Adafruit NeoPixel ring https://www.adafruit.com/product/1463 .
The PIO code is from the examples in the pico sdk and the initialise code is also from the examples, exept the allocated state machine is returned.
Notis, the CMakeLists.txt, I used a pico-w, if you have a pico, change that in CMakeLists.txt (Line 3).
The strip functions in ws2812.c:

* `strip_init` Fill's the structure strip with it's reset state data, load the PIO program and allocate a state machine.
