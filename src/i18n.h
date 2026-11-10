#pragma once

#include <stdint.h>

typedef enum {
    LANG_EN = 0,
    LANG_UA = 1,
    LANG_COUNT = 2,
} ui_language_t;

ui_language_t i18n_get_language(void);
void i18n_set_language(ui_language_t lang);
const char *i18n_language_name(ui_language_t lang);

typedef enum {
    // Top headers & navigation
    I18N_STR_MULTIMETER,
    I18N_STR_OSCILLOSCOPE,
    I18N_STR_SIGNAL_GENERATOR,
    I18N_STR_SETTINGS,
    I18N_STR_MENU,

    // Settings screen
    I18N_STR_SETTING_BEEP,
    I18N_STR_SETTING_DISPLAY,
    I18N_STR_SETTING_START,
    I18N_STR_SETTING_SLEEP,
    I18N_STR_SETTING_LANGUAGE,
    I18N_STR_SETTING_BOOTLOADER,
    I18N_STR_SETTING_INFO,
    I18N_STR_EXPORT,

    // Common actions & state
    I18N_STR_RUN,
    I18N_STR_HOLD,
    I18N_STR_ROLL,
    I18N_STR_START,
    I18N_STR_STOP,
    I18N_STR_VALUE,
    I18N_STR_PARAM,
    I18N_STR_OFF,
    I18N_STR_ON,
    I18N_STR_WAIT,

    // Overlays: Firmware update
    I18N_STR_FW_UPDATE,
    I18N_STR_FW_RECEIVING,
    I18N_STR_FW_READY,
    I18N_STR_FW_FLASHING,
    I18N_STR_FW_NOT_FW,
    I18N_STR_FW_TOO_LARGE,
    I18N_STR_FW_ERROR,

    // Overlays: Screenshot
    I18N_STR_SCREENSHOT,
    I18N_STR_SAVING_TO_USB,
    I18N_STR_SAVED_TO_USB,
    I18N_STR_SAVE_ERROR,

    // Overlays: Bootloader export
    I18N_STR_BOOT_EXPORT,
    I18N_STR_BOOT_EJECT_FIRST,
    I18N_STR_BOOT_REPLACES_BACKUP,
    I18N_STR_BOOT_PROMPT,
    I18N_STR_BOOT_READING_VERIFYING,
    I18N_STR_BOOT_ERR_VERIFIED,
    I18N_STR_BOOT_ERR_BUSY,
    I18N_STR_BOOT_ERR_PROTECTED,
    I18N_STR_BOOT_ERR_VECTORS,
    I18N_STR_BOOT_ERR_STORAGE,
    I18N_STR_BOOT_ERR_SPACE,
    I18N_STR_BOOT_ERR_VERIFY,

    // Scope Math / Bode / FFT menu
    I18N_STR_MATH_BODE_MENU,
    I18N_STR_PLOT_MODE,
    I18N_STR_FFT_WINDOW,
    I18N_STR_FFT_DISPLAY,
    I18N_STR_MATH_MODE,
    I18N_STR_MATH_OP,
    I18N_STR_HIDE_TRACES,
    I18N_STR_START_FREQ,
    I18N_STR_STOP_FREQ,
    I18N_STR_STEPS,
    I18N_STR_SWEEP,
    I18N_STR_BODE_DONE,
    I18N_STR_RUNNING,
    I18N_STR_RESWEEP,

    // Scope move & calibration
    I18N_STR_MOVE,
    I18N_STR_Y_POS,
    I18N_STR_X_POS,
    I18N_STR_RESET,
    I18N_STR_ZERO,
    I18N_STR_BASELINE_CAL,
    I18N_STR_REMOVE_PROBE_WAIT,

    // Signal generator sweep & FM menu
    I18N_STR_SWEEP_MODULATION,
    I18N_STR_SWEEP_MODE,
    I18N_STR_SWEEP_TIME,
    I18N_STR_FM_MODE,
    I18N_STR_FM_WAVE,
    I18N_STR_FM_FREQ,
    I18N_STR_NO_FILE,

    // DMM alerts & status
    I18N_STR_LIVE_WIRE,
    I18N_STR_DANGER_V,
    I18N_STR_HIGH_V,
    I18N_STR_USE_HI_JACK,
    I18N_STR_USE_LO_JACK,
    I18N_STR_AUTO_MODE,
    I18N_STR_MANUAL_MODE,
    I18N_STR_REL,
    I18N_STR_OPEN,
    I18N_STR_LIVE,
    I18N_STR_AUTO,
    I18N_STR_DMM_MIN,
    I18N_STR_DMM_MAX,
    I18N_STR_DMM_MAX_MIN,
    I18N_STR_OVER,
    I18N_STR_NEG_OVER,

    I18N_STR_TOTAL_COUNT,
} i18n_str_id_t;

const char *i18n_str(i18n_str_id_t id);

const char *i18n_dmm_unit(const char *raw_unit);

const char *i18n_mode_short(uint8_t mode);
const char *i18n_menu_label(uint8_t index);
const char *i18n_settings_row_label(uint8_t index);
const char *i18n_startup_label(uint8_t index);
const char *i18n_beep_level_label(uint8_t index);
const char *i18n_brightness_level_label(uint8_t index);
const char *i18n_sleep_label(uint8_t index);

const char *i18n_dmm_mode_name(uint8_t index);
const char *i18n_dmm_mode_short(uint8_t index);
const char *i18n_dmm_group_label(uint8_t group);

const char *i18n_scope_display_label(uint8_t index);
const char *i18n_scope_cursor_label(uint8_t index);
const char *i18n_scope_cursor_short(uint8_t index);
const char *i18n_scope_trigger_short(uint8_t index);
const char *i18n_scope_param_label(uint8_t index);
const char *i18n_scope_trigger_edge_label(uint8_t index);

const char *i18n_gen_wave_label(uint8_t index);
const char *i18n_gen_wave_short_label(uint8_t index);
const char *i18n_gen_param_label(uint8_t index);
const char *i18n_gen_sweep_mode_name(uint8_t index);
const char *i18n_gen_fm_mode_name(uint8_t index);
const char *i18n_gen_fm_wave_name(uint8_t index);

const char *i18n_fft_status_text(uint8_t index);
const char *i18n_fft_window_text(uint8_t index);
const char *i18n_fft_display_text(uint8_t index);
const char *i18n_math_status_text(uint8_t index);
const char *i18n_math_op_text(uint8_t index);
const char *i18n_hide_traces_text(uint8_t index);

const char *i18n_boot_export_error(uint8_t index);
const char *i18n_fw_update_status(uint8_t index);
