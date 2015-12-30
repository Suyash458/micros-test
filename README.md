# micros-test

Testing a faster implementation of Arduino's micros() function


Arduino's implementation always returns multiples of four so I'm using the 16 bit timer 
from an Atmega328p. The timer runs at F_CPU(16 Mhz) in CTC mode without any prescaling.
While polling continuously, the smallest time interval that can be measured without significant 
error seems to be 5µs and there's an error of 0.05µs to 0.2µs in the readings. The overhead for 
the function call seems to be 3-4µs in this example so I'm checking for 7µs instead of 10µs. 
The error and overhead could change in different examples since there's varying overhead involved 
in checking the loop condition for different values and toggling pin 5 on PORTD. 

timer1_overflow_count is incremented once every 256µs and 16 counts of TCNT1 take 1µs.
It wraps around after 71.58 minutes.

## usage

* `cd` to the directory
* run `make all`
* run  `avrdude -p m328p -c 'your-programmer-type' -P 'your-COM-port' -U flash:w:microsTest.hex:i`

## screenshots

![alt tag](http://imgur.com/98YxzRy.png?1)
* Testing at 10µs

![alt tag](http://i.imgur.com/jODWywt.png?1)
* Testing at 15µs