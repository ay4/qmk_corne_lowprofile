/*

██╗███╗   ██╗ ██████╗██╗     ██╗   ██╗██████╗ ███████╗███████╗
██║████╗  ██║██╔════╝██║     ██║   ██║██╔══██╗██╔════╝██╔════╝
██║██╔██╗ ██║██║     ██║     ██║   ██║██║  ██║█████╗  ███████╗
██║██║╚██╗██║██║     ██║     ██║   ██║██║  ██║██╔══╝  ╚════██║
██║██║ ╚████║╚██████╗███████╗╚██████╔╝██████╔╝███████╗███████║
╚═╝╚═╝  ╚═══╝ ╚═════╝╚══════╝ ╚═════╝ ╚═════╝ ╚══════╝╚══════╝

*/


#include QMK_KEYBOARD_H
#include "russian_arno.h"
#include "symbols.h"

/*
██╗      █████╗ ██╗   ██╗███████╗██████╗ ███████╗
██║     ██╔══██╗╚██╗ ██╔╝██╔════╝██╔══██╗██╔════╝
██║     ███████║ ╚████╔╝ █████╗  ██████╔╝███████╗
██║     ██╔══██║  ╚██╔╝  ██╔══╝  ██╔══██╗╚════██║
███████╗██║  ██║   ██║   ███████╗██║  ██║███████║
╚══════╝╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═╝  ╚═╝╚══════╝
*/


#define _EN  0 // English layer
#define _RU  1 // Russian layer
#define _SYM 2 // Punctiation
#define _QW  3 // QWERTY for hotkeys
#define _NAV 4 // Navigation around the text
#define _NUM 5 // Numbers


/*
███╗   ███╗ █████╗  ██████╗██████╗  ██████╗ ███████╗
████╗ ████║██╔══██╗██╔════╝██╔══██╗██╔═══██╗██╔════╝
██╔████╔██║███████║██║     ██████╔╝██║   ██║███████╗
██║╚██╔╝██║██╔══██║██║     ██╔══██╗██║   ██║╚════██║
██║ ╚═╝ ██║██║  ██║╚██████╗██║  ██║╚██████╔╝███████║
╚═╝     ╚═╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝
*/

#define RU_Q LT(0, KC_Q)         // Switches ro _RU when held, Q on tap
#define EN_ESC LT(0, KC_ESC)     // Switches to _EN when held, Escape when tapped
#define RU_SYM LT(0, KC_SPC)     // Switches to _EN and to the _SYM layer to produce symbols
#define ST_SH OSM(MOD_LSFT)      // Sticky shift
#define CTL_ESC CTL_T(KC_ESC)    // Ctrl on hold, Esc on tap
#define CMD_QW LT(0, KC_LCMD)    // When held, switches to English and to _QW,
                                 // while holding Command. When released switches back
                                 // to _RU. When tapped, just Command.
#define ALT_QW LT(0, KC_LALT)    // ^ Same for Option.


enum custom_keycodes {
  TO_RU = SAFE_RANGE
};

//
// These four combos implement three-finger presses in home
// position. The combo on the left side will produce cmd-shift-space
// which in my current setup is matched to the Hints mode of the
// piece of software called Homerow.app (Mac).
// The same on the right will have the software enter
// the scrolling mode. I need four combos because of
// two language layouts.
//
const uint16_t PROGMEM hints_en[] = {KC_I, KC_E, KC_A, COMBO_END};
const uint16_t PROGMEM hints_ru[] = {KC_B, KC_T, KC_F, COMBO_END};
const uint16_t PROGMEM scroll_en[] = {KC_H, KC_T, KC_S, COMBO_END};
const uint16_t PROGMEM scroll_ru[] = {KC_Y, KC_N, KC_C, COMBO_END};

combo_t key_combos[] = {
    COMBO(hints_en, LCMD(S(KC_SPC))),
    COMBO(hints_ru, LCMD(S(KC_SPC))),
    COMBO(scroll_en, LCMD(S(KC_J))),
    COMBO(scroll_ru, LCMD(S(KC_J))),
};


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {



        // A macro that switches to layer _RU when held
        // and presses Q when tapped
        case RU_Q:
            if (record->tap.count && record->event.pressed) {
                tap_code16(KC_Q);
            } else if (record->event.pressed) {
              SEND_STRING(SS_LCMD(SS_LALT("2")));
              layer_move(_RU);
            }
            return false;


        // A macro that switches to layer _EN when held
        // and presses ESC when tapped
        case EN_ESC:
            if (record->tap.count && record->event.pressed) {
                tap_code16(KC_ESC);
            } else if (record->event.pressed) {
              SEND_STRING(SS_LCMD(SS_LALT("1")));
              layer_move(_EN);
            }
            return false;

        // A macro that, whn held, changes the software language
        // from RU to EN, switches to the punctuation layer,
        // allows to type some characters in there and then
        // once released switches back from EN to RU
          case RU_SYM:
            if (record->tap.count && record->event.pressed) { // When simply tapped
                tap_code16(KC_SPC);
            } else if (record->event.pressed) {
              SEND_STRING(SS_LCMD(SS_LALT("1")));            //When held
              layer_move(_SYM);
            } else if (!record->event.pressed) {
              SEND_STRING(SS_LCMD(SS_LALT("2")));            //When released
              layer_move(_RU);
            }
            return false;

        // When held, switches to English and to _QW,
        // while holding Command. When released switches back
        // to _RU. When tapped, just Command.
          case CMD_QW:
            if (record->tap.count && record->event.pressed) { // When simply tapped
                tap_code16(KC_LCMD);
            } else if (record->event.pressed) {
              SEND_STRING(SS_LCMD(SS_LALT("1")));            //When held
              layer_move(_QW);
              register_code16(KC_LCMD);
            } else if (!record->event.pressed) {
              unregister_code16(KC_LCMD);
              SEND_STRING(SS_LCMD(SS_LALT("2")));            //When released
              layer_move(_RU);
            }
            return false;

        // Same as above, but for alt
          case ALT_QW:
            if (record->tap.count && record->event.pressed) { // When simply tapped
                tap_code16(KC_LALT);
            } else if (record->event.pressed) {
              SEND_STRING(SS_LCMD(SS_LALT("1")));            //When held
              layer_move(_QW);
              register_code16(KC_LALT);
            } else if (!record->event.pressed) {
              unregister_code16(KC_LALT);
              SEND_STRING(SS_LCMD(SS_LALT("2")));            //When released
              layer_move(_RU);
            }
            return false;
    }
    return true;
};

/*
██╗      █████╗ ██╗   ██╗███████╗██████╗ ███████╗
██║     ██╔══██╗╚██╗ ██╔╝██╔════╝██╔══██╗██╔════╝
██║     ███████║ ╚████╔╝ █████╗  ██████╔╝███████╗
██║     ██╔══██║  ╚██╔╝  ██╔══╝  ██╔══██╗╚════██║
███████╗██║  ██║   ██║   ███████╗██║  ██║███████║
╚══════╝╚═╝  ╚═╝   ╚═╝   ╚══════╝╚═╝  ╚═╝╚══════╝
*/


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_split_3x6_3(
/*
┌───────┬───────┬───────┬───────┬───────┬───────┐                        ┌───────┬───────┬───────┬───────┬───────┬───────┐
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
│  tab  │   B   │   Y   │   O   │   U   │       │░                       │       │   L   │   D   │   W   │   Z   │       │░
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
├───────┼───────┼───────┼───────┼───────┼───────┤░                       ├───────┼───────┼───────┼───────┼───────┼───────┤░
│  CTRL │       │       │       │       │       │░                       │       │       │       │       │       │  RUS  │░
│  ---  │   C   │   I   │   E   │   A   │       │░                       │   V   │   H   │   T   │   S   │   N   │  ---  │░
│  esc  │       │       │       │       │       │░                       │       │       │       │       │       │   Q   │░
├───────┼───────┼───────┼───────┼───────┼───────┤░                       ├───────┼───────┼───────┼───────┼───────┼───────┤░
│ stick │       │       │       │       │       │░                       │       │       │       │       │       │ stick │░
│ shift │   G   │   X   │   J   │   K   │       │░                       │       │   R   │   M   │   F   │   P   │ shift │░
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
└───────┴───────┴───────┴───────┴───────┴───────┘░                       └───────┴───────┴───────┴───────┴───────┴───────┘░
 ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░┌───────┐      ┌───────┐ ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
                                ┌───────┬───────┐│       │░     │       │░┌───────┬───────┐
                                │       │       ││ _SYM  │░     │       │░│       │       │░
                                │ ⌥ + Q │ ⌘ + Q ││  ---  │░     │ bkspc │░│  NAV  │  NUM  │░
                                │       │       ││ space │░     │       │░│       │       │░
                                └───────┴───────┘│       │░     │       │░└───────┴───────┘░
                                 ░░░░░░░░░░░░░░░░│       │░     │       │░ ░░░░░░░░░░░░░░░░░
                                                 └───────┘░     └───────┘░
                                                  ░░░░░░░░░      ░░░░░░░░░
*/

  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_TAB,    KC_B,   KC_Y,    KC_O,    KC_U,  XXXXXXX,                       XXXXXXX,   KC_L,    KC_D,    KC_W,   KC_Z,  XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      CTL_ESC,   KC_C,   KC_I,    KC_E,    KC_A,  XXXXXXX,                        KC_V,    KC_H,    KC_T,    KC_S,   KC_N,    RU_Q,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      ST_SH,    KC_G,    KC_X,    KC_J,    KC_K,  XXXXXXX,                       XXXXXXX,   KC_R,    KC_M,    KC_F,   KC_P,   ST_SH,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
              LM(_QW, MOD_LALT), LM(_QW, MOD_LGUI), LT(_SYM, KC_SPC),   KC_BSPC,  MO(_NAV),   MO(_NUM)
                                      //`--------------------------'  `--------------------------

  ),

/*
┌───────┬───────┬───────┬───────┬───────┬───────┐                        ┌───────┬───────┬───────┬───────┬───────┬───────┐
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
│  +++  │   Б   │   Ы   │   О   │   У   │   Э   │░                       │   Ц   │   Л   │   Д   │   З   │   Я   │   Ъ   │░
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
├───────┼───────┼───────┼───────┼───────┼───────┤░                       ├───────┼───────┼───────┼───────┼───────┼───────┤░
│  ENG  │       │       │       │       │       │░                       │       │       │       │       │       │       │░
│  ---  │   Ч   │   И   │   Е   │   А   │   Й   │░                       │   В   │   Н   │   Т   │   С   │   Ь   │   Ю   │░
│  esc  │       │       │       │       │       │░                       │       │       │       │       │       │       │░
├───────┼───────┼───────┼───────┼───────┼───────┤░                       ├───────┼───────┼───────┼───────┼───────┼───────┤░
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
│  +++  │   Г   │   Ш   │   Ж   │   К   │   Щ   │░                       │   Х   │   Р   │   М   │   Ф   │   П   │  +++  │░
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
└───────┴───────┴───────┴───────┴───────┴───────┘░                       └───────┴───────┴───────┴───────┴───────┴───────┘░
 ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░┌───────┐      ┌───────┐ ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
                                ┌───────┬───────┐│       │░     │       │░┌───────┬───────┐
                                │       │       ││RU_SYM │░     │       │░│       │       │░
                                │ ⌥ + Q │ ⌘ + Q ││  ---  │░     │ bkspc │░│  +++  │  +++  │░
                                │       │       ││ space │░     │       │░│       │       │░
                                └───────┴───────┘│       │░     │       │░└───────┴───────┘░
                                 ░░░░░░░░░░░░░░░░│       │░     │       │░ ░░░░░░░░░░░░░░░░░
                                                 └───────┘░     └───────┘
                                                  ░░░░░░░░░      ░░░░░░░░░
*/

    [1] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______, RU_BE,   RU_Y,    RU_O,    RU_U,    RU_UE,                       RU_TS,    RU_L,    RU_D,    RU_Z,    RU_YA,  RU_HARD,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      EN_ESC,  RU_CH,   RU_I,    RU_E,    RU_A,    RU_J,                         RU_V,    RU_N,    RU_T,    RU_S,    RU_SOFT,   RU_YU,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, RU_G,    RU_SH,   RU_ZH,   RU_K,    RU_SCH,                       RU_KH,   RU_R,    RU_M,    RU_F,    RU_PE,  KC_TRNS,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          ALT_QW,  CMD_QW,   RU_SYM,      KC_BSPC, _______, _______
                                      //`--------------------------'  `--------------------------'
  ),

/*
┌───────┬───────┬───────┬───────┬───────┬───────┐                        ┌───────┬───────┬───────┬───────┬───────┬───────┐
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
│       │   $   │   _   │   [   │   -   │ mdash │░                       │   /   │   !   │   ]   │   *   │   %   │       │░
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
├───────┼───────┼───────┼───────┼───────┼───────┤░                       ├───────┼───────┼───────┼───────┼───────┼───────┤░
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
│       │   @   │   +   │   (   │   ,   │   ;   │░                       │   :   │   .   │   )   │   #   │   &   │       │░
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
├───────┼───────┼───────┼───────┼───────┼───────┤░                       ├───────┼───────┼───────┼───────┼───────┼───────┤░
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
│       │   =   │   <   │   {   │   '   │   "   │░                       │   ~   │   ?   │   }   │   >   │   =   │       │░
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
└───────┴───────┴───────┴───────┴───────┴───────┘░                       └───────┴───────┴───────┴───────┴───────┴───────┘░
 ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░┌───────┐      ┌───────┐ ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
                                ┌───────┬───────┐│░░░░░░░│      │       │░┌───────┬───────┐
                                │       │       ││░░░░░░░│      │       │░│       │       │░
                                │       │       ││░░░░░░░│      │  RET  │░│       │       │░
                                │       │       ││░░░░░░░│      │       │░│       │       │░
                                └───────┴───────┘│░░░░░░░│      │       │░└───────┴───────┘░
                                 ░░░░░░░░░░░░░░░░│░░░░░░░│      │       │░ ░░░░░░░░░░░░░░░░░
                                                 └───────┘      └───────┘░
                                                                 ░░░░░░░░░

*/

    [2] = LAYOUT_split_3x6_3(
  //,---------------------------------------------------------------------.         ,----------------------------------------------------------------.
      XXXXXXX, AY_DLR,    AY_UNDR,    AY_LBRC,   AY_MINS,    AY_MDASH,                 AY_SLSH,   AY_EXCL,    AY_RBRC,    AY_STAR,  AY_PERC,  XXXXXXX,
  //|--------+-----------+-----------+---------+-----------+--------------|         |-----------+-----------+-----------+----------+--------+--------|
      XXXXXXX, AY_AT,     AY_PLUS,    AY_LPRN,   AY_COMM,    AY_SCLN,                  AY_CLN,    AY_DOT,     AY_RPRN,    AY_HASH,  AY_AMPER, XXXXXXX,
  //|--------+-----------+-----------+---------+-----------+--------------|         |-----------+-----------+-----------+----------+--------+--------|
      XXXXXXX, AY_EQL,    AY_LCORN,   AY_LCURL,  AY_APOS,    AY_QUOTE,                 AY_TILDE,  AY_QUEST,   AY_RCURL,   AY_RCORN, AY_EQL,   XXXXXXX,
  //|--------+-----------+-----------+---------+-----------+--------------|         |-----------+-----------+-----------+----------+--------+--------|
                                             XXXXXXX,   XXXXXXX,  XXXXXXX,          KC_ENT, XXXXXXX, XXXXXXX
                                             //`--------------------------'         `--------------------------'
  ),


/*
┌───────┬───────┬───────┬───────┬───────┬───────┐                        ┌───────┬───────┬───────┬───────┬───────┬───────┐
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
│  +++  │   Q   │   W   │   E   │   R   │   T   │░                       │   Y   │   U   │   I   │   O   │   P   │   [   │░
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
├───────┼───────┼───────┼───────┼───────┼───────┤░                       ├───────┼───────┼───────┼───────┼───────┼───────┤░
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
│  +++  │   A   │   S   │   D   │   F   │   G   │░                       │   H   │   J   │   K   │   L   │   ;   │   '   │░
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
├───────┼───────┼───────┼───────┼───────┼───────┤░                       ├───────┼───────┼───────┼───────┼───────┼───────┤░
│ norm. │       │       │       │       │       │░                       │       │       │       │       │       │       │░
│ shift │   Z   │   X   │   C   │   V   │   B   │░                       │   N   │   M   │   ,   │   .   │   /   │       │░
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
└───────┴───────┴───────┴───────┴───────┴───────┘░                       └───────┴───────┴───────┴───────┴───────┴───────┘░
 ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░┌───────┐      ┌───────┐ ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
                                ┌───────┬───────┐│       │░     │       │░┌───────┬───────┐
                                │       │       ││       │░     │       │░│       │       │░
                                │   ⌥   │   ⌘   ││  +++  │░     │  +++  │░│  +++  │  +++  │░
                                │       │       ││       │░     │       │░│       │       │░
                                └───────┴───────┘│       │░     │       │░└───────┴───────┘░
                                 ░░░░░░░░░░░░░░░░│       │░     │       │░ ░░░░░░░░░░░░░░░░░
                                                 └───────┘░     └───────┘░
                                                  ░░░░░░░░░      ░░░░░░░░░
*/

    [3] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
     _______,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,   KC_LBRC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L,   KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M,   KC_COMM,  KC_DOT, KC_SLSH, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LALT, KC_LCMD, _______,     _______, _______, _______
                                      //`--------------------------'  `--------------------------'
  ),

/*
┌───────┬───────┬───────┬───────┬───────┬───────┐                        ┌───────┬───────┬───────┬───────┬───────┬───────┐
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
│       │       │       │       │       │       │░                       │       │ Tab←  │   ↑   │ Tab→  │       │       │░
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
├───────┼───────┼───────┼───────┼───────┼───────┤░                       ├───────┼───────┼───────┼───────┼───────┼───────┤░
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
│       │       │       │       │       │       │░                       │  ⌥←   │   ←   │   ↓   │   →   │  ⌥→   │       │░
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
├───────┼───────┼───────┼───────┼───────┼───────┤░                       ├───────┼───────┼───────┼───────┼───────┼───────┤░
│ norm. │       │       │       │       │       │░                       │       │       │       │       │       │       │░
│ shift │       │       │       │       │       │░                       │       │       │       │       │       │       │░
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
└───────┴───────┴───────┴───────┴───────┴───────┘░                       └───────┴───────┴───────┴───────┴───────┴───────┘░
 ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░┌───────┐      ┌───────┐ ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
                                ┌───────┬───────┐│       │░     │       │░┌───────┬───────┐
                                │       │       ││       │░     │       │░│░░░░░░░│       │░
                                │   ⌥   │   ⌘   ││       │░     │       │░│░░░░░░░│       │░
                                │       │       ││       │░     │       │░│░░░░░░░│       │░
                                └───────┴───────┘│       │░     │       │░└───────┴───────┘░
                                 ░░░░░░░░░░░░░░░░│       │░     │       │░ ░░░░░░░░░░░░░░░░░
                                                 └───────┘░     └───────┘░
                                                  ░░░░░░░░░      ░░░░░░░░░
*/

    [4] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-------------------------------------------------------------------------.
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX,    G(S(KC_LBRC)), KC_UP,    G(S(KC_RBRC)),  XXXXXXX,    XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |-----------+--------------+--------+---------------+------------+--------|
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      A(KC_LEFT), KC_LEFT,       KC_DOWN,  KC_RIGHT,       A(KC_RIGHT), XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |-----------+--------------+--------+---------------+------------+--------|
     KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX,    XXXXXXX,       XXXXXXX,  S(A(KC_RIGHT)), XXXXXXX,     XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+-----------+--------------+--------+---------------+------------+--------|
                                          KC_LALT, KC_LCMD, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX
                                      //`--------------------------'  `--------------------------'
  ),

/*
┌───────┬───────┬───────┬───────┬───────┬───────┐                        ┌───────┬───────┬───────┬───────┬───────┬───────┐
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
│       │       │       │       │       │       │░                       │       │       │       │   1   │   2   │   3   │░
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
├───────┼───────┼───────┼───────┼───────┼───────┤░                       ├───────┼───────┼───────┼───────┼───────┼───────┤░
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
│       │       │       │       │       │       │░                       │       │       │       │   4   │   5   │   6   │░
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
├───────┼───────┼───────┼───────┼───────┼───────┤░                       ├───────┼───────┼───────┼───────┼───────┼───────┤░
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
│       │       │       │       │       │       │░                       │       │       │   0   │   7   │   8   │   9   │░
│       │       │       │       │       │       │░                       │       │       │       │       │       │       │░
└───────┴───────┴───────┴───────┴───────┴───────┘░                       └───────┴───────┴───────┴───────┴───────┴───────┘░
 ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░┌───────┐      ┌───────┐ ░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░
                                ┌───────┬───────┐│       │░     │       │░┌───────┬───────┐
                                │       │       ││       │░     │       │░│       │░░░░░░░│░
                                │       │       ││       │░     │       │░│       │░░░░░░░│░
                                │       │       ││       │░     │       │░│       │░░░░░░░│░
                                └───────┴───────┘│       │░     │       │░└───────┴───────┘░
                                 ░░░░░░░░░░░░░░░░│       │░     │       │░ ░░░░░░░░░░░░░░░░░
                                                 └───────┘░     └───────┘░
                                                  ░░░░░░░░░      ░░░░░░░░░
*/

    [5] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX,   KC_1,    KC_2,    KC_3,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX,   KC_4,    KC_5,    KC_6,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, KC_0,      KC_7,    KC_8,    KC_9,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX
                                      //`--------------------------'  `--------------------------'
  )
};
