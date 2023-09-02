#include <mruby.h>
#include <mruby/data.h>
#include <mruby/variable.h>
#include <mruby/string.h>
#include <mruby/array.h>
#include <mruby/presym.h>

#include <stdio.h>
#include <string.h>

#if ESP_PLATFORM
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#else
#define ESP_LOGI(tag, fmt, ...)  printf(fmt, __VA_ARGS__)
#endif

#define TAG ("mruby-esp32-adc")

#ifdef ESP_PLATFORM
static adc_oneshot_unit_handle_t mrb_adc_oneshot_unit_handle[2];
#endif

typedef struct adc_t {
  mrb_int pin;
  mrb_int unit;
  mrb_state *mrb;
} adc_t;

static void mrb_adc_free(mrb_state *mrb, void *p);

static const struct mrb_data_type mrb_adc = {
  "mrb_mruby_esp32_adc", mrb_adc_free
};

static void
mrb_adc_free(mrb_state *mrb, void *p) {
  adc_t *adc = (adc_t *)p;

  mrb_free(mrb, p);
}

static mrb_value
mrb_adc_init(mrb_state *mrb, mrb_value self) {
  adc_t *adc = mrb_malloc(mrb, sizeof(adc_t));

  mrb_get_args(mrb, "ii", &adc->pin, &adc->unit);
  adc->mrb = mrb;

  ESP_LOGI(TAG, "initialize(pin: %d, unit: %d)", adc->pin, adc->unit);

#ifdef ESP_PLATFORM
  adc_oneshot_chan_cfg_t chan_cfg = {
    .bitwidth = ADC_BITWIDTH_DEFAULT,
    .atten = ADC_ATTEN_DB_11,
  };
  ESP_ERROR_CHECK(adc_oneshot_config_channel(mrb_adc_oneshot_unit_handle[adc->unit], adc->pin, &chan_cfg));
#endif

  mrb_data_init(self, adc, &mrb_adc);

  return self;
}

static mrb_value
mrb_adc_read_raw(mrb_state *mrb, mrb_value self) {
  adc_t *adc = (adc_t *) DATA_PTR(self);
  mrb_int value = 0;

  ESP_LOGI(TAG, "read_raw(%s)", "");

#ifdef ESP_PLATFORM
  ESP_ERROR_CHECK(adc_oneshot_read(mrb_adc_oneshot_unit_handle[adc->unit], adc->pin, &value));
#endif

  return mrb_int_value(adc->mrb, value);
}

void
mrb_mruby_esp32_adc_gem_init(mrb_state* mrb) {
  struct RClass *class = mrb_define_class(mrb, "ADC", mrb->object_class);

#ifdef ESP_PLATFORM
  for(int i = 0 ; i < 2 ; i++) {
    adc_oneshot_unit_init_cfg_t init_cfg = {
        .unit_id = i,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_cfg, &mrb_adc_oneshot_unit_handle[i]));
  }

  mrb_define_const(mrb, class, "CHANNEL_0", mrb_fixnum_value(ADC_CHANNEL_0));
  mrb_define_const(mrb, class, "CHANNEL_1", mrb_fixnum_value(ADC_CHANNEL_1));
  mrb_define_const(mrb, class, "CHANNEL_2", mrb_fixnum_value(ADC_CHANNEL_2));
  mrb_define_const(mrb, class, "CHANNEL_3", mrb_fixnum_value(ADC_CHANNEL_3));
  mrb_define_const(mrb, class, "CHANNEL_4", mrb_fixnum_value(ADC_CHANNEL_4));

#if defined(CONFIG_IDF_TARGET_ESP32)   || defined(CONFIG_IDF_TARGET_ESP32S2) || \
    defined(CONFIG_IDF_TARGET_ESP32S3) || defined(CONFIG_IDF_TARGET_ESP32C6)
  mrb_define_const(mrb, class, "CHANNEL_5", mrb_fixnum_value(ADC_CHANNEL_5));
  mrb_define_const(mrb, class, "CHANNEL_6", mrb_fixnum_value(ADC_CHANNEL_6));
#endif

#if defined(CONFIG_IDF_TARGET_ESP32)   || defined(CONFIG_IDF_TARGET_ESP32S2) || \
    defined(CONFIG_IDF_TARGET_ESP32S3)
  mrb_define_const(mrb, class, "CHANNEL_7", mrb_fixnum_value(ADC_CHANNEL_7));
  mrb_define_const(mrb, class, "CHANNEL_8", mrb_fixnum_value(ADC_CHANNEL_8));
  mrb_define_const(mrb, class, "CHANNEL_9", mrb_fixnum_value(ADC_CHANNEL_9));
#endif

  mrb_define_const(mrb, class, "UNIT_1", mrb_fixnum_value(ADC_UNIT_1));
  mrb_define_const(mrb, class, "UNIT_2", mrb_fixnum_value(ADC_UNIT_2));
#endif

  mrb_define_method(mrb, class, "__initialize", mrb_adc_init, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, class, "__read_raw", mrb_adc_read_raw, MRB_ARGS_NONE());
}

void
mrb_mruby_esp32_adc_gem_final(mrb_state* mrb) {
#ifdef ESP_PLATFORM
  for(int i = 0 ; i < 2 ; i++) {
    ESP_ERROR_CHECK(adc_oneshot_del_unit(mrb_adc_oneshot_unit_handle[i]));
  }
#endif
}
