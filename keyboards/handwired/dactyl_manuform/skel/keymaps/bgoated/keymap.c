#include QMK_KEYBOARD_H

// LAYERS
#define _ALPHA 0
#define _NUM 1
#define _NAV 2

// TODO: Finish alt-tab and shift-alt-tab on nav layer
// TODO: Eventually add a fourth layer on a combo of enter and tab to get the function keys and media keys
// FIXME: LDR just registers an a instead of LCTL( a )
// FIXME: RESET combo doesn't seem to work at all.
// FIXME: COMBO time is too long for DR at least


// Fillers to make layering more clear

#define ____ KC_TRNS

// MOUSE ALIASES
#define KC_ML KC_MS_LEFT
#define KC_MR KC_MS_RIGHT
#define KC_MU KC_MS_UP
#define KC_MD KC_MS_DOWN
#define KC_MB1 KC_MS_BTN1
#define KC_MB2 KC_MS_BTN2
#define KC_MB3 KC_MS_BTN3

// MOD TAPS
#define ESC_CTRL LCTL_T( KC_ESC )
#define SPC_SHFT RSFT_T( KC_SPC )
#define TAB_NUM LT(_NUM,KC_TAB)
#define ENT_NAV LT(_NAV,KC_ENT)

// COMBO KEYS
#define CTL_TAB LCTL(KC_TAB)
#define SCTL_TAB LCTL(LSFT(KC_TAB))
#define CTL_MB1 LCTL(KC_MB1)
#define SFT_MB1 LSFT(KC_MB1)
#define ALT_TAB KC_NO
#define SALT_TAB KC_NO

// COMBOS
const uint16_t PROGMEM combo1[] = { KC_E, KC_D, COMBO_END };
const uint16_t PROGMEM combo2[] = { KC_R, KC_F, COMBO_END };
const uint16_t PROGMEM combo3[] = { KC_D, KC_C, COMBO_END };
const uint16_t PROGMEM combo4[] = { KC_F, KC_V, COMBO_END };
const uint16_t PROGMEM combo5[] = { KC_U, KC_J, COMBO_END };
const uint16_t PROGMEM combo6[] = { KC_I, KC_K, COMBO_END };
const uint16_t PROGMEM combo7[] = { KC_J, KC_M, COMBO_END };
const uint16_t PROGMEM combo8[] = { KC_K, KC_COMMA, COMBO_END };
combo_t key_combos[COMBO_COUNT] = {
    COMBO( combo1, LSFT( KC_LBRACKET ) ),
    COMBO( combo2, LSFT( KC_RBRACKET ) ),
    COMBO( combo3, KC_LBRACKET ),
    COMBO( combo4, KC_RBRACKET ),
    COMBO( combo5, LSFT( KC_9 ) ),
    COMBO( combo6, LSFT( KC_0 ) ),
    COMBO( combo7, KC_QUOT ),
    COMBO( combo8, LSFT( KC_QUOT ) )
};

/// \brief Registers an alternate key code on a shift.
/// \param[in] record  The first argument of process_record_user
/// \param[in] registered  A static bool that holds the status of if the key is pressed or not.
/// \param[in] desired shift  The keycode to register without a shift
bool alternate_shift( keyrecord_t* record, bool* registered, const uint16_t desired_shift )
{
    uint8_t mod_state = get_mods();
    if( record->event.pressed )
    {
        // Detect the activation of either shift keys
        if( mod_state & MOD_MASK_SHIFT )
        {
            // First temporarily canceling both shifts so that
            // shift isn't applied to the KC_DEL keycode
            del_mods( MOD_MASK_SHIFT );
            register_code( desired_shift );
            // Update the boolean variable to reflect the status of KC_DEL
            (*registered) = true;
            // Reapplying modifier state so that the held shift key(s)
            // still work even after having tapped the Backspace/Delete key.
            set_mods(mod_state);
            return false;
        }
    }
    else
    { // on release of the key
        // In case desired_shift is still being sent even after the release of the key
        if( *registered )
        {
            unregister_code( desired_shift );
            (*registered) = false;
            return false;
        }
    }
    // Let QMK process the KC_BSPC keycode as usual outside of shift
    return true;
}

/// Set up a key that is Ctrl+a on tap and Alt on hold.
///
#define LDR LCTL( KC_A )

// Leader key
enum custom_keycodes {
    LDR_ALT = SAFE_RANGE,
};

bool key_pressed = false; // Used to track if a key is pressed while LDR_ALT is held
uint16_t LDR_ALT_timer = 0; // Timer to track hold time for LDR_ALT

bool process_record_user( uint16_t keycode, keyrecord_t* record )
{
    if( record->event.pressed ) key_pressed = true;

    switch( keycode )
    {
/*         case ALT_TAB: */
// Release Alt from alt-tab when leaving the nav layer.  See this code snippet:
// https://github.com/davidagross/qmk_firmware/commit/63208a37c2b88a739591608b85e1fb794406235d#diff-837a0e3a3a20c83ec8d3ace09f5b8d97
// See also QMK Macros advanced example
/*             break; */
        case LDR_ALT:
        {
            if( record->event.pressed )
            {
                register_code( KC_LALT );
                key_pressed = false;
                LDR_ALT_timer = timer_read();
            }
            else
            {
                unregister_code( KC_LALT );
                if( key_pressed )
                {
                    break;
                }

                if( timer_elapsed( LDR_ALT_timer ) < TAPPING_TERM )
                {
                    tap_code16( LDR );
                }
            }
            break;
        }
        // Alternate shift cases
        case KC_BSPC:
        {
            static bool delkey_registered;
            return alternate_shift( record, &delkey_registered, KC_DEL );
            break;
        }
    }
    return true;
}


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Base (qwerty)
 * ,----------------------------------,                ,----------------------------------,
 * |   k  |   m  |   l  |   u  |  /\  |                |   v  |   d  |   r  |   '  |   q  |
 * |------+------+------+------+------|                |------+------+------+------+------|
 * |   a  |   t  |   h  |   e  |  ,.  |                |   c  |   s  |   n  |   o  |   i  |
 * |------+------+------+------+------|                |------|------+------+------+------|
 * |   z  |   p  |   f  |   j  |  ;:  |                |   b  |   g  |   w  |   x  |   y  |
 * ,------+------+------+------+------,                ,------+------+------+------+------,
 *                      | ESC  |                              | LDR  |
 *                      |  +   |                              |  +   |
 *                      | CTRL |                              | ALT  |
 *                      '------+                              +------'
 *                              '------+------' '------+------'
 *                              | BKSP | TAB  | |ENTER |SPACE |
 *                              |   +  |   +  | |  +   |  +   |
 *                              |  DEL |  NUM | |  NAV |SHIFT |
 *                              '------+------' '------+------'
 */

[_ALPHA] = LAYOUT_WITH_RESET( \
  KC_Q, KC_W, KC_E, KC_R, KC_T,                                      KC_Y, KC_U, KC_I,     KC_O,   KC_P, \
  KC_A, KC_S, KC_D, KC_F, KC_G,                                      KC_H, KC_J, KC_K,     KC_L,   KC_SCLN, \
  KC_Z, KC_X, KC_C, KC_V, KC_B,                                      KC_N, KC_M, KC_COMMA, KC_DOT, KC_SLSH, \
  RESET,     ESC_CTRL,  KC_BSPC, TAB_NUM,                     ENT_NAV, SPC_SHFT, LDR_ALT \
),

/* Raise
 * ,----------------------------------,                ,----------------------------------,
 * |RESET |   `  |   +  |  =   |      |                |      |   -  |   _  |   ~  |      |
 * |------+------+------+------+------|                |------+------+------+------+------|
 * |   1  |   2  |   3  |   4  |   5  |                |   6  |   7  |   8  |   9  |   0  |
 * |------+------+------+------+------|                |------|------+------+------+------|
 * |   #  |   @  |   !  |   $  |   %  |                |   ^  |   &  |   *  |   |  |   ?  |
 * ,------+------+------+-------------,                ,-------------+------+------+------,
 *                      | ESC  |                              | LDR  |
 *                      |  +   |                              |  +   |
 *                      | CTRL |                              | ALT  |
 *                      '------+                              +------'
 *                              '------+------' '------+------'
 *                              | BKSP |      | |ENTER |SPACE |
 *                              '------+------' '------+------'
 */

[_NUM] = LAYOUT( \
  RESET,   KC_GRV, KC_PLUS, KC_EQL,  ____,                           ____,    KC_MINS, KC_UNDS, KC_TILD,  ____,    \
  KC_1,    KC_2,   KC_3,    KC_4,    KC_5,                           KC_6,    KC_7,    KC_8,    KC_9,     KC_0,    \
  KC_HASH, KC_AT,  KC_EXLM, KC_DLR,  KC_PERC,                        KC_CIRC, KC_AMPR, KC_ASTR, KC_PIPE,  KC_BSLASH, \
                               ____,  ____, ____,              KC_ENT, KC_SPC, LDR_ALT \
),
/* Lower
 */

[_NAV] = LAYOUT( \
    ____,    SALT_TAB, ALT_TAB, SFT_MB1, ____,                            ____,    KC_HOME, KC_PGDN, KC_PGUP, KC_END,     \
    KC_LGUI, KC_MB2,   KC_MB3,  KC_MB1,  KC_LSFT,                         KC_WH_U, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, \
    ____,    SCTL_TAB, CTL_TAB, CTL_MB1, ____,                            KC_WH_D, KC_ML,   KC_MD,   KC_MU,   KC_MR, \
                          ESC_CTRL,  KC_BSPC, KC_TAB,              ____, ____, ____ \
),
};

void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}
