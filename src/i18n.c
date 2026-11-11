#include "i18n.h"

static ui_language_t current_language = LANG_EN;

ui_language_t i18n_get_language(void) {
    return current_language;
}

void i18n_set_language(ui_language_t lang) {
    if (lang < LANG_COUNT) {
        current_language = lang;
    }
}

const char *i18n_language_name(ui_language_t lang) {
    static const char *const names[LANG_COUNT] = {
        [LANG_EN] = "ENGLISH",
        [LANG_UA] = "УКРАЇНСЬКА",
    };
    if (lang < LANG_COUNT) {
        return names[lang];
    }
    return names[LANG_EN];
}

static const char *const i18n_tbl_strings[LANG_COUNT][I18N_STR_TOTAL_COUNT] = {
    [LANG_EN] = {
        [I18N_STR_MULTIMETER]         = "MULTIMETER",
        [I18N_STR_OSCILLOSCOPE]       = "OSCILLOSCOPE",
        [I18N_STR_SIGNAL_GENERATOR]   = "SIGNAL GENERATOR",
        [I18N_STR_SETTINGS]           = "SETTINGS",
        [I18N_STR_MENU]               = "MENU",

        [I18N_STR_SETTING_BEEP]       = "BEEP",
        [I18N_STR_SETTING_DISPLAY]    = "DISPLAY",
        [I18N_STR_SETTING_START]      = "START",
        [I18N_STR_SETTING_SLEEP]      = "SLEEP",
        [I18N_STR_SETTING_LANGUAGE]   = "LANGUAGE",
        [I18N_STR_SETTING_BOOTLOADER] = "BOOTLOADER",
        [I18N_STR_SETTING_INFO]       = "INFO",
        [I18N_STR_EXPORT]             = "EXPORT",

        [I18N_STR_RUN]                = "RUN",
        [I18N_STR_HOLD]               = "HOLD",
        [I18N_STR_ROLL]               = "ROLL",
        [I18N_STR_START]              = "START",
        [I18N_STR_STOP]               = "STOP",
        [I18N_STR_VALUE]              = "VALUE",
        [I18N_STR_PARAM]              = "PARAM",
        [I18N_STR_OFF]                = "OFF",
        [I18N_STR_ON]                 = "ON",
        [I18N_STR_WAIT]               = "WAIT",

        [I18N_STR_FW_UPDATE]          = "FIRMWARE UPDATE",
        [I18N_STR_FW_RECEIVING]       = "RECEIVING",
        [I18N_STR_FW_READY]           = "READY TO FLASH",
        [I18N_STR_FW_FLASHING]        = "FLASHING",
        [I18N_STR_FW_NOT_FW]          = "NOT A FIRMWARE FILE",
        [I18N_STR_FW_TOO_LARGE]       = "FILE TOO LARGE",
        [I18N_STR_FW_ERROR]           = "UPDATE ERROR",

        [I18N_STR_SCREENSHOT]         = "SCREENSHOT",
        [I18N_STR_SAVING_TO_USB]      = "SAVING TO USB",
        [I18N_STR_SAVED_TO_USB]       = "SAVED TO USB",
        [I18N_STR_SAVE_ERROR]         = "SAVE ERROR",

        [I18N_STR_BOOT_EXPORT]            = "BOOTLOADER EXPORT",
        [I18N_STR_BOOT_EJECT_FIRST]       = "EJECT USB DRIVE FIRST",
        [I18N_STR_BOOT_REPLACES_BACKUP]   = "REPLACES EXISTING BACKUP",
        [I18N_STR_BOOT_PROMPT]            = "PLAY: EXPORT   MENU: CANCEL",
        [I18N_STR_BOOT_READING_VERIFYING] = "READING AND VERIFYING",
        [I18N_STR_BOOT_ERR_VERIFIED]      = "SAVED AND VERIFIED",
        [I18N_STR_BOOT_ERR_BUSY]          = "STORAGE BUSY - RETRY",
        [I18N_STR_BOOT_ERR_PROTECTED]     = "BOOTLOADER READ PROTECTED",
        [I18N_STR_BOOT_ERR_VECTORS]       = "INVALID BOOTLOADER VECTORS",
        [I18N_STR_BOOT_ERR_STORAGE]       = "STORAGE WRITE ERROR",
        [I18N_STR_BOOT_ERR_SPACE]         = "NOT ENOUGH FREE SPACE",
        [I18N_STR_BOOT_ERR_VERIFY]        = "BACKUP VERIFY ERROR",

        [I18N_STR_MATH_BODE_MENU]     = "MATH / BODE MENU",
        [I18N_STR_PLOT_MODE]          = "PLOT MODE:",
        [I18N_STR_FFT_WINDOW]         = "FFT WINDOW:",
        [I18N_STR_FFT_DISPLAY]        = "FFT DISPLAY:",
        [I18N_STR_MATH_MODE]          = "MATH MODE:",
        [I18N_STR_MATH_OP]            = "MATH OP:",
        [I18N_STR_HIDE_TRACES]        = "HIDE TRACES:",
        [I18N_STR_START_FREQ]         = "START FREQ:",
        [I18N_STR_STOP_FREQ]          = "STOP FREQ:",
        [I18N_STR_STEPS]              = "STEPS:",
        [I18N_STR_SWEEP]              = "SWEEP:",
        [I18N_STR_BODE_DONE]          = "BODE DONE",
        [I18N_STR_RUNNING]            = "RUNNING",
        [I18N_STR_RESWEEP]            = "RE-SWEEP",

        [I18N_STR_MOVE]               = "MOVE",
        [I18N_STR_Y_POS]              = "Y POS",
        [I18N_STR_X_POS]              = "X POS",
        [I18N_STR_RESET]              = "RESET",
        [I18N_STR_ZERO]               = "ZERO",
        [I18N_STR_BASELINE_CAL]       = "BASELINE CAL",
        [I18N_STR_REMOVE_PROBE_WAIT]  = "REMOVE PROBE  WAIT",

        [I18N_STR_SWEEP_MODULATION]   = "SWEEP AND MODULATION",
        [I18N_STR_SWEEP_MODE]         = "SWEEP MODE:",
        [I18N_STR_SWEEP_TIME]         = "SWEEP TIME:",
        [I18N_STR_FM_MODE]            = "FM MODE:",
        [I18N_STR_FM_WAVE]            = "FM WAVE:",
        [I18N_STR_FM_FREQ]            = "FM FREQ:",
        [I18N_STR_NO_FILE]            = "NO FILE",

        [I18N_STR_LIVE_WIRE]          = "LIVE WIRE",
        [I18N_STR_DANGER_V]           = "DANGER V",
        [I18N_STR_HIGH_V]             = "HIGH V",
        [I18N_STR_USE_HI_JACK]        = "USE HI JACK",
        [I18N_STR_USE_LO_JACK]        = "USE LO JACK",
        [I18N_STR_AUTO_MODE]          = "AUTO MODE",
        [I18N_STR_MANUAL_MODE]        = "MANUAL MODE",
        [I18N_STR_REL]                = "REL",
        [I18N_STR_OPEN]               = "OPEN",
        [I18N_STR_LIVE]               = "LIVE",
        [I18N_STR_AUTO]               = "AUTO",
        [I18N_STR_DMM_MIN]            = "MIN",
        [I18N_STR_DMM_MAX]            = "MAX",
        [I18N_STR_DMM_MAX_MIN]        = "MAX-MIN",
        [I18N_STR_OVER]               = "OVER",
        [I18N_STR_NEG_OVER]           = "-OVER",
    },
    [LANG_UA] = {
        [I18N_STR_MULTIMETER]         = "МУЛЬТИМЕТР",
        [I18N_STR_OSCILLOSCOPE]       = "ОСЦИЛОГРАФ",
        [I18N_STR_SIGNAL_GENERATOR]   = "ГЕНЕРАТОР",
        [I18N_STR_SETTINGS]           = "НАЛАШТУВАННЯ",
        [I18N_STR_MENU]               = "МЕНЮ",

        [I18N_STR_SETTING_BEEP]       = "ЗВУК",
        [I18N_STR_SETTING_DISPLAY]    = "ЕКРАН",
        [I18N_STR_SETTING_START]      = "СТАРТ",
        [I18N_STR_SETTING_SLEEP]      = "СОН",
        [I18N_STR_SETTING_LANGUAGE]   = "МОВА",
        [I18N_STR_SETTING_BOOTLOADER] = "ЗАВАНТАЖУВАЧ",
        [I18N_STR_SETTING_INFO]       = "ІНФО",
        [I18N_STR_EXPORT]             = "ЕКСПОРТ",

        [I18N_STR_RUN]                = "ПУСК",
        [I18N_STR_HOLD]               = "СТОП",
        [I18N_STR_ROLL]               = "ПРОКРУТ",
        [I18N_STR_START]              = "СТАРТ",
        [I18N_STR_STOP]               = "СТОП",
        [I18N_STR_VALUE]              = "ЗНАЧЕННЯ",
        [I18N_STR_PARAM]              = "ПАРАМЕТР",
        [I18N_STR_OFF]                = "ВИМК",
        [I18N_STR_ON]                 = "УВІМК",
        [I18N_STR_WAIT]               = "ЧЕКАЙТЕ",

        [I18N_STR_FW_UPDATE]          = "ОНОВЛЕННЯ ПРОШИВКИ",
        [I18N_STR_FW_RECEIVING]       = "ОТРИМАННЯ",
        [I18N_STR_FW_READY]           = "ГОТОВО ДО ПРОШИВКИ",
        [I18N_STR_FW_FLASHING]        = "ПРОШИВАННЯ",
        [I18N_STR_FW_NOT_FW]          = "НЕ ФАЙЛ ПРОШИВКИ",
        [I18N_STR_FW_TOO_LARGE]       = "ФАЙЛ ЗАВЕЛИКИЙ",
        [I18N_STR_FW_ERROR]           = "ПОМИЛКА ОНОВЛЕННЯ",

        [I18N_STR_SCREENSHOT]         = "ЗНІМОК ЕКРАНА",
        [I18N_STR_SAVING_TO_USB]      = "ЗАПИС НА USB",
        [I18N_STR_SAVED_TO_USB]       = "ЗБЕРЕЖЕНО НА USB",
        [I18N_STR_SAVE_ERROR]         = "ПОМИЛКА ЗБЕРЕЖЕННЯ",

        [I18N_STR_BOOT_EXPORT]            = "ЕКСПОРТ ЗАВАНТАЖУВАЧА",
        [I18N_STR_BOOT_EJECT_FIRST]       = "СПОЧАТКУ ВИЙМІТЬ USB",
        [I18N_STR_BOOT_REPLACES_BACKUP]   = "ПЕРЕЗАПИШЕ КОПІЮ",
        [I18N_STR_BOOT_PROMPT]            = "PLAY: ЕКСПОРТ   MENU: СКАСУВАТИ",
        [I18N_STR_BOOT_READING_VERIFYING] = "ЗЧИТУВАННЯ ТА ПЕРЕВІРКА",
        [I18N_STR_BOOT_ERR_VERIFIED]      = "ЗБЕРЕЖЕНО ТА ПЕРЕВІРЕНО",
        [I18N_STR_BOOT_ERR_BUSY]          = "ДИСК ЗАЙНЯТИЙ - ПОВТОР",
        [I18N_STR_BOOT_ERR_PROTECTED]     = "ЗАХИСТ ВІД ЗЧИТУВАННЯ",
        [I18N_STR_BOOT_ERR_VECTORS]       = "НЕВІРНІ ВЕКТОРИ",
        [I18N_STR_BOOT_ERR_STORAGE]       = "ПОМИЛКА ЗАПИСУ НА ДИСК",
        [I18N_STR_BOOT_ERR_SPACE]         = "НЕДОСТАТНЬО МІСЦЯ",
        [I18N_STR_BOOT_ERR_VERIFY]        = "ПОМИЛКА ПЕРЕВІРКИ",

        [I18N_STR_MATH_BODE_MENU]     = "МЕНЮ МАТЕМ / БОДЕ",
        [I18N_STR_PLOT_MODE]          = "РЕЖИМ:",
        [I18N_STR_FFT_WINDOW]         = "ВІКНО FFT:",
        [I18N_STR_FFT_DISPLAY]        = "ДИСПЛЕЙ FFT:",
        [I18N_STR_MATH_MODE]          = "МАТЕМАТИКА:",
        [I18N_STR_MATH_OP]            = "ОПЕРАЦІЯ:",
        [I18N_STR_HIDE_TRACES]        = "ПРИХОВАТИ:",
        [I18N_STR_START_FREQ]         = "ПОЧАТКОВА F:",
        [I18N_STR_STOP_FREQ]          = "КІНЦЕВА F:",
        [I18N_STR_STEPS]              = "КРОКИ:",
        [I18N_STR_SWEEP]              = "РОЗГОРТКА:",
        [I18N_STR_BODE_DONE]          = "БОДЕ ЗАВЕРШЕНО",
        [I18N_STR_RUNNING]            = "ВИКОНУЄТЬСЯ",
        [I18N_STR_RESWEEP]            = "ПОВТОРИТИ",

        [I18N_STR_MOVE]               = "ЗСУВ",
        [I18N_STR_Y_POS]              = "ПОЗ Y",
        [I18N_STR_X_POS]              = "ПОЗ X",
        [I18N_STR_RESET]              = "СКИД",
        [I18N_STR_ZERO]               = "НУЛЬ",
        [I18N_STR_BASELINE_CAL]       = "КАЛІБРУВАННЯ",
        [I18N_STR_REMOVE_PROBE_WAIT]  = "ЗНІМІТЬ ЩУП  ЧЕКАЙТЕ",

        [I18N_STR_SWEEP_MODULATION]   = "РОЗГОРТКА ТА МОДУЛЯЦІЯ",
        [I18N_STR_SWEEP_MODE]         = "РЕЖИМ РОЗГОРТКИ:",
        [I18N_STR_SWEEP_TIME]         = "ЧАС РОЗГОРТКИ:",
        [I18N_STR_FM_MODE]            = "РЕЖИМ ЧМ:",
        [I18N_STR_FM_WAVE]            = "ХВИЛЯ ЧМ:",
        [I18N_STR_FM_FREQ]            = "ЧАСТОТА ЧМ:",
        [I18N_STR_NO_FILE]            = "НЕМАЄ ФАЙЛУ",

        [I18N_STR_LIVE_WIRE]          = "ФАЗА",
        [I18N_STR_DANGER_V]           = "НЕБЕЗПЕЧНА V",
        [I18N_STR_HIGH_V]             = "ВИСОКА НАПРУГА",
        [I18N_STR_USE_HI_JACK]        = "ГНІЗДО 10A",
        [I18N_STR_USE_LO_JACK]        = "ГНІЗДО mA",
        [I18N_STR_AUTO_MODE]          = "АВТО РЕЖИМ",
        [I18N_STR_MANUAL_MODE]        = "РУЧНИЙ РЕЖИМ",
        [I18N_STR_REL]                = "ВІДН",
        [I18N_STR_OPEN]               = "ОБРИВ",
        [I18N_STR_LIVE]               = "ФАЗА",
        [I18N_STR_AUTO]               = "АВТО",
        [I18N_STR_DMM_MIN]            = "МІН",
        [I18N_STR_DMM_MAX]            = "МАКС",
        [I18N_STR_DMM_MAX_MIN]        = "МАКС-МІН",
        [I18N_STR_OVER]               = "ПЕРЕВ",
        [I18N_STR_NEG_OVER]           = "-ПЕРЕВ",
    },
};

const char *i18n_str(i18n_str_id_t id) {
    if (id < I18N_STR_TOTAL_COUNT) {
        return i18n_tbl_strings[current_language][id];
    }
    return "";
}

const char *i18n_dmm_unit(const char *raw) {
    if (!raw || !raw[0]) {
        return "";
    }
    if (raw[0] == 'V' && !raw[1]) {
        return "V";
    }
    if ((raw[0] == 'M' || raw[0] == 'm') && (raw[1] == 'V' || raw[1] == 'v') && !raw[2]) {
        return "mV";
    }
    if (raw[0] == 'A' && !raw[1]) {
        return "A";
    }
    if ((raw[0] == 'M' || raw[0] == 'm') && (raw[1] == 'A' || raw[1] == 'a') && !raw[2]) {
        return "mA";
    }
    if ((raw[0] == 'U' || raw[0] == 'u') && (raw[1] == 'A' || raw[1] == 'a') && !raw[2]) {
        return "µA";
    }
    if (raw[0] == 'O' && raw[1] == 'H' && raw[2] == 'M' && !raw[3]) {
        return "Ω";
    }
    if (raw[0] == 'K' && raw[1] == 'O' && raw[2] == 'H' && raw[3] == 'M' && !raw[4]) {
        return "kΩ";
    }
    if (raw[0] == 'M' && raw[1] == 'O' && raw[2] == 'H' && raw[3] == 'M' && !raw[4]) {
        return "MΩ";
    }
    if (raw[0] == 'F' && !raw[1]) {
        return "F";
    }
    if ((raw[0] == 'M' || raw[0] == 'm') && (raw[1] == 'F' || raw[1] == 'f') && !raw[2]) {
        return "mF";
    }
    if ((raw[0] == 'U' || raw[0] == 'u') && (raw[1] == 'F' || raw[1] == 'f') && !raw[2]) {
        return "µF";
    }
    if ((raw[0] == 'N' || raw[0] == 'n') && (raw[1] == 'F' || raw[1] == 'f') && !raw[2]) {
        return "nF";
    }
    if (raw[0] == 'D' && raw[1] == 'E' && raw[2] == 'G' && !raw[3]) {
        return "°C";
    }
    return raw;
}

static const char *const i18n_tbl_mode_short_labels[LANG_COUNT][3] = {
    [LANG_EN] = {"DMM", "OSC", "SIG"},
    [LANG_UA] = {"ММТ", "ОСЦ", "ГЕН"},
};

const char *i18n_mode_short(uint8_t mode) {
    if (mode < 3) return i18n_tbl_mode_short_labels[current_language][mode];
    return "";
}

static const char *const i18n_tbl_menu_labels[LANG_COUNT][4] = {
    [LANG_EN] = {"MULTIMETER", "OSCILLOSCOPE", "SIGNAL GENERATOR", "SETTINGS"},
    [LANG_UA] = {"МУЛЬТИМЕТР", "ОСЦИЛОГРАФ", "ГЕНЕРАТОР", "НАЛАШТУВАННЯ"},
};

const char *i18n_menu_label(uint8_t index) {
    if (index < 4) return i18n_tbl_menu_labels[current_language][index];
    return "";
}

static const char *const i18n_tbl_settings_row_labels[LANG_COUNT][7] = {
    [LANG_EN] = {"BEEP", "DISPLAY", "START", "SLEEP", "LANGUAGE", "BOOTLOADER", "INFO"},
    [LANG_UA] = {"ЗВУК", "ЕКРАН", "СТАРТ", "СОН", "МОВА", "ЗАВАНТАЖУВАЧ", "ІНФО"},
};

const char *i18n_settings_row_label(uint8_t index) {
    if (index < 7) return i18n_tbl_settings_row_labels[current_language][index];
    return "";
}

static const char *const i18n_tbl_startup_labels[LANG_COUNT][4] = {
    [LANG_EN] = {"MENU", "DMM", "SCOPE", "GEN"},
    [LANG_UA] = {"МЕНЮ", "МУЛЬТ", "ОСЦИЛ", "ГЕНЕР"},
};

const char *i18n_startup_label(uint8_t index) {
    if (index < 4) return i18n_tbl_startup_labels[current_language][index];
    return "";
}

static const char *const i18n_tbl_beep_level_labels[LANG_COUNT][5] = {
    [LANG_EN] = {"OFF", "LOW", "MEDIUM", "HIGH", "MAX"},
    [LANG_UA] = {"ВИМК", "ТИХО", "СЕРЕДН", "ГУЧНО", "МАКС"},
};

const char *i18n_beep_level_label(uint8_t index) {
    if (index < 5) return i18n_tbl_beep_level_labels[current_language][index];
    return "";
}

static const char *const i18n_tbl_brightness_level_labels[LANG_COUNT][5] = {
    [LANG_EN] = {"DIM", "LOW", "MEDIUM", "HIGH", "BRIGHT"},
    [LANG_UA] = {"ТЬМЯНО", "НИЗЬКА", "СЕРЕДНЯ", "ВИСОКА", "ЯСКРАВО"},
};

const char *i18n_brightness_level_label(uint8_t index) {
    if (index < 5) return i18n_tbl_brightness_level_labels[current_language][index];
    return "";
}

static const char *const i18n_tbl_sleep_labels[LANG_COUNT][4] = {
    [LANG_EN] = {"OFF", "10 MIN", "20 MIN", "30 MIN"},
    [LANG_UA] = {"ВИМК", "10 ХВ", "20 ХВ", "30 ХВ"},
};

const char *i18n_sleep_label(uint8_t index) {
    if (index < 4) return i18n_tbl_sleep_labels[current_language][index];
    return "";
}

static const char *const i18n_tbl_dmm_mode_names[LANG_COUNT][13] = {
    [LANG_EN] = {
        "AUTO DETECT", "DC VOLTAGE", "AC VOLTAGE", "RESISTANCE",
        "DIODE", "CAPACITANCE", "LIVE WIRE", "CONTINUITY",
        "TEMPERATURE", "AC HIGH CURR", "DC HIGH CURR",
        "AC LOW CURR", "DC LOW CURR",
    },
    [LANG_UA] = {
        "АВТО ВИЗНАЧЕННЯ", "ПОСТ. НАПРУГА", "ЗМІННА НАПРУГА", "ОПІР",
        "ДІОД", "ЄМНІСТЬ", "ФАЗНИЙ ПРОВІД", "ПРОДЗВОНКА",
        "ТЕМПЕРАТУРА", "ЗМІННИЙ СТРУМ 10A", "ПОСТІЙНИЙ СТРУМ 10A",
        "ЗМІННИЙ СТРУМ mA", "ПОСТІЙНИЙ СТРУМ mA",
    },
};

const char *i18n_dmm_mode_name(uint8_t index) {
    if (index < 13) return i18n_tbl_dmm_mode_names[current_language][index];
    return "";
}

static const char *const i18n_tbl_dmm_mode_short[LANG_COUNT][13] = {
    [LANG_EN] = {
        "AUTO", "DCV", "ACV", "RES", "DIODE", "CAP", "LIVE",
        "CONT", "TEMP", "AC HI", "DC HI", "AC LO", "DC LO",
    },
    [LANG_UA] = {
        "АВТО", "DC V", "AC V", "ОПІР", "ДІОД", "ЄМН", "ФАЗА",
        "ДЗВ", "ТЕМП", "AC A", "DC A", "AC mA", "DC mA",
    },
};

const char *i18n_dmm_mode_short(uint8_t index) {
    if (index < 13) return i18n_tbl_dmm_mode_short[current_language][index];
    return "";
}

static const char *const i18n_tbl_dmm_group_labels[LANG_COUNT][4] = {
    [LANG_EN] = {"V/OHM", "TEST", "SENSE", "CURRENT"},
    [LANG_UA] = {"V/Ω", "ТЕСТ", "ДАТЧИК", "СТРУМ"},
};

const char *i18n_dmm_group_label(uint8_t group) {
    if (group < 4) return i18n_tbl_dmm_group_labels[current_language][group];
    return "";
}

static const char *const i18n_tbl_scope_display_labels[LANG_COUNT][3] = {
    [LANG_EN] = {"Y-T", "ROLL", "X-Y"},
    [LANG_UA] = {"Y-T", "ПРОКРУТ", "X-Y"},
};

const char *i18n_scope_display_label(uint8_t index) {
    if (index < 3) return i18n_tbl_scope_display_labels[current_language][index];
    return "";
}

static const char *const i18n_tbl_scope_cursor_labels[LANG_COUNT][4] = {
    [LANG_EN] = {"CUR OFF", "T CUR", "Y CUR", "B CUR"},
    [LANG_UA] = {"КУРС ВИМК", "КУРС T", "КУРС Y", "КУРС B"},
};

const char *i18n_scope_cursor_label(uint8_t index) {
    if (index < 4) return i18n_tbl_scope_cursor_labels[current_language][index];
    return "";
}

static const char *const i18n_tbl_scope_cursor_short[LANG_COUNT][4] = {
    [LANG_EN] = {"OFF", "T", "Y", "B"},
    [LANG_UA] = {"ВИМК", "T", "Y", "B"},
};

const char *i18n_scope_cursor_short(uint8_t index) {
    if (index < 4) return i18n_tbl_scope_cursor_short[current_language][index];
    return "";
}

static const char *const i18n_tbl_scope_trigger_short[LANG_COUNT][3] = {
    [LANG_EN] = {"AUTO", "NORM", "SNGL"},
    [LANG_UA] = {"АВТО", "НОРМ", "ОДИН"},
};

const char *i18n_scope_trigger_short(uint8_t index) {
    if (index < 3) return i18n_tbl_scope_trigger_short[current_language][index];
    return "";
}

static const char *const i18n_tbl_scope_param_labels[LANG_COUNT][15] = {
    [LANG_EN] = {
        "VIEW", "ON", "PROBE", "V/DIV", "COUP", "ZERO", "POS", "TIME", "TRIG", "CUR",
        "SRC", "LEVEL", "MODE", "EDGE", "MEAS",
    },
    [LANG_UA] = {
        "ВИД", "УВІМК", "ЩУП", "В/ПОД", "ВХІД", "НУЛЬ", "ПОЗ", "ЧАС", "ТРИГ", "КУРС",
        "ДЖЕР", "РІВЕНЬ", "РЕЖИМ", "ФРОНТ", "ВИМІР",
    },
};

const char *i18n_scope_param_label(uint8_t index) {
    if (index < 15) return i18n_tbl_scope_param_labels[current_language][index];
    return "";
}

static const char *const i18n_tbl_scope_trigger_edge_labels[LANG_COUNT][2] = {
    [LANG_EN] = {"RISE", "FALL"},
    [LANG_UA] = {"НАРОСТ", "СПАД"},
};

const char *i18n_scope_trigger_edge_label(uint8_t index) {
    if (index < 2) return i18n_tbl_scope_trigger_edge_labels[current_language][index];
    return "";
}

static const char *const i18n_tbl_gen_wave_labels[LANG_COUNT][16] = {
    [LANG_EN] = {
        "SINE", "RECT", "SAW", "HALF", "FULL",
        "P STEP", "R STEP", "EXP UP", "EXP DN", "DC",
        "MULTI AUD", "SINK PLS", "LORENTZ", "TRIANGLE", "NOISE", "ARBITRARY",
    },
    [LANG_UA] = {
        "СИНУС", "МЕАНДР", "ПИЛА", "НАПІВХВИЛЯ", "ПОВНА ХВИЛЯ",
        "ПОЗ СХОДИ", "НЕГ СХОДИ", "ЕКСП ЗРОСТ", "ЕКСП СПАД", "ПОСТІЙНИЙ",
        "МУЛЬТИ АУД", "SINK ІМП", "ЛОРЕНЦ", "ТРИКУТНИК", "ШУМ", "ДОВІЛЬНИЙ",
    },
};

const char *i18n_gen_wave_label(uint8_t index) {
    if (index < 16) return i18n_tbl_gen_wave_labels[current_language][index];
    return "";
}

static const char *const i18n_tbl_gen_wave_short_labels[LANG_COUNT][16] = {
    [LANG_EN] = {
        "SINE", "RECT", "SAW", "HALF", "FULL",
        "PSTEP", "RSTEP", "EXP+", "EXP-", "DC",
        "MULTI", "SINK", "LOR", "TRI", "NOISE", "ARBT",
    },
    [LANG_UA] = {
        "СИН", "МЕАНДР", "ПИЛА", "НАПІВ", "ПОВНА",
        "ПСХІД", "НСХІД", "ЕКСП+", "ЕКСП-", "DC",
        "МУЛЬТ", "SINK", "ЛОР", "ТРИК", "ШУМ", "ДОВІЛ",
    },
};

const char *i18n_gen_wave_short_label(uint8_t index) {
    if (index < 16) return i18n_tbl_gen_wave_short_labels[current_language][index];
    return "";
}

static const char *const i18n_tbl_gen_param_labels[LANG_COUNT][4] = {
    [LANG_EN] = {"WAVE", "FREQ", "DUTY", "AMP"},
    [LANG_UA] = {"ФОРМА", "ЧАСТ", "ЗАПОВН", "АМПЛ"},
};

const char *i18n_gen_param_label(uint8_t index) {
    if (index < 4) return i18n_tbl_gen_param_labels[current_language][index];
    return "";
}

static const char *const i18n_tbl_gen_sweep_mode_names[LANG_COUNT][3] = {
    [LANG_EN] = {"OFF", "LINEAR", "LOG"},
    [LANG_UA] = {"ВИМК", "ЛІНІЙНА", "ЛОГАРИФМ"},
};

const char *i18n_gen_sweep_mode_name(uint8_t index) {
    if (index < 3) return i18n_tbl_gen_sweep_mode_names[current_language][index];
    return "";
}

static const char *const i18n_tbl_gen_fm_mode_names[LANG_COUNT][2] = {
    [LANG_EN] = {"OFF", "ON"},
    [LANG_UA] = {"ВИМК", "УВІМК"},
};

const char *i18n_gen_fm_mode_name(uint8_t index) {
    if (index < 2) return i18n_tbl_gen_fm_mode_names[current_language][index];
    return "";
}

static const char *const i18n_tbl_gen_fm_wave_names[LANG_COUNT][3] = {
    [LANG_EN] = {"SINE", "TRIANGLE", "SQUARE"},
    [LANG_UA] = {"СИНУС", "ТРИКУТНИК", "МЕАНДР"},
};

const char *i18n_gen_fm_wave_name(uint8_t index) {
    if (index < 3) return i18n_tbl_gen_fm_wave_names[current_language][index];
    return "";
}

static const char *const i18n_tbl_fft_status_text[LANG_COUNT][5] = {
    [LANG_EN] = {"OFF", "CH1 ENABLED", "CH2 ENABLED", "XY MODE", "BODE PLOT"},
    [LANG_UA] = {"ВИМК", "КАН1 УВІМК", "КАН2 УВІМК", "XY РЕЖИМ", "ГРАФІК БОДЕ"},
};

const char *i18n_fft_status_text(uint8_t index) {
    if (index < 5) return i18n_tbl_fft_status_text[current_language][index];
    return "";
}

static const char *const i18n_tbl_fft_window_text[LANG_COUNT][4] = {
    [LANG_EN] = {"HANN", "HAMMING", "BLACKMAN", "RECTANGLE"},
    [LANG_UA] = {"ХАНН", "ХЕММІНГ", "БЛЕКМЕН", "ПРЯМОКУТ"},
};

const char *i18n_fft_window_text(uint8_t index) {
    if (index < 4) return i18n_tbl_fft_window_text[current_language][index];
    return "";
}

static const char *const i18n_tbl_fft_display_text[LANG_COUNT][3] = {
    [LANG_EN] = {"NORMAL", "AVERAGING", "MAX HOLD"},
    [LANG_UA] = {"ЗВИЧАЙНИЙ", "УСЕРЕДНЕННЯ", "МАКС УТРИМ"},
};

const char *i18n_fft_display_text(uint8_t index) {
    if (index < 3) return i18n_tbl_fft_display_text[current_language][index];
    return "";
}

static const char *const i18n_tbl_math_status_text[LANG_COUNT][2] = {
    [LANG_EN] = {"OFF", "ON"},
    [LANG_UA] = {"ВИМК", "УВІМК"},
};

const char *i18n_math_status_text(uint8_t index) {
    if (index < 2) return i18n_tbl_math_status_text[current_language][index];
    return "";
}

static const char *const i18n_tbl_math_op_text[LANG_COUNT][3] = {
    [LANG_EN] = {"CH1 + CH2", "CH1 - CH2", "CH2 - CH1"},
    [LANG_UA] = {"КАН1 + КАН2", "КАН1 - КАН2", "КАН2 - КАН1"},
};

const char *i18n_math_op_text(uint8_t index) {
    if (index < 3) return i18n_tbl_math_op_text[current_language][index];
    return "";
}

static const char *const i18n_tbl_hide_traces_text[LANG_COUNT][4] = {
    [LANG_EN] = {"NONE", "CH1", "CH2", "ALL"},
    [LANG_UA] = {"НЕМАЄ", "КАН1", "КАН2", "ВСІ"},
};

const char *i18n_hide_traces_text(uint8_t index) {
    if (index < 4) return i18n_tbl_hide_traces_text[current_language][index];
    return "";
}

static const char *const i18n_tbl_boot_export_errors[LANG_COUNT][7] = {
    [LANG_EN] = {
        "SAVED AND VERIFIED", "STORAGE BUSY - RETRY", "BOOTLOADER READ PROTECTED",
        "INVALID BOOTLOADER VECTORS", "STORAGE WRITE ERROR",
        "NOT ENOUGH FREE SPACE", "BACKUP VERIFY ERROR",
    },
    [LANG_UA] = {
        "ЗБЕРЕЖЕНО ТА ПЕРЕВІРЕНО", "ДИСК ЗАЙНЯТИЙ - ПОВТОР", "ЗАХИСТ ВІД ЗЧИТУВАННЯ",
        "НЕВІРНІ ВЕКТОРИ", "ПОМИЛКА ЗАПИСУ НА ДИСК",
        "НЕДОСТАТНЬО МІСЦЯ", "ПОМИЛКА ПЕРЕВІРКИ",
    },
};

const char *i18n_boot_export_error(uint8_t index) {
    if (index < 7) return i18n_tbl_boot_export_errors[current_language][index];
    return "";
}

static const char *const i18n_tbl_fw_update_statuses[LANG_COUNT][6] = {
    [LANG_EN] = {
        "RECEIVING", "READY TO FLASH", "FLASHING",
        "NOT A FIRMWARE FILE", "FILE TOO LARGE", "UPDATE ERROR",
    },
    [LANG_UA] = {
        "ОТРИМАННЯ", "ГОТОВО ДО ПРОШИВКИ", "ПРОШИВАННЯ",
        "НЕ ФАЙЛ ПРОШИВКИ", "ФАЙЛ ЗАВЕЛИКИЙ", "ПОМИЛКА ОНОВЛЕННЯ",
    },
};

const char *i18n_fw_update_status(uint8_t index) {
    if (index < 6) return i18n_tbl_fw_update_statuses[current_language][index];
    return "";
}
