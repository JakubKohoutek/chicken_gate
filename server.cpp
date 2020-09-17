#include "server.h"

const char *apName = "chicken-gate"; // Try http://esp8266.local

// DNS server
const byte DNS_PORT = 53;
DNSServer dnsServer;

// Web server
ESP8266WebServer server(80);

void handleRoot () {
  Serial.println("Handle root");

  // If caprive portal, redirect instead of displaying the page
  if (captivePortal()) {
    return;
  }

  String page = HTTP_HEADER;
  page += HTTP_SCRIPT;
  page += HTTP_STYLE;
  page += HTTP_HEADER_END;
  page += String("<h1>Chicken Gate</h1>");
  page += HTTP_END;

  server.sendHeader("Content-Length", String(page.length()));
  server.send(200, "text/html", page);
}

void handleSave() {
  // for (uint8_t i = 0; i < server.args(); i++) {
  //   if (server.argName(i) == "code") {
  //     String commmand = server.arg(i);
  //     uint32_t code = translateCommandToCode(commmand);
  //     irsend.sendNEC(code, 32);
  //     String json = "{\"command\":\"" + commmand + "\", \"code\":\"" + code + "\"}";
  //     server.send(200, "application/json", json.c_str());
  //   }
  // }
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
  server.onNotFound(handleNotFound);
  server.begin();

  Serial.println("HTTP server started");

  while (true){
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