/*
 * RetroDeLuxe Engine for MSX
 *
 * Copyright (C) 2020 Enric Martin Geijo (retrodeluxemsx@gmail.com)
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
#ifndef _ASCII8_H_
#define _ASCII8_H_

#include "msx.h"

#define HASH_LITERAL #
#define HASH() HASH_LITERAL

/**
 * Gets the ASCII8 ROM page of a symbol and stores it in ascii8_page
 */
#define ascii8_get_page(SYMBOL)                                                 \
  __asm                                                                         \
  push  iy                                                                      \
  ld    iy, HASH()_ascii8_page                                                  \
  ld    0(iy),HASH()(_##SYMBOL >> 16)                                           \
  pop   iy                                                                      \
  __endasm

extern uint8_t ascii8_page;

extern void ascii8_set_data(uint8_t page) __nonbanked;
extern void ascii8_set_code(uint8_t page) __nonbanked;
extern void ascii8_restore() __nonbanked;

#endif // ASCII8_H_