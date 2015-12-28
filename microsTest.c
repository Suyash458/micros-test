#define F_CPU 16000000
#define clockCyclesPerMicrosecond() (F_CPU/1000000)
#define MICROS_OVERHEAD = 3

#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

unsigned long timer1_overflow_count = 0;
unsigned long micros(void);
void timer1_init(void);

ISR(TIMER1_COMPA_vect){
    timer1_overflow_count++;
}

int main(){
    DDRD |= (1<<5);
    PORTD |= (1<<5);
    timer1_init();
    sei();
    unsigned long start;
    while(1)
    {
       start = micros();
       while((micros() - start) < 16);
       PORTD ^= (1<<5);
    }
}

unsigned long micros() {
    // Overflow_count * 256 + current_timer_count/16
	return (timer1_overflow_count << 8) + (TCNT1 >> 4); 
}

void timer1_init(){
    //TCCR1A |= _BV(COM1A0);
    TCCR1B |= _BV(CS10) | _BV(WGM12);
    TIMSK1 |= _BV(OCIE1A);
    OCR1A = 4095;
}



