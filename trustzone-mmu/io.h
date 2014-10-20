#ifndef __IO_H__
#define __IO_H__

void SEC_PUT32(unsigned int, unsigned int);
void SEC_PUT16(unsigned int, unsigned int);
void SEC_PUT8(unsigned int, unsigned int);
unsigned int SEC_GET32(unsigned int);
unsigned int SEC_GETPC(void);
void SEC_BRANCHTO(unsigned int);
void SEC_DUMMY(unsigned int);

void setup_monitor(void);

#endif
