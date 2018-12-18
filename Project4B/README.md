# CS 111 - Project 4B - Sensors Input

A description of the project can be found in **P4B.html**.

## Deviations from the Project Description

* For primarily financial reasons, I decided to use a RaspberryPi 3 instead of a Beaglebone.

## Known Issues

* Wiring the Grove sensors directly to GPIO pins on the Pi seemed too time-consuming. I bought an attachment called GrovePi to deal with this. I believe there is an analogous attachment required for the Beaglebone.
* Set up for the MRAA library on the Pi is not trivial - this took a good chunk of my time on the project.
* Due to a lack of time, I did not have much of a chance to thoroughly test the software. I imagine there are at least a few bugs lurking. Hopefully someone will find them someday!