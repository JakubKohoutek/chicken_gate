#include "server.h"
#include "rtc.h"
#include "voltage.h"
#include "motor.h"

const char *apName = "chicken-gate"; // Try http://esp8266.local

// DNS server
const byte DNS_PORT = 53;
DNSServer dnsServer;

// Web server
ESP8266WebServer server(80);

static boolean setupMode = true;

void handleRoot () {
  Serial.println("Handle root");

  // If caprive portal, redirect instead of displaying the page
  if (captivePortal()) {
    return;
  }

  String page = ROOT_PAGE;

  server.sendHeader("Content-Length", String(page.length()));
  server.send(200, "text/html", page);
}

void handleUpdateTime () {
  char date[12] = {0};
  char time[9] = {0};

  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i) == "date") {
      strcpy(date, server.arg(i).c_str());
    }
    if (server.argName(i) == "time") {
      strcpy(time, server.arg(i).c_str());
    }
  }

  RtcDateTime newDate = RtcDateTime(date, time);
  Rtc.SetDateTime(newDate);

  Serial.println("New date set:");
  Serial.println(date);
  Serial.println(time);

  server.send(200, "text/plain", "Success");
}


void handleSetTimer () {
  char date[12] = {0};
  char time[9] = {0};
  String mode;

  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i) == "date") {
      strcpy(date, server.arg(i).c_str());
    }
    if (server.argName(i) == "time") {
      strcpy(time, server.arg(i).c_str());
    }
    if (server.argName(i) == "mode") {
      mode = server.arg(i);
    }
  }

  Serial.println(mode);
  RtcDateTime alarmTime = RtcDateTime(date, time);

  if (mode == "timerOne") {
    DS3231AlarmOne alarm1(
      alarmTime.Day(),
      alarmTime.Hour(),
      alarmTime.Minute(), 
      alarmTime.Second(),
      DS3231AlarmOneControl_HoursMinutesSecondsMatch
    );
    Rtc.SetAlarmOne(alarm1);
    Serial.println("New alarm 1 set:");
  }

  if (mode == "timerTwo") {
    DS3231AlarmTwo alarm2(
      alarmTime.Day(),
      alarmTime.Hour(),
      alarmTime.Minute(), 
      DS3231AlarmTwoControl_HoursMinutesMatch
    );
    Rtc.SetAlarmTwo(alarm2);
    Serial.println("New alarm 2 set:");
  }
  Serial.println(date);
  Serial.println(time);

  server.send(200, "text/plain", "Success");
}

void handleDone () {
  server.send(200, "text/plain", "Success");
  setupMode = false;
}

void handleReadVoltage () {
  float voltage = readVoltage();
  char voltageString[10];
  sprintf(voltageString, "%.2fV", voltage);

  String payload = String("{") +
    "\"voltage\":\"" + voltageString + "\"" +
  "}";
  server.send(200, "application/json", payload);
}

void handleReadTime () {
  RtcDateTime now = Rtc.GetDateTime();

  String timeString = String("") +
    "Date:" + now.Year() + '/' + now.Month() + '/' + now.Day() + " " +
    "Time:" + now.Hour() + ':' + now.Minute() + ':' + now.Second();
  String payload = String("{") +
    "\"time\":\"" + timeString + "\"" +
  "}";

  server.send(200, "application/json", payload);
}

void handleOpenNow () {
  openGate();
  server.send(200, "text/plain", "Success");
}

void handleCloseNow () {
  closeGate();
  server.send(200, "text/plain", "Success");
}

void handleNotFound () {
  // If caprive portal, redirect instead of displaying the page
  if (captivePortal()) {
    return;
  }

}

void startServer () {
  WiFi.softAP(apName);
  delay(500);
  Serial.println("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  /* Setup the DNS server redirecting all the domains to the apIP */
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/updateTime", handleUpdateTime);
  server.on("/setTimer", handleSetTimer);
  server.on("/readVoltage", handleReadVoltage);
  server.on("/readTime", handleReadTime);
  server.on("/open", handleOpenNow);
  server.on("/close", handleCloseNow);
  server.on("/done", handleDone);
  server.onNotFound(handleNotFound);
  server.begin();

  Serial.println("HTTP server started");

  while (setupMode) {
    dnsServer.processNextRequest();
    server.handleClient();
  }
}

boolean captivePortal() {
  // Redirect to captive portal if we got a request for another domain
  if (!isIp(server.hostHeader()) ) {
    Serial.println("Request redirected to captive portal");
    server.sendHeader("Location", String("http://") + toStringIp(server.client().localIP()), true);
    server.send( 302, "text/plain", "");
    server.client().stop(); // Empty content inhibits Content-length header so we have to close the socket ourselves.

    return true;
  }

  return false;
}

boolean isIp(String str) {
  for (size_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}

String toStringIp(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}