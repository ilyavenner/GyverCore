﻿/*
Нормальный ввод/вывод
*/
#define ARDUINO_MAIN
#include "Arduino.h"
#include <avr/io.h>
uint8_t a_ref = DEFAULT;  // глобальная переменная для хранения опорного напряжения АЦП
// ============= DIGITAL =============

void pinMode(uint8_t pin, uint8_t mode)
{
	switch (mode) {
	case 0: // input
		if (pin < 8) bitWrite(DDRD, pin, 0);   // расставляем нули в DDRn
		else if (pin < 14) bitWrite(DDRB, (pin - 8), 0);
		else if (pin < 20) bitWrite(DDRC, (pin - 14), 0);
		break;
	case 1: // output
		if (pin < 8) bitWrite(DDRD, pin, 1);   // расставляем еденицы в DDRn
		else if (pin < 14) bitWrite(DDRB, (pin - 8), 1);
		else if (pin < 20) bitWrite(DDRC, (pin - 14), 1);
		break;
	case 2: // in_pullup
		if (pin < 8) {
			bitWrite(DDRD, pin, 0);  // настраиваем как вх
			bitWrite(PORTD, pin, 1); // вкл подтяжку
		}
		else if (pin < 14) {
			bitWrite(DDRB, (pin - 8), 0);
			bitWrite(PORTB, (pin - 8), 1);
		}
		else if (pin < 20) {
			bitWrite(DDRC, (pin - 14), 0);
			bitWrite(PORTC, (pin - 14), 1);
		}
		break;
	}
}

void digitalWrite(uint8_t pin, uint8_t x) {
	switch (pin) { // откл pwm
	case 3:  // 2B
		TCCR2A &= ~(1 << COM2B1);
		break;
	case 5: // 0B
		TCCR0A &= ~(1 << COM0B1);
		break;
	case 6: // 0A
		TCCR0A &= ~(1 << COM0A1);
		break;
	case 9: // 1A
		TCCR1A &= ~(1 << COM1A1);
		break;
	case 10: // 1B
		TCCR1A &= ~(1 << COM1B1);
		break;
	case 11: // 2A
		TCCR2A &= ~(1 << COM2A1);
		break;
	}
	if (pin < 8) bitWrite(PORTD, pin, x);
	else if (pin < 14) bitWrite(PORTB, (pin - 8), x);
	else if (pin < 20) bitWrite(PORTC, (pin - 14), x);
}

int digitalRead (uint8_t pin) {
	if (pin < 8) return bitRead(PIND, pin);
	else if (pin < 14) return bitRead(PINB, pin - 8);
	else if (pin < 20) return bitRead(PINC, pin - 14);
}


void digitalToggle(uint8_t pin){
	if (pin < 8) bitToggle(PORTD, pin);
	else if (pin < 14) bitToggle(PORTB, pin - 8);
	else if (pin < 20) bitToggle(PORTC, pin - 14);
	/*
	if (pin < 8) {
		bitWrite(PORTD, pin, !bitRead(PORTD,pin));
	}
	else if (pin < 14){
		bitWrite(PORTB, (pin - 8),!bitRead(PORTB,(pin-8)));
	}
	else if (pin < 20){ 
		bitWrite(PORTC, (pin - 14), !bitRead(PORTC,(pin-14)));
	}
	*/
}
// ============= ANALOG =============
void analogPrescaler (uint8_t prescl) { 
	cli();
	switch (prescl) {
	case 2: 
		ADCSRA &= ~((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));
		break;
	case 4: // (defalut)
		ADCSRA &= ~((1 << ADPS2) | (1 << ADPS0));
		ADCSRA |= (1 << ADPS1);
		break;
	case 8:
		ADCSRA &= ~ (1 << ADPS2);
		ADCSRA |= ((1 << ADPS1) | (1 << ADPS0));
		break;
	case 16:
		ADCSRA &= ~((1 << ADPS1) | (1 << ADPS0));
		ADCSRA |= (1 << ADPS2);
		break;
	case 32:
		ADCSRA &= ~ (1 << ADPS1);
		ADCSRA |= ((1 << ADPS2) | (1 << ADPS0));
		break;
	case 64:
		ADCSRA &= ~ (1 << ADPS0);
		ADCSRA |= ((1 << ADPS2) | (1 << ADPS1));
		break;
	case 128:
		ADCSRA |= ((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));
		break;
	}
	sei();
}

void analogReference(uint8_t mode)
{
	a_ref  = mode; // изменения будут приняты в силу при следующем analogRead() / analogStartConvert()
}

int analogRead(uint8_t pin)
{    
	analogStartConvert(pin);
	analogGet();
}

void analogStartConvert(byte pin) {
	if(a_ref == INTERNAL) a_ref = 3; // для удобства задвигания 
	uint8_t oldSREG = SREG; // запомнинаем были ли включены прерывания
	cli();//выключаем прерывания
	pin = (pin < 14) ? (pin) : (pin - 14);		// совместимость между A0, A1.. A7 и 0, 1.. 7
	ADMUX = 0; // обнуляем опорное и мультиплексор 
	ADMUX = (a_ref << 6) | pin; // задвигаем опорное и номер входа
	SREG = oldSREG; // если прерывания не были включены - не включаем и наоборот
	if (pin == 8 || pin == 14) delay(2); // Wait for Vref to settle для VCC и термометра
	ADCSRA |= (1 << ADSC); // начинаем преобразование
}

int analogGet() {
	while (ADCSRA & (1 << ADSC)); // ждем окончания
	return ADCL | (ADCH << 8); // склеить и вернуть результат
}

// ============= PWM =============
void analogWrite(uint8_t pin, int val) {
	cli();
	switch (val) {
	case 5:
		bitSet(TCCR0A, COM0B1);
		OCR0B = val;
		break;
	case 6:
		bitSet(TCCR0A, COM0A1);
		OCR0A = val;
		break;
	case 10:
		bitSet(TCCR1A, COM1B1);
		OCR1B = val;
		break;
	case 9:
		bitSet(TCCR1A, COM1A1);
		OCR1A = val;
		break;
	case 3:
		bitSet(TCCR2A, COM2B1);
		OCR2B = val;
		break;
	case 11:
		bitSet(TCCR2A, COM2A1);
		OCR2A = val;
		break;
	default:
		break;
	}
	sei();
}