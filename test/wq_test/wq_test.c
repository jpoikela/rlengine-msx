/**
 *
 * Copyright (C) Retro DeLuxe 2013, All rights reserved.
 *
 */

#define DEBUG

#include "msx.h"
#include "sys.h"
#include "vdp.h"
#include "sprite.h"
#include "wq.h"
#include "gen/wq_test.h"
#include <stdlib.h>

/**
 * Global data is placed in 0xC000 (RAM page 2) in 32K roms by default
 */
 enum spr_patterns_t {
 	PATRN_BEE,
 	PATRN_RAT,
 	PATRN_EGG,
 };
SpriteDef eggspr;

struct vdp_hw_sprite bee_hw;

SpriteDef bee[10];
SpriteDef rats[10];

struct work_struct work1;
struct work_struct work2;
struct work_struct work3;
struct work_struct work4;

/**
 * NOTE : any initialized global data must be constant.
 */
const uint8_t control_patt[8] = {255,255,255,255,255,255,255,255};
const uint8_t control_colors [1] = {6};


void update_bees1();
void update_bees2();
void update_rats1();
void update_rats2();

void main()
{
	uint8_t i;

	vdp_set_mode(MODE_GRP1);
	vdp_set_color(COLOR_WHITE, COLOR_BLACK);
	vdp_clear_grp1(0);
	spr_init();

	/**
	 * Low level sprites using direct access to VRAM
	 */
	vdp_copy_to_vram(bee1, vdp_base_sppat_grp1, 16 * 8);
	bee_hw.x = 100;
	bee_hw.y = 100;
	bee_hw.pattern = 0;
	bee_hw.color = 15;
	vdp_set_hw_sprite(&bee_hw, 0);

	do {
	} while (sys_get_key(8) & 1);

	/**
	 * Single layer sprites with animation in two directions
	 */
	spr_define_pattern_set(PATRN_BEE, SPR_SIZE_16x16, 1, 2, 2, bee1, bee1_color);
	spr_define_pattern_set(PATRN_RAT, SPR_SIZE_16x16, 1, 2, 2, rat, rat_color);
	spr_define_pattern_set(PATRN_EGG, SPR_SIZE_16x16, 2, 3, 4, eggerland, eggerland_color);

	spr_valloc_pattern_set(PATRN_BEE);
	spr_valloc_pattern_set(PATRN_RAT);
	spr_valloc_pattern_set(PATRN_EGG);

	for (i = 0; i< 10; i++) {
		spr_init_sprite(&bee[i], PATRN_BEE);
		spr_init_sprite(&rats[i], PATRN_RAT);
		// set in random initial positions
		spr_set_pos(&bee[i], i * 20, i * 20);
		spr_set_pos(&rats[i], 16 + i * 20, 16 + i * 20);
		spr_show(&bee[i]);
		spr_show(&rats[i]);
	}

	//SPR_DEFINE_SPRITE(eggspr, &egg_patt, 10, eggerland_color);
	//spr_set_pos(&eggspr, 100, 100);
	//spr_show(&eggspr);

	sys_irq_init();
	wq_start();

	INIT_WORK(work1,update_bees1);

	queue_delayed_work(&work1,2,0);

	do {
		sys_sleep(20);
	} while (1);

}

void update_bees1() {
	uint8_t i;
	for (i = 0; i< 4; i++)  {
		spr_animate(&bee[i],-1,0);
	}
	queue_delayed_work(&work1,0,100);
}

void update_bees2() {
	uint8_t i;
	for (i = 4; i< 8; i++)  {
		spr_animate(&bee[i],1,0);
	}
}
