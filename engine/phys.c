/**
 *
 * Copyright (C) Retro DeLuxe 2017, All rights reserved.
 *
 */

#include "msx.h"
#include "log.h"
#include "displ.h"
#include "phys.h"
#include "sys.h"
#include "sprite.h"
#include "bitmap.h"
#include "tile.h"

static uint8_t colliding_tiles[32];
static uint8_t colliding_tiles_down[32];
static uint8_t tile[12];
static void (*sprite_colision_cb)();

static struct tile_collision_group cgroup[MAX_CROUPS];
static uint8_t n_cgroups;

#define STATFL 0xf3e7;
#define SPR_COLISION_MASK 32
#define SPR_5TH_ALIGN_MASK 64

void phys_check_collision_bit()
{
	uint8_t *status = (uint8_t *)STATFL;
	if ((*status & SPR_COLISION_MASK) != 0) {
		sprite_colision_cb();
	}
}

void phys_init()
{
	sys_memset(colliding_tiles, 255, 32);
	sys_memset(colliding_tiles_down, 255, 32);
	n_cgroups = 0;
}


void phys_set_sprite_collision_handler(void (*handler))
{
	sprite_colision_cb = handler;
	sys_proc_register(phys_check_collision_bit);
}


/**
 * set callbacks for specific tiles
 */
void phys_set_tile_collision_handler(struct tile_object *tob, void (*handler))
{
	uint8_t base_tile = tob->ts->pidx;
	uint8_t num_tiles = tob->ts->frame_w * tob->ts->frame_h *
		tob->ts->n_frames * tob->ts->n_dirs;

	cgroup[n_cgroups].start = base_tile;
	cgroup[n_cgroups].end = base_tile + num_tiles;
	cgroup[n_cgroups].handler = handler;
	n_cgroups++;
}

/*
 * if the tile has a handler set, notigy
 */
static void phys_tile_collision_notify(uint8_t tile)
{
	uint8_t i;
	for (i = 0; i < n_cgroups; i++) {
		if (tile >= cgroup[i].start && tile <= cgroup[i].end) {
			cgroup[i].handler();
		}
	}
}
/*
 * Set collision flag for a specific tile (all directions)
 */
void phys_set_colliding_tile(uint8_t tile)
{
        bitmap_reset(colliding_tiles, tile);
        bitmap_reset(colliding_tiles_down, tile);
}

/**
 * Set a tile that collides only when falling
 */
void phys_set_down_colliding_tile(uint8_t tile)
{
        bitmap_reset(colliding_tiles_down, tile);
}

void phys_clear_colliding_tile(uint8_t tile)
{
        bitmap_set(colliding_tiles, tile);
        bitmap_set(colliding_tiles_down, tile);
}

static bool is_coliding_tile_pair(uint8_t tile1, uint8_t tile2)
{
	return ((bitmap_get(colliding_tiles, tile1) == 0) ||
                 (bitmap_get(colliding_tiles, tile2) == 0));
}

static bool is_coliding_down_tile_pair(uint8_t tile1, uint8_t tile2)
{
	return ((bitmap_get(colliding_tiles_down, tile1) == 0) ||
                 (bitmap_get(colliding_tiles_down, tile2) == 0));
}

static bool is_coliding_tile_triplet(uint8_t tile1, uint8_t tile2, uint8_t tile3)
{
	return ((bitmap_get(colliding_tiles, tile1) == 0) ||
                 (bitmap_get(colliding_tiles, tile2) == 0) ||
                 (bitmap_get(colliding_tiles, tile3) == 0));
}


#define TILE_WIDTH 32


/**
 * Update dpo collision_state
 */
static void phys_detect_tile_collisions_16x32(struct displ_object *obj,
        uint8_t *map, int8_t dx, int8_t dy)
{
	int16_t x,y;
	uint8_t *base;
        uint16_t base_vdp;
        uint8_t i;

	x = obj->xpos + 4;
	y = obj->ypos + 6;

	base = map + x / 8 + y / 8 * TILE_WIDTH;
        base_vdp = vdp_base_names_grp1 + x / 8 + y / 8 * TILE_WIDTH;

	tile[0] = *(base);
	tile[1] = *(base + 1);
        // ignore the above ones
	tile[2] = *(base + TILE_WIDTH);
	tile[3] = *(base + TILE_WIDTH * 2);
	tile[4] = *(base + TILE_WIDTH * 3);
	tile[5] = *(base + TILE_WIDTH + 1);
	tile[6] = *(base + TILE_WIDTH * 2 + 1);
	tile[7] = *(base + TILE_WIDTH * 3 + 1);

	// check for special tiles
	// TODO do this only if there is any collision??
	for (i = 2; i < 8; i++) {
		phys_tile_collision_notify(tile[i]);
	}

        if (dx < 0) {
		if (is_coliding_tile_pair(tile[2], tile[3])) {
			obj->collision_state |= COLLISION_LEFT;
			obj->collision_state &= ~COLLISION_RIGHT;
		} else {
			obj->collision_state &= ~COLLISION_LEFT;
			obj->collision_state &= ~COLLISION_RIGHT;
		}
	}
        if (dx > 0) {
		if (is_coliding_tile_pair(tile[5], tile[6])) {
			obj->collision_state |= COLLISION_RIGHT;
			obj->collision_state &= ~COLLISION_LEFT;
		} else {
			obj->collision_state &= ~COLLISION_RIGHT;
			obj->collision_state &= ~COLLISION_LEFT;
		}
	}
	if (dy < 0) {
		if (is_coliding_tile_pair(tile[2], tile[5])) {
			obj->collision_state |= COLLISION_UP;
		} else {
			obj->collision_state &= ~COLLISION_UP;
		}
	}
	if (is_coliding_down_tile_pair(tile[4], tile[7])) {
		obj->collision_state |= COLLISION_DOWN;
		obj->collision_state &= ~COLLISION_UP;
	}  else {
		obj->collision_state &= ~COLLISION_DOWN;
	}

}

static void phys_detect_tile_collisions_16x16(struct displ_object *obj,
        uint8_t *map, int8_t dx, int8_t dy)
{

	uint8_t x,y;
	uint8_t *base;

	x = obj->xpos;
	y = obj->ypos;

	/* adjust box size  : this may have to be 4 */
	if (x >= 2) x = x - 2;
	if (y >= 2) y = y - 2;

	base = map + x / 8 + y / 8 * TILE_WIDTH;

	tile[0] = *(base);
	tile[1] = *(base + 1);
	tile[2] = *(base + 2);
	tile[3] = *(base + 3);
	tile[4] = *(base + TILE_WIDTH);
	tile[5] = *(base + TILE_WIDTH * 2);
	tile[6] = *(base + TILE_WIDTH * 3);
	tile[7] = *(base + TILE_WIDTH * 3 + 1);
	tile[8] = *(base + TILE_WIDTH * 3 + 2);
	tile[9] = *(base + TILE_WIDTH * 3 + 3);
	tile[10] = *(base + 3 + TILE_WIDTH);
	tile[11] = *(base + 3 + TILE_WIDTH * 2);

	obj->collision_state = 0;
	if (dx < 0 && is_coliding_tile_pair(tile[4], tile[5])) {
		obj->collision_state |= COLLISION_LEFT;
	}
	if (dx > 0 && is_coliding_tile_pair(tile[10], tile[11])) {
		obj->collision_state |= COLLISION_RIGHT;
	}
	if (dy < 0 && is_coliding_tile_pair(tile[1], tile[2])) {
		obj->collision_state |= COLLISION_UP;
	}
	if (dy > 0 && is_coliding_tile_pair(tile[7], tile[8])) {
		obj->collision_state |= COLLISION_DOWN;
	}
}

/*
 * Update collision state of a display object 16x16
 */
void phys_detect_tile_collisions(struct displ_object *obj, uint8_t *map,
	int8_t dx, int8_t dy)
{
       uint8_t size = obj->spr->pattern_set->size;

       if (size == SPR_SIZE_16x16) {
	       phys_detect_tile_collisions_16x16(obj,map, dx, dy);
       } else if (size = SPR_SIZE_16x32) {
	       phys_detect_tile_collisions_16x32(obj, map, dx, dy);
       }
}
