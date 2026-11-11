#include <assert.h>
#include <stdio.h>
#include <string.h>

// Test the real decoder without Cortex-M interrupt instructions or MMIO calls.
#define __asm__
#define volatile(...)
#define dmm_current_mode dmm_wire_mode
#include "../src/dmm.c"
#undef dmm_current_mode
#undef volatile
#undef __asm__
#include "../src/ui.c"

static void make_frame(const char digits[4], unsigned dots, uint8_t prefix,
                       uint8_t flags_b, uint8_t flags_c, uint8_t frame[9]) {
    uint8_t segments[4];
    for (unsigned i = 0; i < 4; ++i) {
        const char *p = strchr(dmm_segment_chars, digits[i]);
        assert(p);
        segments[i] = dmm_segment_codes[p - dmm_segment_chars];
        if (dots & (1u << i)) segments[i] |= 0x10;
    }
    frame[0] = 0x5A;
    frame[1] = 0xA5;
    frame[2] = segments[0] & 0xF0;
    frame[3] = (segments[1] & 0xF0) | (segments[0] & 0x0F);
    frame[4] = (segments[2] & 0xF0) | (segments[1] & 0x0F);
    frame[5] = (segments[3] & 0xF0) | (segments[2] & 0x0F);
    frame[6] = (segments[3] & 0x0F) | prefix;
    frame[7] = flags_b;
    frame[8] = flags_c;
}

static void setup(uint8_t mode) {
    dmm_wire_mode = ui.dmm_mode = mode;
    ui.mode = UI_MODE_DMM;
    dmm_valid = dmm_synthetic_valid = dmm_numeric_valid = dmm_baud_locked = 0;
    dmm_value_fixed = 0;
    dmm_hold_active = dmm_rel_active = 0;
    rx_pos = rx_ready = 0;
    strcpy(dmm_value, "----");
    strcpy(dmm_unit, "?");
}

static void reading(uint8_t mode, const char digits[4], unsigned dots,
                    uint8_t prefix, uint8_t flags_b, uint8_t flags_c,
                    const char *value, const char *unit) {
    uint8_t frame[9];
    setup(mode);
    make_frame(digits, dots, prefix, flags_b, flags_c, frame);
    assert(parse_frame(frame));
    assert(dmm_reading_is_real() && dmm_baud_locked);
    assert(strcmp(dmm_value_text(), value) == 0);
    assert(strcmp(dmm_unit_text(), unit) == 0);
}

static void test_stock_decoding(void) {
    // Stock digit renderers: 2.0.2 at 0x0800D1B4, 2.1.0 at 0x0800C52C.
    reading(0, "1234", 8, 0, 0, 2, "123.4", "V");
    reading(1, "1234", 8, 0, 0, 2, "123.4", "V");
    reading(2, "1234", 8, 0, 0, 2, "123.4", "V");
    reading(5, "0123", 8, 0x10, 0x10, 0, "12.3", "UF");
    reading(3, "1234", 0, 0, 0x20, 0, "1234.0", "OHM");
    reading(1, "1234", 0, 0, 0, 2, "1234", "V");
    reading(12, "1234", 2, 0x10, 0, 1, "1.234", "UA");
    assert(strcmp(dmm_live_display_value(), "1.2340") == 0);
    assert(strcmp(dmm_live_display_unit(), "UA") == 0);
    reading(10, "1234", 2, 0, 1, 1, "1.234", "MA");
    assert(strcmp(dmm_live_display_unit(), "MA") == 0);
    reading(12, "0499", 0, 0x10, 0, 1, "499", "UA");
    reading(10, "0123", 5, 0, 0, 1, "-1.23", "A");
    reading(12, "0123", 5, 0, 1, 1, "-1.23", "MA");
    reading(8, "0037", 1, 0, 0, 0x20, "-37", "DEG");
    reading(1, "3478", 9, 0, 0, 2, "-347.8", "V");
    reading(0, "0000", 8, 0, 0, 2, "0.0", "V");
    assert(strcmp(dmm_live_display_unit(), "V") == 0);
    reading(8, "0000", 0, 0, 0, 0x20, "0", "DEG");
    reading(8, "0042", 0, 0, 0, 0x10, "42", "DEG");

    uint8_t frame[9];
    for (uint8_t mode = 0; mode <= 2; ++mode) {
        setup(mode);
        make_frame("1234", 4, 0, 0, 2, frame);
        frame[2] |= 1; // Stock's AUTO AC/DC flag is not a negative sign.
        assert(parse_frame(frame));
        assert(strcmp(dmm_value_text(), "12.34") == 0);
    }

    static const uint8_t commands[] = {
        0x14, 0x0C, 0x0D, 0x0B, 0x0E, 0x0A, 0x13,
        0x12, 0x12, 0x10, 0x11, 0x15, 0x16,
    };
    assert(sizeof(commands) == sizeof(dmm_mode_command));
    assert(memcmp(commands, dmm_mode_command, sizeof(commands)) == 0);
}

static void test_all_numeric_digits(void) {
    static const struct {
        uint8_t mode, prefix, flags_b, flags_c;
        const char *unit;
    } kinds[] = {
        {0, 0, 0, 2, "V"}, {1, 0, 0, 2, "V"}, {2, 0, 1, 2, "MV"},
        {3, 0x40, 0x20, 0, "KOHM"}, {4, 0, 0, 2, "V"},
        {5, 0x20, 0x10, 0, "NF"}, {8, 0, 0, 0x20, "DEG"},
        {9, 0, 0, 1, "A"}, {10, 0, 1, 1, "MA"},
        {11, 0x10, 0, 1, "UA"}, {12, 0, 0, 1, "A"},
    };
    for (unsigned kind = 0; kind < sizeof(kinds) / sizeof(kinds[0]); ++kind) {
        setup(kinds[kind].mode);
        for (unsigned value = 0; value <= 9999; ++value) {
            char digits[5];
            snprintf(digits, sizeof(digits), "%04u", value);
            for (unsigned point = 0; point <= 3; ++point) {
                int32_t factor = point == 1 ? 10 : point == 2 ? 100 : point == 3 ? 1000 : DMM_VALUE_SCALE;
                for (unsigned negative = 0; negative <= 1; ++negative) {
                    uint8_t frame[9];
                    make_frame(digits, (point ? 1u << point : 0) | negative,
                               kinds[kind].prefix, kinds[kind].flags_b, kinds[kind].flags_c, frame);
                    parse_frame(frame);
                    assert(dmm_has_reading() && dmm_value_is_numeric());
                    assert(dmm_value_fixed_units() == (negative ? -(int32_t)value : (int32_t)value) * factor);
                    assert(strcmp(dmm_unit_text(), kinds[kind].unit) == 0);
                }
            }
        }
    }
}

static void test_status_and_rejected_frames(void) {
    reading(0, "AUTO", 0, 0, 0, 2, "AUTO", "V");
    assert(strcmp(dmm_live_display_unit(), "") == 0);
    for (unsigned point = 0; point <= 3; ++point) {
        reading(3, "00L0", point ? 1u << point : 0, 0, 0x24, 0, "OPEN", "MOHM");
        assert(!dmm_value_is_numeric());
        assert(strcmp(dmm_live_display_unit(), "") == 0);
        reading(4, "00L0", point ? 1u << point : 0, 0, 0, 2, "OPEN", "V");
        assert(!dmm_diode_continuity_active());
    }
    reading(6, "L1UE", 0, 0, 0, 0, "L1UE", "");
    assert(!dmm_live_wire_active());
    reading(6, "00L0", 0, 0, 0, 0, "00L0", "");
    assert(dmm_live_wire_active());
    reading(6, "L1UE", 0, 0, 0, 2, "L1UE", "V");
    assert(!dmm_live_wire_active());
    reading(6, "00L0", 0, 0, 0, 2, "00L0", "V");
    assert(dmm_live_wire_active());
    reading(6, "0000", 0, 0, 0, 0, "L1UE", "");
    assert(!dmm_live_wire_active());
    reading(8, "0037", 0, 0, 0, 0x20, "37", "DEG");
    uint8_t frame[9];
    make_frame("00L0", 0, 0, 0, 0x20, frame);
    assert(!parse_frame(frame));
    assert(strcmp(dmm_value_text(), "37") == 0);

    setup(3);
    make_frame("L1UE", 0, 0, 0, 0, frame);
    assert(!parse_frame(frame) && dmm_baud_locked && !dmm_has_reading());
    dmm_baud_locked = 0;
    make_frame("1234", 2, 0, 0, 2, frame);
    assert(!parse_frame(frame) && dmm_baud_locked); // Do not relabel stale volts as ohms.
    setup(8);
    make_frame("00L0", 0, 0, 0, 0x20, frame);
    assert(!parse_frame(frame) && dmm_baud_locked && !dmm_has_reading());
    setup(5);
    make_frame("1234", 2, 0, 0, 2, frame);
    assert(!parse_frame(frame) && dmm_baud_locked && !dmm_has_reading());
    dmm_started = dmm_powered = 1;
    dmm_settle_ms = dmm_retry_ms = 0;
    uint8_t baud = dmm_baud_index;
    dmm_tick(3000);
    assert(dmm_baud_index == baud && dmm_retry_ms == 0);
    dmm_started = dmm_powered = 0;
    setup(1);
    make_frame("1234", 2, 0, 0, 2, frame);
    frame[2] = 0x20;
    frame[3] &= 0xF0;
    assert(!parse_frame(frame) && !dmm_baud_locked);
    make_frame("1234", 6, 0, 0, 2, frame);
    assert(!parse_frame(frame) && !dmm_baud_locked);
    make_frame("1234", 2, 0, 0, 0, frame);
    assert(!parse_frame(frame) && !dmm_baud_locked);
}

static void test_relative_precision(void) {
    setup(12);
    dmm_valid = dmm_numeric_valid = 1;
    strcpy(dmm_unit, "MA");
    strcpy(dmm_value, "0.0001");
    assert(parse_fixed_units(dmm_value, &dmm_value_fixed));
    dmm_toggle_relative();
    assert(dmm_rel_active && strcmp(dmm_display_value(), "0.0000") == 0);
    strcpy(dmm_value, "0.0002");
    assert(parse_fixed_units(dmm_value, &dmm_value_fixed));
    assert(strcmp(dmm_display_value(), "0.0001") == 0);
    assert(strcmp(dmm_rel_ref_value, "0.0001") == 0);
    strcpy(dmm_value, "-0.0001");
    assert(parse_fixed_units(dmm_value, &dmm_value_fixed));
    assert(strcmp(dmm_display_value(), "-0.0002") == 0);
    strcpy(dmm_unit, "UA");
    assert(strcmp(dmm_display_value(), "RANGE") == 0);
    format_signed_fixed(-99999999, dmm_rel_value);
    assert(strcmp(dmm_rel_value, "-9999.9999") == 0);
    assert(parse_fixed_units("0.00015", &dmm_value_fixed) && dmm_value_fixed == 2);
    assert(parse_fixed_units("0.99995", &dmm_value_fixed) && dmm_value_fixed == DMM_VALUE_SCALE);
    assert(parse_fixed_units("-1.2345", &dmm_value_fixed));
    assert(dmm_value_milli_units() == -1235);
}

static void test_synthetic_readings(void) {
    for (uint8_t mode = 0; mode < sizeof(dmm_mode_command); ++mode) {
        setup(mode);
        dmm_apply_missing_uart_fallback();
        assert(dmm_has_reading() && !dmm_reading_is_real());
        assert(!dmm_diode_continuity_active() && !dmm_live_wire_active());
        assert(!dmm_baud_locked);
    }
    setup(12);
    dmm_apply_missing_uart_fallback();
    assert(strcmp(dmm_live_display_value(), "0.0000") == 0);
    setup(0);
    dmm_apply_missing_uart_fallback();
    assert(strcmp(dmm_live_display_value(), "AUTO") == 0);
    assert(strcmp(dmm_live_display_unit(), "") == 0);
}

static void test_diode_thresholds(void) {
    reading(4, "0010", 8, 0x40, 0x20, 0, "1.0", "KOHM");
    assert(!dmm_diode_continuity_active());
    reading(4, "0050", 2, 0x40, 0x20, 0, "0.050", "KOHM");
    assert(dmm_diode_continuity_active());
    reading(4, "0051", 2, 0x40, 0x20, 0, "0.051", "KOHM");
    assert(!dmm_diode_continuity_active());
    reading(4, "0001", 2, 0, 0x24, 0, "0.001", "MOHM");
    assert(!dmm_diode_continuity_active());
    reading(4, "5000", 4, 0, 1, 2, "50.00", "MV");
    assert(dmm_diode_continuity_active());
    reading(4, "0200", 2, 0, 0, 2, "0.200", "V");
    assert(!dmm_diode_continuity_active());
    reading(4, "0200", 3, 0, 0, 2, "-0.200", "V");
    assert(!dmm_diode_continuity_active());
    reading(4, "0050", 3, 0, 0, 2, "-0.050", "V");
    assert(dmm_diode_continuity_active());
    dmm_synthetic_valid = 1;
    assert(!dmm_diode_continuity_active());
}

static void receive(const uint8_t frame[9], unsigned count) {
    for (unsigned i = 0; i < count; ++i) dmm_rx_event(USART_STS_RXNE, frame[i]);
}

static void test_packet_boundaries(void) {
    uint8_t frame[9];
    setup(1);
    make_frame("1234", 8, 0, 0, 2, frame);
    for (unsigned cut = 1; cut < 9; ++cut) {
        rx_pos = rx_ready = 0;
        receive(frame, cut);
        assert(!rx_ready);
        dmm_rx_event(USART_STS_IDLE, 0);
        assert(rx_pos == 0);
        receive(frame, 9);
        assert(rx_ready && memcmp((const void *)rx_pending, frame, 9) == 0);
        dmm_rx_event(USART_STS_IDLE, 0);
        assert(rx_ready && rx_pos == 0);
    }
    static const uint32_t errors[] = {USART_STS_FE, USART_STS_NE, USART_STS_ORE};
    for (unsigned i = 0; i < sizeof(errors) / sizeof(errors[0]); ++i) {
        rx_pos = rx_ready = 0;
        receive(frame, 4);
        dmm_rx_event(errors[i] | USART_STS_RXNE, frame[4]);
        assert(rx_pos == 0 && !rx_ready);
        receive(frame, 9);
        assert(rx_ready && memcmp((const void *)rx_pending, frame, 9) == 0);
    }
    rx_pos = rx_ready = 0;
    receive(frame, 4);
    dmm_rx_event(USART_STS_IDLE | USART_STS_RXNE, frame[0]);
    assert(rx_pos == 1);
    receive(frame + 1, 8);
    assert(rx_ready && memcmp((const void *)rx_pending, frame, 9) == 0);
}

static void test_dmm_i18n_units_and_labels(void) {
    i18n_set_language(LANG_EN);
    assert(strcmp(i18n_dmm_unit("V"), "V") == 0);
    assert(strcmp(i18n_dmm_unit("MV"), "mV") == 0);
    assert(strcmp(i18n_dmm_unit("A"), "A") == 0);
    assert(strcmp(i18n_dmm_unit("MA"), "mA") == 0);
    assert(strcmp(i18n_dmm_unit("UA"), "µA") == 0);
    assert(strcmp(i18n_dmm_unit("OHM"), "Ω") == 0);
    assert(strcmp(i18n_dmm_unit("KOHM"), "kΩ") == 0);
    assert(strcmp(i18n_dmm_unit("MOHM"), "MΩ") == 0);
    assert(strcmp(i18n_dmm_unit("F"), "F") == 0);
    assert(strcmp(i18n_dmm_unit("MF"), "mF") == 0);
    assert(strcmp(i18n_dmm_unit("UF"), "µF") == 0);
    assert(strcmp(i18n_dmm_unit("NF"), "nF") == 0);
    assert(strcmp(i18n_dmm_unit("DEG"), "°C") == 0);
    assert(strcmp(i18n_str(I18N_STR_OPEN), "OPEN") == 0);
    assert(strcmp(i18n_str(I18N_STR_LIVE), "LIVE") == 0);

    // Test display values in EN
    setup(3); // Resistance mode
    strcpy(dmm_value, "OPEN");
    dmm_valid = 1;
    assert(strcmp(dmm_live_display_value(), "OPEN") == 0);

    setup(6); // Live wire mode
    dmm_valid = 1;
    assert(strcmp(dmm_live_display_value(), "LIVE") == 0);

    // Test Ukrainian translations
    i18n_set_language(LANG_UA);
    assert(strcmp(i18n_dmm_unit("V"), "В") == 0);
    assert(strcmp(i18n_dmm_unit("MV"), "мВ") == 0);
    assert(strcmp(i18n_dmm_unit("A"), "А") == 0);
    assert(strcmp(i18n_dmm_unit("MA"), "мА") == 0);
    assert(strcmp(i18n_dmm_unit("UA"), "µА") == 0);
    assert(strcmp(i18n_dmm_unit("OHM"), "Ω") == 0);
    assert(strcmp(i18n_dmm_unit("KOHM"), "кΩ") == 0);
    assert(strcmp(i18n_dmm_unit("MOHM"), "МΩ") == 0);
    assert(strcmp(i18n_dmm_unit("F"), "Ф") == 0);
    assert(strcmp(i18n_dmm_unit("MF"), "мФ") == 0);
    assert(strcmp(i18n_dmm_unit("UF"), "µФ") == 0);
    assert(strcmp(i18n_dmm_unit("NF"), "nФ") == 0);
    assert(strcmp(i18n_dmm_unit("DEG"), "°C") == 0);
    assert(strcmp(i18n_str(I18N_STR_OPEN), "ОБРИВ") == 0);
    assert(strcmp(i18n_str(I18N_STR_LIVE), "ФАЗА") == 0);
    assert(strcmp(i18n_str(I18N_STR_AUTO), "АВТО") == 0);

    // Test display values in UA
    setup(3);
    strcpy(dmm_value, "OPEN");
    dmm_valid = 1;
    assert(strcmp(dmm_live_display_value(), "ОБРИВ") == 0);

    setup(6);
    dmm_valid = 1;
    assert(strcmp(dmm_live_display_value(), "ФАЗА") == 0);

    setup(0);
    strcpy(dmm_value, "AUTO");
    dmm_valid = 1;
    assert(strcmp(dmm_live_display_value(), "АВТО") == 0);

    // Test dmm_display_unit reflects localized unit
    setup(10); // DC HI mode, mA
    strcpy(dmm_unit, "MA");
    dmm_valid = 1;
    assert(strcmp(dmm_display_unit(), "мА") == 0);

    i18n_set_language(LANG_EN);
    assert(strcmp(dmm_display_unit(), "mA") == 0);
    setup(0);
    strcpy(dmm_value, "AUTO");
    dmm_valid = 1;
    assert(strcmp(dmm_live_display_value(), "AUTO") == 0);
}

void delay_ms(uint32_t ms) { (void)ms; }
void gpio_config_mask(uint32_t base, uint16_t mask, uint8_t cfg) {
    (void)base; (void)mask; (void)cfg;
}
void scope_hw_slow_stop(void) {}
void scope_hw_arm(void) {}
uint8_t scope_hw_enabled(void) { return 1; }
void usb_cdc_send_bytes(const uint8_t *data, uint16_t len) { (void)data; (void)len; }
void usb_cdc_send_dmm(const char *mode, const char *val, const char *unit, uint8_t valid) {
    (void)mode; (void)val; (void)unit; (void)valid;
}
uint8_t usb_cdc_is_configured(void) { return 0; }
void siggen_shutdown(void) {}
void siggen_configure(uint8_t enabled, uint8_t wave, uint32_t freq_hz, uint8_t duty_percent, uint8_t amplitude_tenths_v) {
    (void)enabled; (void)wave; (void)freq_hz; (void)duty_percent; (void)amplitude_tenths_v;
}
void siggen_set_frequency(uint32_t freq) { (void)freq; }
void siggen_set_arb_waveform(const uint8_t *samples, uint16_t len) { (void)samples; (void)len; }
void settings_note(const settings_state_t *settings) { (void)settings; }
void settings_flush(void) {}
uint8_t settings_load(settings_state_t *state) { (void)state; return 0; }
uint8_t battery_percent(void) { return 100; }
uint8_t battery_is_charging(void) { return 0; }
uint16_t battery_millivolts(void) { return 4200; }
void battery_update(void) {}
void battery_update_charging_status(void) {}
uint8_t usb_msc_store_screenshot(void) { return 0; }
boot_export_result_t usb_msc_export_bootloader(const char *version,
                                               boot_export_progress_fn_t progress) {
    (void)version; (void)progress; return BOOT_EXPORT_OK;
}
void board_buzzer_set_volume(uint8_t level) { (void)level; }
void board_backlight_set_level(uint8_t level) { (void)level; }
uint8_t arb_scan_files(void) { return 0; }
uint8_t arb_file_count(void) { return 0; }
uint8_t arb_load_file(uint8_t index, uint8_t *buffer, uint16_t max) {
    (void)index; (void)buffer; (void)max; return 0;
}
uint16_t arb_file_sample_count(uint8_t index) { (void)index; return 0; }
const char *arb_file_name(uint8_t index) { (void)index; return ""; }
void fw_update_status(fw_update_status_t *status) { (void)status; }
void lcd_render(lcd_render_fn_t fn, void *ctx) { (void)fn; (void)ctx; }
uint32_t load_counter_read(void) { return 0; }
uint32_t load_counter_elapsed(uint32_t start, uint32_t end) { return end - start; }
void lcd_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    (void)x; (void)y; (void)w; (void)h; (void)color;
}
void lcd_text(uint16_t x, uint16_t y, const char *text, uint16_t fg, uint16_t bg, uint8_t scale) {
    (void)x; (void)y; (void)text; (void)fg; (void)bg; (void)scale;
}
uint16_t lcd_text_width(const char *text, uint8_t scale) { (void)text; (void)scale; return 0; }
void lcd_frame(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    (void)x; (void)y; (void)w; (void)h; (void)color;
}
void lcd_text_center(uint16_t x, uint16_t y, uint16_t w, const char *text, uint16_t fg, uint16_t bg, uint8_t scale) {
    (void)x; (void)y; (void)w; (void)text; (void)fg; (void)bg; (void)scale;
}
void lcd_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    (void)x0; (void)y0; (void)x1; (void)y1; (void)color;
}

static void test_dmm_stats_and_ch1_keys(void) {
    // AUTO mode should not accumulate stats
    setup(DMM_MODE_AUTO);
    dmm_stats_reset();
    dmm_view_mode = DMM_VIEW_LIVE;
    dmm_valid = dmm_numeric_valid = 1;
    strcpy(dmm_unit, "V");
    strcpy(dmm_value, "5.000");
    dmm_stats_update();
    assert(!dmm_stats.valid);

    // DCV mode: near-zero readings are ignored, min waits for settle
    setup(DMM_MODE_DCV);
    dmm_stats_reset();
    dmm_view_mode = DMM_VIEW_LIVE;
    dmm_valid = dmm_numeric_valid = 1;
    strcpy(dmm_unit, "V");
    strcpy(dmm_value, "0.000");
    dmm_stats_update();
    assert(!dmm_stats.valid);

    // Sample 1 (rising transient when probe touches): max tracks, min not yet latched
    strcpy(dmm_value, "2.100");
    dmm_stats_update();
    assert(dmm_stats.valid && !dmm_stats.min_valid);
    assert(dmm_stats.max_value == 2100);

    // Sample 2 (rising): max updates, min still waiting
    strcpy(dmm_value, "4.950");
    dmm_stats_update();
    assert(dmm_stats.valid && !dmm_stats.min_valid);
    assert(dmm_stats.max_value == 4950);

    // Sample 3 (settled): min_valid becomes 1, min=5.000, max=5.000
    strcpy(dmm_value, "5.000");
    dmm_stats_update();
    assert(dmm_stats.valid && dmm_stats.min_valid);
    assert(dmm_stats.min_value == 5000);
    assert(dmm_stats.max_value == 5000);

    // Sample 4 & 5: track min and max
    strcpy(dmm_value, "4.850");
    dmm_stats_update();
    strcpy(dmm_value, "5.150");
    dmm_stats_update();
    assert(dmm_stats.min_value == 4850);
    assert(dmm_stats.max_value == 5150);

    // Lifting probes (0.000 V) preserves min and max
    strcpy(dmm_value, "0.000");
    dmm_stats_update();
    assert(dmm_stats.valid && dmm_stats.min_valid);
    assert(dmm_stats.min_value == 4850);
    assert(dmm_stats.max_value == 5150);

    // Cycling main display: LIVE -> MIN -> MAX -> MAX-MIN -> LIVE
    strcpy(dmm_value, "5.010");
    assert(strcmp(dmm_display_value(), "5.010") == 0);
    dmm_view_mode = (uint8_t)((dmm_view_mode + 1u) % DMM_VIEW_COUNT);
    assert(dmm_view_mode == DMM_VIEW_MIN);
    assert(strcmp(dmm_display_value(), "4.850") == 0);
    dmm_view_mode = (uint8_t)((dmm_view_mode + 1u) % DMM_VIEW_COUNT);
    assert(dmm_view_mode == DMM_VIEW_MAX);
    assert(strcmp(dmm_display_value(), "5.150") == 0);
    dmm_view_mode = (uint8_t)((dmm_view_mode + 1u) % DMM_VIEW_COUNT);
    assert(dmm_view_mode == DMM_VIEW_MAX_MIN);
    assert(strcmp(dmm_display_value(), "0.300") == 0);
    dmm_view_mode = (uint8_t)((dmm_view_mode + 1u) % DMM_VIEW_COUNT);
    assert(dmm_view_mode == DMM_VIEW_LIVE);
    assert(strcmp(dmm_display_value(), "5.010") == 0);

    // Reset statistics
    dmm_stats_reset();
    assert(!dmm_stats.valid && !dmm_stats.min_valid);
}

int main(void) {
    test_stock_decoding();
    test_all_numeric_digits();
    test_status_and_rejected_frames();
    test_relative_precision();
    test_synthetic_readings();
    test_diode_thresholds();
    test_packet_boundaries();
    test_dmm_i18n_units_and_labels();
    test_dmm_stats_and_ch1_keys();
    assert(strcmp(settings_value_text(SETTINGS_ROW_COUNT - 1u, NULL), EXPECTED_FW_VERSION) == 0);
    puts("DMM decoding, display, REL, continuity and UART framing tests passed");
    return 0;
}
