#ifndef _LOGIC_H_
#define _LOGIC_H_

struct game_state_t {

  /* current room */
  uint8_t room;
  bool change_room;

  /* jean position when swithing rooms */
  int16_t jean_x;
  int16_t jean_y;
  uint8_t jean_state;
  uint8_t jean_collision_state;
  uint8_t jean_anim_state;

  /* check point saved position */
  int16_t checkpoint_x;
  int16_t checkpoint_y;
  uint8_t checkpoint_room;

  /* cross and live count for state panel */
  uint8_t cross_cnt;

#define GAME_MAX_LIVES 9
  uint8_t live_cnt;

  /* action item status */
  uint8_t hearth[9];
  uint8_t scroll[8];

  // some of the crosses are inverted...
  // 1,2,3, 4, 5, 6, 7, 8 normal
  // 9, 10, 11, 12  inverted
  uint8_t cross[12];
  bool bell;
  uint8_t invisible_trigger[5];
  uint8_t checkpoint[8];
  uint8_t toggle[3];
  bool cross_switch;
  bool door_trigger;
  uint8_t templar_ct;
  uint8_t templar_delay;
  bool death;
  bool refresh_score;
  bool cup_picked_up;
  uint8_t show_parchment;
  uint8_t red_parchment;
  bool start_ending_seq;
  bool start_bonfire_seq;
  bool final_animation;
  bool teletransport;
  bool final_fight;
};

enum trigger_ids {
  TRIGGER_ENTRANCE_DOOR = 2,
};

extern struct game_state_t game_state;

void null_handler(DisplayObject *dpo, uint8_t data);
void pickup_heart(DisplayObject *dpo, uint8_t data);
void pickup_scroll(DisplayObject *dpo, uint8_t data);
void pickup_red_scroll(DisplayObject *dpo, uint8_t data);
void pickup_cross(DisplayObject *dpo, uint8_t data);
void checkpoint_handler(DisplayObject *dpo, uint8_t data);
void toggle_handler(DisplayObject *dpo, uint8_t data);
void bell_handler(DisplayObject *dpo, uint8_t data);
void crosswitch_handler(DisplayObject *dpo, uint8_t data);
void trigger_handler(DisplayObject *dpo, uint8_t data);
void deadly_tile_handler(DisplayObject *dpo, uint8_t data);
void cup_handler(DisplayObject *dpo, uint8_t data);
void teletransport(DisplayObject *dpo, uint8_t data);
#endif
