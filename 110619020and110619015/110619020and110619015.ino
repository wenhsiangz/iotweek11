#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

String website;
const char* ssid = "A";
const char* password = "andy0910410";
char webSite[1000];
WebServer server(80);

const int led = 25;

void handleRoot() {
  snprintf(webSite,1000,"<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"> <script src=\"https://code.jquery.com/jquery-2.2.4.min.js\"></script> <script>setInterval(requestData,500);function requestData(){$.get(\"/handleSensorData\").done(function(data){if(data){$(\"#resis\").text(data.vr);}else{$(\"#resis\").text(\"?\");}}).fail(function(){console.log(\"fail\");});}</script> <title>VR Reader</title></head><body><div class=\"container\"><p>Variable Resistor = <span id=\"resis\"></span></p></div></body></html>");
  server.send(200, "text/html",webSite);

}
void handleSensorData(){
  int sensor=analogRead(33);
  String json="{\"vr\": ";
  json+=sensor;
  json+="}";
  Serial.println(sensor);
  server.send(200,"appliction/json",json);
  
  }

  

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

 
  

void setup(void) {
  ledcSetup(0,5000,8);
  ledcAttachPin(13,0);
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  
  server.on("/", handleRoot);
  server.on("/handleSensorData",handleSensorData);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
