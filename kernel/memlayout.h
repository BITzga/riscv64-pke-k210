#ifndef _MEMLAYOUT_H
#define _MEMLAYOUT_H
#include "riscv.h"

// RISC-V machine places its physical memory above DRAM_BASE
#define DRAM_BASE 0x80020000

// the beginning virtual address of PKE kernel
#define KERN_BASE 0x80020000

// default stack size
#define STACK_SIZE 4096

// virtual address of stack top of user process
#define USER_STACK_TOP 0x7ffff000

// simple heap bottom, virtual address starts from 4MB
#define USER_FREE_ADDRESS_START 0x00000000 + PGSIZE * 1024

#endif
