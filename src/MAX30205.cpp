#include <MAX30205.h>

/*!
 *  @brief  Instantiates the private members
 *  @param  address
 *          i2c address of the sensor
 *  @param  wire
 *          Wire object, the default value is Wire.
 *  @return true if sensor is found
 */
bool MAX30205::Initialize(const uint8_t &address, TwoWire &wire)
{
    _wire = &wire;
    _address = address;
    _wire->beginTransmission(_address);
    if (Wire.endTransmission() == 0)
        if (ReadConfig() != 0xFF)
            return true;
    return false;
}

/*!
 *  @brief  Instantiates the private members and resolves the address of the sensor automatically.
 *  @param  wire
 *          Wire object, the default value is Wire.
 *  @return true if sensor is found
 */
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

/*!
 *  @brief  Instantiates the private members and find all the available sensors on the bus
 *  @param  wire
 *          Wire object, the default value is Wire.
 *  @return true if sensor is found
 */
bool MAX30205::InitializeMulti(TwoWire &wire)
{
    _wire = &wire;
    uint8_t add = BASE_ADDRESS;
    while (add < LAST_ADDRESS)
    {
        _wire->beginTransmission(add);

        if (Wire.endTransmission() == 0)
            if (ReadConfig() != 0xFF)
                _addresses.emplace_back(add);

        add++;
    }
    return _addresses.size() > 0;
}

/*!
 *  @brief  Returns the configuration of the target sensor id, uses default if multi-sensor is not initialized.
 *  @param  sensor_id
 *          sensor id. uses the last sensor if given number is bigger than the available sensors.
 *  @return Configuration of the sensor
 */
uint8_t MAX30205::ReadConfig(const uint8_t &id)
{
    if (_addresses.size() == 0)
        return ReadConfig();
    if (_addresses.size() <= id)
        _address = _addresses.back();
    else
        _address = _addresses.at(id);
    return ReadConfig();
}

/*!
 *  @brief  Returns the configuration of the default sensor 
 *  @return Configuration of the sensor
 */
uint8_t MAX30205::ReadConfig()
{
    return read(MAX30205_CONFIGURATION);
}

/*!
 *  @brief  Returns the configuration of the target sensor id, uses default if multi-sensor is not initialized.
 *  @param  sensor_id
 *          i2c address of the sensor
 *  @return temperature measured by the target sensor
 */
float MAX30205::ReadTemperature(const uint8_t &id)
{
    if (_addresses.size() == 0)
        return ReadTemperature();
    if (_addresses.size() <= id)
        _address = _addresses.back();
    else
        _address = _addresses.at(id);
    return ReadTemperature();
}

/*!
 *  @brief  Returns the measurment of the default sensor 
 *  @return temperature measured by the default sensor
 */
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