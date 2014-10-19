#ifndef __IO_H__
#define __IO_H__

void EARLY_PUT32(unsigned int, unsigned int);
void EARLY_PUT16(unsigned int, unsigned int);
void EARLY_PUT8(unsigned int, unsigned int);
unsigned int EARLY_GET32(unsigned int);
unsigned int EARLY_GETPC(void);
void EARLY_BRANCHTO(unsigned int);
void EARLY_DUMMY(unsigned int);

void PUT32(unsigned int, unsigned int);
void PUT16(unsigned int, unsigned int);
void PUT8(unsigned int, unsigned int);
unsigned int GET32(unsigned int);
unsigned int GETPC(void);
void BRANCHTO(unsigned int);
void DUMMY(unsigned int);


#endif
