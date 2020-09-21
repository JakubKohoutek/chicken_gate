#ifndef SERVER_H
#define SERVER_H

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>

#include "index.h"

void    handleRoot      ();

void    handleSave      ();

void    handleNotFound  ();

void    startServer     ();

boolean captivePortal   ();

boolean isIp            (String str);

String  toStringIp      (IPAddress ip);

#endif // SERVER_H