/*!
 *  @file MAX30205.h
 *
 *  This is a library for the Maxim Integrated MAX30205
 *
 *  Designed specifically to work with sOximeter project,
 *  but it should work for any other purposes.
 *
 *  These sensors use I2C to communicate, 2 pins are required to interface.
 *  The driver enables can also automatically find the right address and 
 *  also supports multi sensor.
 *
 *  MReza Naeemabadi (RS Systems)
 *
 *  MIT license, all text above must be included in any redistribution
 */

#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <vector>

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
    uint8_t ReadConfig(const uint8_t &id);
    float ReadTemperature(const uint8_t &id);

    MAX30205(){};
    ~MAX30205() = default; // deleting the multi objects
    MAX30205(const MAX30205 &source) = delete;
    MAX30205 &operator=(const MAX30205 &source) = delete;
    MAX30205(MAX30205 &&source) = delete;
    MAX30205 &operator=(MAX30205 &&source) = delete;

private:
    TwoWire *_wire;
    uint8_t _address;
    std::vector<uint8_t> _addresses;

    void read(const uint8_t &_register, const uint8_t &_length, uint8_t *_values);
    uint8_t read(const uint8_t &_register);
    void write(const uint8_t &_register, const uint8_t &_value);
};