#ifndef _INTERRUPTIONS_H_
#define _INTERRUPTIONS_H_

#include <stdint.h>

void iSetHandler(int index, uint64_t handler);
void setIDT(void);
void irqDispatcher(int interruptIndex);
void _sti(void);
void _cli(void);
uint64_t _irq00Handler();
uint64_t _irq01Handler();
uint64_t _sysCallHandler(uint64_t sysCallID, uint64_t str);
uint64_t _irq11Handler();
void _setPicMaster(uint16_t);	//ESTO ESTA BIEN?
void _setPicSlave(uint16_t);	//IDEM

typedef struct { 
	uint16_t offset_l; //bit 0..15
	uint16_t selector;
	uint8_t zero_l;
	uint8_t attrs;
	uint16_t offset_m; //16..31
	uint32_t offset_h; //32..63
	uint32_t zero_h;
} IDTEntry_t;

#endif