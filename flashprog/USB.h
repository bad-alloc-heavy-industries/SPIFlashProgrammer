#ifndef FLASHPROG_USB_H
#define FLASHPROG_USB_H

void usbInit();
void usbDeinit();

int32_t usbWrite(uint8_t *data, int32_t dataLen);
int32_t usbRead(uint8_t *data, int32_t dataLen);

#endif /*FLASHPROG_USB_H*/
