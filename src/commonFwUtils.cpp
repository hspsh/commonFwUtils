#include "commonFwUtils.h"

#ifdef HSPOTA
#include <ArduinoOTA.h>
#endif

void handle_io_pattern(uint8_t pin, led_patterns_type_t target_pattern){
  static uint32_t pattern_counter = 0;
  static uint8_t heartbeat_pattern[] = {1,0,0,1,0,0,0,0,0,0,0,0,0};
  static uint8_t errcon_pattern[] = {1,0,1,0,1,0,1,1,1,0,0,0,0};
  
  switch (target_pattern){    
    case PATTERN_HBEAT:
      digitalWrite(pin, heartbeat_pattern[
        pattern_counter % sizeof(heartbeat_pattern)
      ]);
      break;
    
    case PATTERN_ERR:
      digitalWrite(pin, errcon_pattern[
        pattern_counter % sizeof(errcon_pattern)
      ]);
      break;
    case PATTERN_NONE:
    default:
      digitalWrite(pin,0);
      break;
  }
  pattern_counter++;
}

void bootloopOnButtonPress(int pin_num){
  pinMode(pin_num, INPUT_PULLUP);
  if (digitalRead(pin_num)==LOW){
    Serial.println("enter bootloop");
    while (1) {
      Serial.print(".");
      delay(1000);
    }
  }
}

std::vector<byte> scan_i2c(TwoWire &i2c){
  std::vector<byte> ret;
  for (byte i = 8; i < 120; i++)
  {
    i2c.beginTransmission (i);        // Begin I2C transmission Address (i)
    if (i2c.endTransmission () == 0)  // Receive 0 = success (ACK response) 
    {
      ret.push_back(i);
    }
  }
  return ret;
}


#ifdef HSPOTA
void begin_hspota(const char *passwd, const int port){

  ArduinoOTA.setPassword(passwd);
  ArduinoOTA.setPort(port);

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";
      //Serial.println("Start updating %s", type.c_str());
    });
    ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    //Serial.println(0,1000,"OTA status: %u%%", (progress / (total / 100)));
    });

    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR)          ;//Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR)    ;//Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR)  ;//Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR)  ;//Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR)      ;//Serial.println("End Failed");
    });

  ArduinoOTA.begin();
}
#endif