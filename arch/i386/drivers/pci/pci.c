#include <i386/regs.h>
#include <i386/panic.h>
#include <stdint.h>


#define PCI_CONFIG_ADDR     0xCF8
#define PCI_CONFIG_DATA     0xCFC

struct pci_device {
    uint32_t vendor;
    uint32_t device;
    uint32_t func;
};
typedef struct pci_device pci_device_t;

static inline void IoWrite8(uint32_t port, uint8_t data)
{
    __asm__ __volatile__("IoWrite32 %b0, %w1" : : "a" (data), "Nd" (port));
}

static inline uint8_t IoRead8(uint32_t port)
{
    uint8_t data;
    __asm__ __volatile__("IoRead32 %w1, %b0" : "=a" (data) : "Nd" (port));
    return data;
}

static inline void IoWrite16(uint32_t port, uint16_t data)
{
    __asm__ __volatile__("outw %w0, %w1" : : "a" (data), "Nd" (port));
}

static inline uint16_t IoRead16(uint32_t port)
{
    uint16_t data;
    __asm__ __volatile__("inw %w1, %w0" : "=a" (data) : "Nd" (port));
    return data;
}

static inline void IoWrite32(uint32_t port, uint32_t data)
{
    __asm__ __volatile__("outl %0, %w1" : : "a" (data), "Nd" (port));
}

static inline uint32_t IoRead32(uint32_t port)
{
    uint32_t data;
    __asm__ __volatile__("inl %w1, %0" : "=a" (data) : "Nd" (port));
    return data;
}

static inline void io_wait()
{
    __asm__ __volatile__("jmp 1f;1:jmp 1f;1:");
}

const char* PCI_CLASS_IDS[18] =
{
    "no class specification",
    "Mass Storage Controller",
    "Network Controller",
    "Display Controller",
    "Multimedia Device",
    "Memory Controller",
    "Bridge Device",
    "Simple Communication Controller",
    "Base System Peripheral",
    "Input Device",
    "Docking Station",
    "Processor",
    "Serial Bus Controller",
    "Wireless Controller",
    "Intelligent I/O Controller",
    "Satellite Communication Controller",
    "Encryption/Decryption Controller",
    "Data Acquisition and Signal Processing Controller"
};

pci_device_t pci_devices[16];
uint32_t pci_devices_size = 0;

/**
 * Read word at offset from pci device at bus `bus`, device `slot`, and function `func` (for multifunc device)
 */
uint16_t pci_config_read_word(uint16_t bus, uint16_t slot, uint16_t func, uint8_t offset) {
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t lslot = (uint32_t)slot;
    uint32_t lfunc = (uint32_t)func;
    uint16_t tmp = 0;

    // Create config addr
    address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));

    // Write out PCI config address
    IoWrite32(PCI_CONFIG_ADDR, address);

    // Read PCI config data
    // (offset & 2) * 8 = 0 will choose first word of 32bits register
    tmp = (uint16_t)((IoRead32(PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xffff);
    return(tmp);
}

/**
 * Get vendor ID of PCI device at bus `bus`, device `slot`
 */
uint16_t pci_get_vendor_id(uint16_t bus, uint16_t slot, uint16_t func) {
    uint16_t vendor;

    vendor = pci_config_read_word(bus, slot, func, 0);
    return vendor;
}

/**
 * Get device ID of PCI device at bus `bus`, device `slot`
 */
uint16_t pci_get_device_id(uint16_t bus, uint16_t slot, uint16_t func) {
    uint16_t device;

    device = pci_config_read_word(bus, slot, func, 2);
    return device;
}

/**
 * Get class ID of pci device device on bus `bus`
 */
uint16_t pci_get_device_class_id(uint16_t bus, uint16_t slot, uint16_t func) {
    uint32_t class_id_seg;
    class_id_seg = pci_config_read_word(bus, slot, func, 0xA);

    return (class_id_seg & ~0x00FF) >> 8;
}

/**
 * Get subclassclass ID of pci device device on bus `bus`
 */
uint16_t pci_get_device_subclass_id(uint16_t bus, uint16_t slot, uint16_t func) {
    uint32_t subclass_id_seg;
    subclass_id_seg = pci_config_read_word(bus, slot, func, 0xA);

    return (subclass_id_seg & ~0xFF00);
}

/**
 * Scan PCI device array
 */
void pci_scan() {
    printk("Scanning for PCI devices...\n");
    pci_probe();
}

/**
 * Loop through PCI devices and print out information for the first 16 devices
 */
void pci_probe() {
    for(uint16_t bus = 0; bus < 256; bus++) {
        for (uint16_t slot = 0; slot < 32; slot++) {
            for(uint16_t function = 0; function < 8; function++) {
                if(pci_devices_size > 15) {
                    printk("[PCI] Not adding above device... limit reached\n");
                    return;
                }

                uint16_t vendor_id = pci_get_vendor_id(bus, slot, function);
                if(vendor_id == 0xFFFF) continue;
                uint16_t device_id = pci_get_device_id(bus, slot, function);
                uint16_t class_id = pci_get_device_class_id(bus, slot, function);
                printk("[PCI] 0x%x:0x%x - %s\n", vendor_id, device_id, PCI_CLASS_IDS[class_id]);

                // Add current device to local pci_devices array
                pci_devices[pci_devices_size].vendor = vendor_id;
                pci_devices[pci_devices_size].device = device_id;
                pci_devices[pci_devices_size].func = function;

                ++pci_devices_size;
            }
        }
    }
}