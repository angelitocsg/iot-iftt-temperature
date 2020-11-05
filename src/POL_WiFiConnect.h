#ifndef POL_WiFiConnect_h
#define POL_WiFiConnect_h

#include <ESP8266WiFi.h>
#include "Arduino.h"

class POL_WiFiConnect
{
  public:
    POL_WiFiConnect();
    void connect();
    IPAddress getIp();
    boolean connected();
  private:
    IPAddress ip;
};

#endif
