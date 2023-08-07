#pragma once

#include <Arduino.h>
#include <vector>
#include <Wire.h>

typedef enum {
  PATTERN_NONE,
  PATTERN_HBEAT,
  PATTERN_ERR  
} led_patterns_type_t;

void handle_io_pattern(uint8_t pin, led_patterns_type_t target_pattern);

void bootloopOnButtonPress(int pin_num);
std::vector<byte> scan_i2c(TwoWire &i2c);

#ifdef HSPOTA
void begin_hspota(const char *passwd, const int port);
#endif