class ADC
  attr_accessor :pin, :unit

  def initialize(pin, **kwargs)
    self.pin = pin
    self.unit = kwargs[:unit] || 0

    __initialize(self.pin, self.unit)
  end

  def read_raw
    __read_raw
  end

  def read_voltage
    raise not_inplemented_error('ADC#read_voltage, ADC#read')
  end

  alias read read_voltage

  def not_inplemented_error(name)
    NotImplementedError.new("'#{name}' is not implemented")
  end
end