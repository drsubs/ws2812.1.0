# WS2812 1.0

This project is to demonstrate how to use multi PIO programs on RP2040. I use a Adafruit NeoPixel ring https://www.adafruit.com/product/1463 .
The PIO code is from the examples in the pico sdk and the initialise code is also from the examples, exept the allocated state machine is returned.
Notis, the CMakeLists.txt, I used a pico-w, if you have a pico, change that in CMakeLists.txt (Line 3).

The strip functions in ws2812.c:

* `strip_init` Fill's the structure strip with it's reset state data, load the PIO program and allocate a state machine.
* `strip_reset` Resets the strip after transmition. This must be done after every transmit.
* `strip_tx` Strip transmit. It must be called in a loop until it returns true. If you use it as a single transmit use this code:

```
while(!strip_tx(&myStrip)) ;
```

* `ws2812_init` This setup and initialise a strip. It is called from strip_init.

The functions in main.c:

* `strips_tx` Transmit all strip in an array of strips.
* `strips_reset` Reset all strip in an array of strips.
* `tx_pump` The transmittion pump. 

The functions in patterns.c is to create an output. 

The code is commented in doxy style and there is a doxyfile, too create a doxygen doc. 