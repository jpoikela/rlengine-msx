/*
 * RetroDeLuxe Engine for MSX
 *
 * Copyright (C) 2017 Enric Martin Geijo (retrodeluxemsx@gmail.com)
 *
 * RDLEngine is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, version 2.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _MSX_H_SYS
#define _MSX_H_SYS

#include "msx.h"

#define MSEC_PER_TICK 16 // assume 60Hz
#define MAX_PROCS 10

#define BIOS_INT_HOOK 0xFD9A

struct sys_proc {
  void (*func)();
};

#define STICK_CENTER 0
#define STICK_UP 1
#define STICK_UP_RIGHT 2
#define STICK_RIGHT 3
#define STICK_DOWN_RIGHT 4
#define STICK_DOWN 5
#define STICK_DOWN_LEFT 6
#define STICK_LEFT 7
#define STICK_UP_LEFT 8

extern void sys_reboot();
extern uint8_t sys_get_key(uint8_t line) __nonbanked;
extern uint8_t sys_get_char(void);
extern uint8_t sys_get_stick(uint8_t port) __nonbanked;
extern uint8_t sys_get_trigger(uint8_t port) __nonbanked;
extern void sys_memcpy(uint8_t *dst, uint8_t *src, uint16_t size) __nonbanked;
#define sys_memset __builtin_memset

extern void sys_irq_register(void(*func)());
extern void sys_irq_unregister(void(*func)()) __nonbanked;
extern void sys_irq_init();
extern void sys_sleep_ms(uint16_t msecs) __nonbanked;
extern void sys_sleep(uint16_t secs) __nonbanked;
extern uint16_t sys_gettime_secs() __nonbanked;
extern uint16_t sys_gettime_msec() __nonbanked;
extern uint16_t sys_get_ticks() __nonbanked;
extern void sys_rand_init(uint8_t *seed) __nonbanked;
extern uint8_t sys_rand() __nonbanked;
extern bool sys_is60Hz() __nonbanked;
extern void sys_disable_kbd_click();

#endif
