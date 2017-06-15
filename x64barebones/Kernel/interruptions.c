#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <interruptions.h>
#include <videoDriver.h>
#include <keyboardDriver.h>
#include <tick.h>
#include <rtlDriver.h>

#define TIMERTICK 0
#define KEYBOARD 1
#define NETWORK 11


#pragma pack(push)
#pragma pack(1)

extern uint8_t text;
extern uint8_t rodata;

typedef void (*handler_t)(void);

#pragma pack(pop)

static IDTEntry_t* IDT = (IDTEntry_t*) 0x0;


void iSetHandler(int index, uint64_t handler) {
	IDT[index].offset_l = (uint16_t) handler & 0xFFFF;
	IDT[index].offset_m = (uint16_t) (handler >> 16) & 0xFFFF;
	IDT[index].offset_h = (uint32_t) (handler >> 32) & 0xFFFFFFFF;
	IDT[index].selector = 0x08;
	IDT[index].zero_l = 0;
	IDT[index].attrs = 0x8E;
	IDT[index].zero_h = 0;
}

void setIDT(void) {
	iSetHandler(0x80, (uint64_t) &_sysCallHandler);
	iSetHandler(0x20, (uint64_t) &_irq00Handler);
	iSetHandler(0x21, (uint64_t) &_irq01Handler);
	iSetHandler(0x2B, (uint64_t) &_irq11Handler); // 0x2B es la interrupcion IRQ 11, aca interrumpe el rtl

	_setPicMaster(0x00);
	_setPicSlave(0x0);
}

void irqDispatcher(int interruptIndex) {
	switch(interruptIndex) {
		case TIMERTICK: {
			tickHandler();
			break;
		}
		case KEYBOARD: {
			addKeyToBuffer();
			break;
		}
		case NETWORK: {
			rtlHandler();
			break;
		}
	}
}