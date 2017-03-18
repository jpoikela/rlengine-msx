#include "msx.h"
#include "sys.h"
#include "vdp.h"
#include "sprite.h"
#include "wq.h"
#include "tile.h"
#include "map.h"
#include "log.h"
#include "displ.h"
#include "phys.h"
#include "list.h"

#include "logic.h"
#include "scene.h"

struct game_state_t game_state;

void init_game_state()
{
        sys_memset(&game_state, 0, sizeof(game_state));

	// room 3
	game_state.map_x = 64;
	game_state.map_y = 44;
}


void pickup_heart(struct displ_object *dpo, uint8_t data)
{
        game_state.hearth[data] = 1;
        game_state.live_cnt++;
        remove_tileobject(dpo);
}


void pickup_scroll(struct displ_object *dpo, uint8_t data)
{
        game_state.scroll[data] = 1;
        remove_tileobject(dpo);
        // TODO: show scroll contents
}
void pickup_cross(struct displ_object *dpo, uint8_t data)
{
        game_state.cross[data] = 1;
        game_state.cross_cnt++;
        remove_tileobject(dpo);
}

void checkpoint_handler(struct displ_object *dpo, uint8_t data)
{
        game_state.checkpoint[data] = 1;
        dpo->tob->cur_anim_step = 1;
        update_tileobject(dpo);
}


void toggle_handler(struct displ_object *dpo, uint8_t data)
{
        game_state.toggle[data] = 1;
        dpo->tob->cur_anim_step = 1;
        update_tileobject(dpo);
}

void bell_handler(struct displ_object *dpo, uint8_t data)
{
        game_state.bell = 1;
        dpo->tob->cur_anim_step = 1;
        update_tileobject(dpo);
}

void crosswitch_handler(struct displ_object *dpo, uint8_t data)
{
        if (!game_state.cross_switch_enable)
                return;

        game_state.cross_switch_enable = false;
        if (game_state.cross_switch) {
                dpo->tob->cur_anim_step = 0;
                game_state.cross_switch = false;
        } else {
                dpo->tob->cur_anim_step = 1;
                game_state.cross_switch = true;
        }
        update_tileobject(dpo);
}
