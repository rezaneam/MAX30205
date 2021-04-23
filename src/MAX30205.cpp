#include <MAX30205.h>

bool MAX30205::Initialize(const uint8_t &address, TwoWire &wire)
{
    _wire = &wire;
    _address = address;
    _wire->beginTransmission(_address);
    return Wire.endTransmission() == 0;
}

bool MAX30205::Initialize(TwoWire &wire)
{
    _wire = &wire;
    _address = BASE_ADDRESS;
    while (_address < LAST_ADDRESS)
    {
        _wire->beginTransmission(_address);

        if (Wire.endTransmission() == 0)
            if (ReadConfig() != 0xFF)
                return true;

        _address++;
    }
    return false;
}

uint8_t MAX30205::ReadConfig()
{
    return read(MAX30205_CONFIGURATION);
}

float MAX30205::ReadTemperature()
{
    uint8_t readRaw[2] = {0};
    read(MAX30205_TEMPERATURE, 2, &readRaw[0]); // read two bytes
    int16_t raw = readRaw[0] << 8 | readRaw[1]; //combine two bytes
    printf("RAW TEMP = %X\r\n", raw);
    return raw * 0.00390625;
}

void MAX30205::read(const uint8_t &_register, const uint8_t &_length, uint8_t *_values)
{
    _wire->beginTransmission(_address);
    _wire->write(_register);
    _wire->endTransmission();
    _wire->requestFrom(_address, _length);
    for (uint8_t i = 0; i < _length; i++)
        _values[i] = _wire->read();
}

uint8_t MAX30205::read(const uint8_t &_register)
{
    _wire->beginTransmission(_address);
    _wire->write(_register);
    _wire->endTransmission();
    _wire->requestFrom(_address, 1);
    return _wire->read();
}

void MAX30205::write(const uint8_t &_register, const uint8_t &_value)
{
    _wire->beginTransmission(_address);
    _wire->write((uint8_t)_register);
    _wire->write((uint8_t)_value);
    _wire->endTransmission();
}