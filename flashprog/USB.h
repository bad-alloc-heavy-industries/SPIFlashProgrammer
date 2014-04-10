#ifndef FLASHPROG_USB_H
#define FLASHPROG_USB_H

#include <stdint.h>

void usbInit();
void usbDeinit();

int32_t usbWrite(void *data, int32_t dataLen);
int32_t usbWriteByte(uint8_t data);
int32_t usbRead(void *data, int32_t dataLen);
int32_t usbReadByte(uint8_t *data);

#endif /*FLASHPROG_USB_H*/
