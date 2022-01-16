#include QMK_KEYBOARD_H

// LAYERS
#define _ALPHA 0
#define _UPPER 1

// Fillers to make layering more clear

#define ____ KC_TRNS

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_ALPHA] = LAYOUT( \
    KC_ESC,   KC_1,   KC_2,   KC_3,  KC_4,    KC_5,                                            KC_6,   KC_7,    KC_8,     KC_9,    KC_0,     KC_MINUS, \
    KC_LALT,  KC_Q,   KC_W,   KC_E,  KC_R,    KC_T,                                            KC_Y,   KC_U,    KC_I,     KC_O,    KC_P,     KC_BSLASH, \
    KC_LCTRL, KC_A,   KC_S,   KC_D,  KC_F,    KC_G,                                            KC_H,   KC_J,    KC_K,     KC_L,    KC_SCLN,  KC_QUOT, \
    KC_LSFT,  KC_Z,   KC_X,   KC_C,  KC_V,    KC_B,   KC_HOME, KC_END,        KC_NO,   KC_DEL, KC_N,   KC_M,    KC_COMMA, KC_DOT,  KC_SLASH, KC_RSFT, \
    RESET,    KC_GRV, KC_EQL, KC_UP, KC_DOWN, KC_SPC, KC_BSPC, KC_PGUP,       KC_CAPS, KC_ENT, KC_SPC, KC_LEFT, KC_RGHT,  KC_LBRC, KC_RBRC, \
                                                               KC_PGDOWN,     MO( 1 ) \
),

[_UPPER] = LAYOUT( \
    RESET, ____, ____, ____, ____, ____,                               ____, KC_MUTE, KC_VOLD, KC_VOLU, ____, ____, \
    ____,  ____, ____, ____, ____, ____,                               ____, ____,    ____,    ____,    ____, ____, \
    ____,  ____, ____, ____, ____, ____,                               ____, ____,    KC_UP,   ____,    ____, ____, \
    ____,  ____, ____, ____, ____, ____, ____, ____,       ____, ____, ____, KC_LEFT, KC_DOWN, KC_RGHT, ____, ____, \
    ____,  ____, ____, ____, ____, ____, ____, ____,       ____, ____, ____, ____,    ____,    ____,    ____, \
                                               ____,       ____ \
    ),

};

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}

/* upper + 789 = vol mute, up, dn
f5 = upper, numlkc = capslck
k,m, angle brackets on upper be up/down/left/right?

Make the alpha up/down be on left hand, left/right on the right hand
 */
