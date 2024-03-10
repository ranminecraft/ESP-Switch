#include <ESP8266WiFi.h>
const char* ssid = "Ranica";
const char* password = "000000";

bool status = true;

WiFiServer server(80);
void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(0, OUTPUT);
  digitalWrite(0, 0);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  server.begin();
  Serial.println("Server started @ ");
  Serial.println(WiFi.localIP());
}
void loop() {
  WiFiClient client = server.available();
  if (!client) {
    delay(100);
    return;
  }
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  if (req.indexOf("/switch") == -1) {
    Serial.println("invalid request");
    client.print("HTTP/1.1 404\r\n");
    client.stop();
    return;
  }
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  if (status) {
      status = false;
      digitalWrite(0, 1);
      s += "off ";
  } else {
    status = true;
    digitalWrite(0, 0);
    s += "on ";
  }
  s += "</html>\n";
  client.flush();
  client.print(s);

  Serial.println(status);
  Serial.println(s);
  delay(100);
}