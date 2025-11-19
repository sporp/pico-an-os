#include "bsp/board.h"
#include "tusb.h"
#include "pico/stdlib.h"

// HID keycodes
#define HID_KEY_F2    0x3B
#define HID_KEY_F8    0x41
#define HID_KEY_DOWN  0x51
#define HID_KEY_ENTER 0x28

// Simple function to send one key
void send_key(uint8_t keycode)
{
    uint8_t keycode_report[6] = {keycode, 0, 0, 0, 0, 0};
    uint8_t empty_report[6] = {0, 0, 0, 0, 0, 0};

    // Wait until TinyUSB is ready to send
    while (!tud_hid_ready()) {
        tud_task();
    }

    // Send key down
    tud_hid_keyboard_report(0, 0, keycode_report);

    // Short delay to ensure host sees key down
    sleep_ms(20);

    // Wait until TinyUSB is ready to send the release
    while (!tud_hid_ready()) {
        tud_task();
    }

    // Send key up
    tud_hid_keyboard_report(0, 0, empty_report);
}

void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len) {
    (void)instance;
    (void)report;
    (void)len;
    // No-op
}

// HID report descriptor (keyboard)
uint8_t const desc_hid_report[] = { TUD_HID_REPORT_DESC_KEYBOARD() };

uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance) {
    (void)instance;
    return desc_hid_report;
}


int main(void) {
    board_init();
    tusb_init();

    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    int next_press_delay_ms = 1000;
    int phase_2_delay_ms = 500;
    int phase = 0;       // 0 = Spacebar loop, 1 = F8 + Down ×4 + Enter
    int step_count_f2 = 0;
    int step_count = 0;  // Step tracker for Phase 1

    // Timers
    absolute_time_t start_time = get_absolute_time();
    absolute_time_t next_press = delayed_by_ms(start_time, next_press_delay_ms);

    while (1) {
        tud_task();  // TinyUSB device task

        absolute_time_t now = get_absolute_time();

        // -----------------------------
        // Phase 0: Spacebar every 1 second
        // -----------------------------
        if (phase == 0) {
            int64_t elapsed_us = absolute_time_diff_us(now, start_time);
            if (step_count_f2 <= 24) { // 30 seconds
                if (absolute_time_diff_us(now, next_press) <= 0) {
                    // LED feedback
                    gpio_put(LED_PIN, 1);
                    send_key(HID_KEY_F2);
                    gpio_put(LED_PIN, 0);

                    // Schedule next key press 1 second after previous
                    next_press = delayed_by_ms(next_press, next_press_delay_ms);
                    ++step_count_f2;
                }
            } else {
                // Move to Phase 1
                phase = 1;
                step_count = 0;
                next_press = delayed_by_ms(now, next_press_delay_ms);  // first step 1s later
            }
        }

        // -----------------------------
        // Phase 1: F8 -> Down ×4 -> Enter
        // -----------------------------
        if (phase == 1) {
            if (absolute_time_diff_us(now, next_press) <= 0) {
                // LED step indicator: blink step_count+1 times
                for (int i = 0; i <= step_count; i++) {
                    gpio_put(LED_PIN, 1);
                    sleep_ms(100);
                    gpio_put(LED_PIN, 0);
                    sleep_ms(100);
                }

                // Send the key for this step
                if (step_count == 0) {
                    send_key(HID_KEY_F8);
                } else if (step_count >= 1 && step_count <= 4) {
                    send_key(HID_KEY_DOWN);
                } else if (step_count == 5) {
                    send_key(HID_KEY_ENTER);
                    phase = 2;  // Finished sequence
                }

                step_count++;
                next_press = delayed_by_ms(next_press, phase_2_delay_ms);  // 1 second to next step
            }
        }
    }

    return 0;
}

// TinyUSB HID descriptor callbacks
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id,
                               hid_report_type_t report_type,
                               uint8_t* buffer, uint16_t reqlen) {
    (void)instance;
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)reqlen;
    return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id,
                           hid_report_type_t report_type,
                           uint8_t const* buffer, uint16_t bufsize) {
    (void)instance;
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)bufsize;
}


// -------------------- Device Descriptor --------------------
tusb_desc_device_t const desc_device = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = 0x00,
    .bDeviceSubClass = 0x00,
    .bDeviceProtocol = 0x00,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor = 0xCafe,
    .idProduct = 0x4000,
    .bcdDevice = 0x0100,
    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,
    .bNumConfigurations = 0x01
};

uint8_t const *tud_descriptor_device_cb(void) {
    return (uint8_t const *)&desc_device;
}

// -------------------- Configuration Descriptor --------------------
enum {
    ITF_NUM_HID,
    ITF_NUM_TOTAL
};

#define CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)

uint8_t const desc_configuration[] = {
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
    TUD_HID_DESCRIPTOR(ITF_NUM_HID, 0, HID_ITF_PROTOCOL_KEYBOARD, sizeof(desc_hid_report), 0x81, 8, 10)
};

uint8_t const *tud_descriptor_configuration_cb(uint8_t index) {
    (void)index;
    return desc_configuration;
}

// -------------------- String Descriptors --------------------
char const *string_desc_arr[] = {
    (const char[]){0x09, 0x04}, // 0: supported language (English)
    "Raspberry Pi",             // 1: Manufacturer
    "F2 Keyboard",              // 2: Product
    "123456"                    // 3: Serial
};

static uint16_t _desc_str[32];

uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    (void)langid;
    uint8_t chr_count;

    if (index == 0) {
        memcpy(&_desc_str[1], string_desc_arr[0], 2);
        chr_count = 1;
    } else {
        if (!(index < sizeof(string_desc_arr)/sizeof(string_desc_arr[0]))) return NULL;

        const char *str = string_desc_arr[index];
        chr_count = strlen(str);
        if (chr_count > 31) chr_count = 31;

        for (uint8_t i = 0; i < chr_count; i++) {
            _desc_str[1 + i] = str[i];
        }
    }

    _desc_str[0] = (TUSB_DESC_STRING << 8) | (2*chr_count + 2);
    return _desc_str;
}
