#!/bin/bash

avrdude -c avr910 -p atmega32 -P /dev/ttyUSB0 -b 115200 $*
