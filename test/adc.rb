assert('ADC.new') do
  adc = ADC.new(1)
  assert_equal(adc.pin, 1)
  assert_equal(adc.unit, 0)

  adc = ADC.new(2, unit: 3)
  assert_equal(adc.pin, 2)
  assert_equal(adc.unit, 3)
end

assert('ADC#read_raw') do
  adc = ADC.new(1)
  ret = adc.read_raw
  assert_equal(ret, 0)
end

assert('ADC#read') do
  adc = ADC.new(1)
  assert_raise(NotImplementedError) do
    ret = adc.read
  end
end

assert('ADC#read_voltage') do
  adc = ADC.new(1)
  assert_raise(NotImplementedError) do
    ret = adc.read_voltage
  end
end
