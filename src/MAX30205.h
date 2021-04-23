#pragma once

#include <Arduino.h>
#include <Wire.h>

// Address Range
#define BASE_ADDRESS 0x40
#define LAST_ADDRESS 0x5F

// Registers
#define MAX30205_TEMPERATURE 0x00   // 2 bytes
#define MAX30205_CONFIGURATION 0x01 // 1 byte
#define MAX30205_THYST 0x02         // 2 bytes
#define MAX30205_TOS 0x03           // 2 bytes

class MAX30205
{
public:
    bool Initialize(TwoWire &wire = Wire);
    bool Initialize(const uint8_t &address, TwoWire &wire = Wire);
    uint8_t ReadConfig();
    float ReadTemperature();

    bool InitializeMulti(TwoWire &wire = Wire);
    uint8_t ReadConfig(const uint8_t id);
    float ReadTemperature(const uint8_t id);

private:
    TwoWire *_wire;
    uint8_t _address;

    void read(const uint8_t &_register, const uint8_t &_length, uint8_t *_values);
    uint8_t read(const uint8_t &_register);
    void write(const uint8_t &_register, const uint8_t &_value);
};