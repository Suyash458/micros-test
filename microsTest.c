#define F_CPU 16000000
#define clockCyclesPerMicrosecond() (F_CPU/1000000)
#define USART_BAUDRATE 9600
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1) 

#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<stdlib.h>
#include<stdio.h>

unsigned long timer1_overflow_count = 0;
unsigned long micros(void);
void timer1_init(void);
void USART0Init(void);
int USART0SendByte(char, FILE*);
int USART0ReceiveByte(FILE*);

FILE usart0_str = FDEV_SETUP_STREAM(USART0SendByte, USART0ReceiveByte, _FDEV_SETUP_RW);

ISR(TIMER1_COMPA_vect) {
    timer1_overflow_count++;
}

int main() {
    timer1_init();
    USART0Init();
    stdin=stdout=&usart0_str;
    sei();
    unsigned long start, finish;
    while(1) {
        /*
        The error in measurement is only around 2µs 
        across a wide range of values(tested till 300µs)
        Arduino's micros() function has an error of around 10-30µs 
        when the measurement inverval crosses 60µs
        Change OCR1A's value as per your need since timer1_overflow_count 
        will wraparound after it reaches 429,496,7295.
        Conversion: 16 counts of TCNT1 = 1µs @ F_CPU = 16Mhz
        */
        start = micros();
        _delay_us(65);
        finish = micros();
        if (finish > start)
            printf("%lu %lu %lu\n", finish, start, finish - start);
    }
}

unsigned long micros() {
    // Overflow_count * 1024 + current_timer_count/16
	return (timer1_overflow_count << 10) + (TCNT1 >> 4); 
}

void timer1_init() {
    //TCCR1A |= _BV(COM1A0);
    TCCR1B |= _BV(CS10) | _BV(WGM12);
    TIMSK1 |= _BV(OCIE1A);
    OCR1A = 16383;
}

void USART0Init(void) {
    // Set baud rate
    UBRR0H = (uint8_t)(UBRR_VALUE>>8);
    UBRR0L = (uint8_t)UBRR_VALUE;
    // Set frame format to 8 data bits, no parity, 1 stop bit
    UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
    //enable transmission and reception
    UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
}

int USART0SendByte(char u8Data, FILE *stream) {
   if(u8Data == '\n')
   {
      USART0SendByte('\r', 0);
   } 
    //wait while previous byte is completed
    while(!(UCSR0A&(1<<UDRE0))){};
    // Transmit data
    UDR0 = u8Data;
    return 0;
}

int USART0ReceiveByte(FILE *stream) {
    uint8_t u8Data;
    // Wait for byte to be received
    while(!(UCSR0A&(1<<RXC0))){};
    u8Data=UDR0;
    //echo input data
    USART0SendByte(u8Data,stream); 
    // Return received data
    return u8Data;
}




