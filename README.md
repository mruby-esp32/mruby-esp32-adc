# mruby-esp32-adc

ADC library for mruby-esp32.

## Installation

You need [esp-idf v5.0](https://docs.espressif.com/projects/esp-idf/en/release-v5.0/esp32/index.html) to use this mrbgems.

Add the line below to your `build_config.rb`:

```ruby
  conf.gem :github => 'mruby-esp32/mruby-esp32-adc'
```

## Examples

```ruby
adc = ADC.new(ADC::CHANNEL_0)
value = adc.read_raw()

# If you want to use ADC_UNIT_2
adc = ADC.new(ADC::CHANNEL_0, unit: ADC::UNIT_2)
value = adc.read_raw()
```

## LICENSE

MIT License.
