#pragma once
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;

typedef signed char  int8_t;
typedef signed short int16_t;
typedef signed long int32_t;
typedef signed long long int64_t;

void outb(uint16_t, uint8_t);
uint8_t inb(uint16_t);
uint16_t inw(uint16_t port);