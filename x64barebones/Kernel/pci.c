// Documentacion:
// http://wiki.osdev.org/PCI
// Varias funciones adaptadas de: https://github.com/AlgorithMan-de/wyoos/ (porteadas de C++)



#define PCI_CONFIG_ADDRESS  0x0CF8
#define PCI_CONFIG_DATA     0x0CFC

#define RTL_VENDOR_ID 0x10EC
#define RTL_DEVICE_ID 0x8139

#define INPUT_OUTPUT 1
#define MEMORY_MAPPING 0
#define NULL 0

#include <stdint.h>
#include <naiveConsole.h>
#include <port.h>
#include <pci.h>


// ;Configuration Mechanism One has two IO port rages associated with it.
// ;The address port (0xcf8-0xcfb) and the data port (0xcfc-0xcff).
// ;A configuration cycle consists of writing to the address port to specify which device and register 
//you want to access and then reading or writing the data to the data port.

//Escribir y leer de los puertos de control (ver pci.asm)
void os_pci_write_reg(uint8_t bus, uint8_t func, uint16_t port, uint64_t data);
uint32_t os_pci_read_reg(uint8_t bus, uint8_t func, uint16_t port);



void dma_init(){
  turn_on(0,0x18);
}


void turn_on(uint8_t bus, uint8_t device) {
  uint32_t reg = os_pci_read_reg(bus,device,0x04);
  reg |= (1<<2);
  os_pci_write_reg(bus,device,0x04,reg);

}






// De aca para abajo hay funciones para consultar y explorar el PCI
// Se usaron en la etapa de desarrollo 
// para encontrar el RTL entre los dispositivos del PCI 
// y consultar su IOADDR, no se usan en el kernel para nada

typedef struct {
            uint32_t portBase;
            uint32_t interrupt;

            uint8_t bus;
            uint8_t device;
            uint8_t function;

            uint16_t vendor_id;
            uint16_t device_id;

            uint8_t class_id;
            uint8_t subclass_id;
            uint8_t interface_id;

            uint8_t revision;

        }PCIDescriptor_t;

typedef PCIDescriptor_t * PCIDescriptor;


typedef struct
        {
            uint8_t* address;
            uint32_t size;
            uint8_t type;
        }  BaseAddressRegister;

BaseAddressRegister getBAR(uint8_t bus, uint8_t device, uint8_t function, uint16_t bar);





static int count = 0;
void printPCID(PCIDescriptor d){
    ncPrint("BUS: 0x"); ncPrintHex(d->bus);
    ncPrint(" DEVICE: 0x"); ncPrintHex(d->device & 0xFF);
    ncPrint(" FUNCTION: 0x"); ncPrintHex(d->function & 0xFF);
    ncPrint(" Vendor ID: 0x"); ncPrintHex(d->vendor_id & 0xFFFF);
    ncPrint(", Device ID: 0x"); ncPrintHex(d->device_id & 0xFFFF);
    ncPrint("Interrupt 0x"); ncPrintHex(d->interrupt);
    ncNewline();
}



 uint16_t pciConfigReadWord (uint8_t bus, uint8_t slot,
                             uint8_t func, uint8_t offset)
 {
    uint32_t address;
    uint32_t lbus  = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;

    /* create configuration address as per Figure 1 */
    address =   (uint32_t)((lbus << 16)
                | (lslot << 11)
                | (lfunc << 8)
                | (offset & 0xfc)
                | ((uint32_t)0x80000000));

    /* write out the address */
    sysOutLong(PCI_CONFIG_ADDRESS, address);
    /* read in the data */
    /* (offset & 2) * 8) = 0 will choose the first word of the 32 bits register */
    tmp = (uint16_t)((sysInLong(PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xffff);
    return (tmp);
 }



PCIDescriptor_t getDescriptor(uint8_t bus, uint8_t device, uint8_t function){
    PCIDescriptor_t result;
    int barNum;

    result.bus = bus;
    result.device = device;
    result.function = function;

    result.vendor_id = pciConfigReadWord(bus, device, function, 0x00);
    result.device_id = pciConfigReadWord(bus, device, function, 0x02);

    result.class_id = pciConfigReadWord(bus, device, function, 0x0b);
    result.subclass_id = pciConfigReadWord(bus, device, function, 0x0a);
    result.interface_id = pciConfigReadWord(bus, device, function, 0x09);

    result.revision = pciConfigReadWord(bus, device, function, 0x08);
    result.interrupt = pciConfigReadWord(bus, device, function, 0x3c);

    for(barNum = 0; barNum < 6; barNum++)
                {
                    BaseAddressRegister bar = getBAR(bus, device, function, barNum);
                    if(bar.address && (bar.type == INPUT_OUTPUT))
                        result.portBase = (uint32_t)bar.address;
                }

    return result;
}



BaseAddressRegister getBAR(uint8_t bus, uint8_t device, uint8_t function, uint16_t bar){

    BaseAddressRegister result;
    result.address = 0;

    uint32_t headertype = pciConfigReadWord(bus, device, function, 0x0E) & 0x7F;
    int maxBARs = 6 - (4*headertype);

    if(bar >= maxBARs)
        return result;

    uint32_t bar_value = pciConfigReadWord(bus, device, function, 0x10 + 4*bar);
    result.type = (bar_value & 0x1) ? INPUT_OUTPUT : MEMORY_MAPPING;
    uint32_t temp;

    if(result.type == MEMORY_MAPPING)
    {
        switch((bar_value >> 1) & 0x3)
        {

            case 0: // 32 Bit Mode
            case 1: // 20 Bit Mode
            case 2: // 64 Bit Mode
                break;
        }

    }
    else // INPUT_OUTPUT
    {
        result.address = (uint8_t*)(bar_value & ~0x3);
    }


    return result;
}


 void checkDevice(uint8_t bus, uint8_t device) {
     uint8_t function = 0;
     PCIDescriptor_t descriptor = getDescriptor(bus, device, function);
     uint16_t vendorID = descriptor.vendor_id;

   if(vendorID == 0xFFFF || vendorID == 0x0000) return;        // Device doesn't exist

      printPCID(&descriptor);

//checkFunction(bus, device, function);
//    uint32_t headerType = getHeaderType(bus, device, function);
  //  if( (headerType & 0x80) != 0) {
        /* It is a multi-function device, so check remaining functions */
    //    for(function = 1; function < 8; function++) {
   //         if(getVendorID(bus, device, function) != 0xFFFF) {
   //             checkFunction(bus, device, function);
   //         }
   //     }
  //  }
 }

 void checkFunction(uint8_t bus, uint8_t device, uint8_t function) {

 }


 void checkAllBuses(int from, int to) {
     uint8_t bus;
     uint8_t device;
     ncClear();

     for(bus = from; bus < to; bus++) {
         for(device = 0; device < 32; device++) {
             checkDevice(bus, device);
         }
     }
 }




void deviceDetails(uint8_t bus, uint8_t device){
//0x18
    PCIDescriptor_t descriptor = getDescriptor(bus, device, 0x4);
    PCIDescriptor d = &descriptor;
                ncNewline();
                ncPrint("BUS: 0x"); ncPrintHex(d->bus); ncNewline();
                ncPrint("DEVICE: 0x"); ncPrintHex(d->device & 0xFF); ncNewline();
                ncPrint("FUNCTION: 0x"); ncPrintHex(d->function & 0xFF); ncNewline();
                ncPrint("Vendor ID: 0x"); ncPrintHex(d->vendor_id & 0xFFFF); ncNewline();
                ncPrint("Device ID: 0x"); ncPrintHex(d->device_id & 0xFFFF); ncNewline();
                ncPrint("Interrupt line 0x"); ncPrintHex(d->interrupt & 0xFF); ncNewline();
                ncPrint("Interrupt pin 0x"); ncPrintHex((d->interrupt >> 8)& 0xFF); ncNewline();
                ncPrint("Base port 0x"); ncPrintHex(d->portBase); ncNewline();
                ncNewline();


}

void findRTL(){
     ncClear();
     PCIDescriptor_t descriptor;
     int bus, device, function;

     for(bus = 0; bus < 256; bus++) {
         for(device = 0; device < 32; device++) {
            for(function = 0 ; function < 8; function++){
                    
             descriptor = getDescriptor(bus, device, function);
             uint16_t vendorID = descriptor.vendor_id;
             uint16_t deviceID = descriptor.device_id;

             if(vendorID == RTL_VENDOR_ID || vendorID == RTL_DEVICE_ID){        // Device doesn't exist
                PCIDescriptor d = &descriptor;
                ncPrint("Found RTL8139");
                ncNewline();
                ncPrint("BUS: 0x"); ncPrintHex(d->bus); ncNewline();
                ncPrint("DEVICE: 0x"); ncPrintHex(d->device & 0xFF); ncNewline();
                ncPrint("FUNCTION: 0x"); ncPrintHex(d->function & 0xFF); ncNewline();
                ncPrint("Vendor ID: 0x"); ncPrintHex(d->vendor_id & 0xFFFF); ncNewline();
                ncPrint("Device ID: 0x"); ncPrintHex(d->device_id & 0xFFFF); ncNewline();
                ncPrint("Interrupt line 0x"); ncPrintHex(d->interrupt & 0xFF); ncNewline();
                ncPrint("Interrupt pin 0x"); ncPrintHex((d->interrupt >> 8)& 0xFF); ncNewline();
                ncPrint("Base port 0x"); ncPrintHex(d->portBase); ncNewline();
                ncNewline();
             }
     }
     }

 }
}
