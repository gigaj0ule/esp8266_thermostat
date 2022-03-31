#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
 
const char* ssid = "LiterallyOnFire";
const char* password = "askthegatekeeper";

ESP8266WebServer server(80);

long on_from;
long on_to;
long off_from;
long off_to;

String memory = "";
 
const int StatusLed  = 14;
const int PollingLED = 2;

const int half_hour = 30 * 60;

const int Relay0 = 5;

int highLevel = 1;
int relay_1_state;

void setup(void){
  Serial.begin(115200);
  
  pinMode(StatusLed, OUTPUT);
  pinMode(PollingLED, OUTPUT);
  pinMode(Relay0, OUTPUT);

  digitalWrite(Relay0, highLevel);

  _1_0();
  
  digitalWrite(StatusLed,  0);
  digitalWrite(PollingLED, 1);

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  int ledtoggle = 0;
  while (WiFi.status() != WL_CONNECTED) {
    ledtoggle = !ledtoggle;
    Serial.print("Waiting for ");
    Serial.print(ssid);
    Serial.println("...");
    digitalWrite(StatusLed, ledtoggle);
    delay(500);
  }
   
  digitalWrite(StatusLed, 1);
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address ");
  Serial.println(WiFi.localIP());
   
  server.on("/", handle_root);
  server.on("/30_30", _30_30);
  server.on("/1_1", _1_1);
  server.on("/1_2", _1_2);
  server.on("/1_4", _1_4);
  server.on("/1_6", _1_6);
  server.on("/0_1", _0_1);
  server.on("/0_2", _0_2);
  server.on("/0_4", _0_4);
  server.on("/0_6", _0_6);
  server.on("/0_8", _0_8);
  server.on("/0_12", _0_12);
  server.on("/1_0", _1_0);
  
  server.begin();
  Serial.println("HTTP server started");
}

long now(){
  return millis();
}
 
void loop(void){
  int ledtoggle = 1;
  digitalWrite(StatusLed, 1);
  server.handleClient();

  if(off_from < now() && off_to > now()){
    digitalWrite(Relay0, !highLevel);
  }
  
  if(on_from < now() && on_to > now()){
    digitalWrite(Relay0, highLevel);
  }

  if(off_to < now() && on_to < now()){
    if(memory == "_30_30")  _30_30();
    if(memory == "_1_0")  _1_0();
    if(memory == "_1_1")  _1_1();
    if(memory == "_1_2")  _1_2();
    if(memory == "_1_4")  _1_4();
    if(memory == "_1_6")  _1_6();
    if(memory == "_1_0m")    _1_0();
    
    //if(memory == "_0_1")  _0_1();
    //if(memory == "_0_2")  _0_2();
    //if(memory == "_0_4")  _0_4();
    //if(memory == "_0_6")  _0_6();
    //if(memory == "_0_8")  _0_8();
    //if(memory == "_0_8")  _0_12();
    
    delay(50);
  }
 
  while (WiFi.status() != WL_CONNECTED) {
    ledtoggle = !ledtoggle;
    Serial.print("Waiting for ");
    Serial.print(ssid);
    Serial.println("...");
    digitalWrite(StatusLed, ledtoggle);
    delay(500);
  } 
} 


void handle_root(){ 
  String html = "";
  String html_1 = "<h1>Furnace Snoozer @ ";
  String html_2 = "</h1>Connected to ";
  String html_3 = "<br><br>";
  String html_4 = "Choose how long the thermostat should snooze for.<br><br>";
  String html_4_1 = "Currently set to <b>";
  String html_4_2 = "";
  String html_4_3 = "</b><br><br><br>";
  String html_4_4 = "<a href='./30_30'>Auto-snooze every 30 min, resume every 30 min</a><br><br>";
  String html_5 = "<a href='./1_1'>Auto-snooze every hour, resume every hour</a><br><br>";
  String html_6 = "<a href='./1_2'>Auto-snooze every hour, resume every 2 hour</a><br><br>";
  String html_7 = "<a href='./1_4'>Auto-snooze every hour, resume every 4 hour</a><br><br>";
  String html_8 = "<a href='./1_6'>Auto-snooze every hour, resume every 6 hour</a><br><br>";
  String html_8_1 = "<a href='./0_1'>Keep settings, Snooze now for 1 hour</a><br><br>";
  String html_9 = "<a href='./0_2'>Keep settings, Snooze now for 2 hour</a><br><br>";
  String html_10 = "<a href='./0_4'>Keep settings, Snooze now for 4 hour</a><br><br>";
  String html_11 = "<a href='./0_6'>Keep settings, Snooze now for 6 hour</a><br><br>";
  String html_12 = "<a href='./0_8'>Keep settings, Snooze now for 8 hour</a><br><br>";
  String html_13 = "<a href='./0_12'>Keep settings, Snooze now for 12 hour</a><br><br>";
  String html_14 = "<a href='./1_0'>Stay ON</a><br>";


  if(memory == "_1_0") html_4_2 = "ON (no snooze)";
  if(memory == "_30_30") html_4_2 = "On 0.5 hr, Snooze 0.5 hr";
  if(memory == "_1_1") html_4_2 = "On 1 hr, Snooze 1 hr";
  if(memory == "_1_2") html_4_2 = "On 1 hr, Snooze 2 hr";
  if(memory == "_1_4") html_4_2 = "On 1 hr, Snooze 4 hr";
  if(memory == "_1_6") html_4_2 = "On 1 hr, Snooze 6 hr";
  if(memory == "_0_1") html_4_2 = "Snooze 1 hr";           
  if(memory == "_0_2") html_4_2 = "Snooze 2 hr";
  if(memory == "_0_4") html_4_2 = "Snooze 4 hr";
  if(memory == "_0_6") html_4_2 = "Snooze 6 hr";
  if(memory == "_0_8") html_4_2 = "Snooze 8 hr";
  if(memory == "_0_12") html_4_2 = "Snooze 12 hr";
  if(memory == "ON")    html_4_2 = "ON (no snooze)";
  
  IPAddress ip = WiFi.localIP();
  char sprintf_buffer[128];
  sprintf(sprintf_buffer, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

  html.concat(html_1);
  html.concat(sprintf_buffer);
  html.concat(html_2); 
  html.concat(ssid);
  html.concat(html_3); 
  html.concat(html_4);
  html.concat(html_4_1);
  html.concat(html_4_2);
  html.concat(html_4_3);
  html.concat(html_4_4);
  html.concat(html_5);
  html.concat(html_6);
  html.concat(html_7);
  html.concat(html_8);
  html.concat(html_8_1);
  html.concat(html_9);
  html.concat(html_10);
  html.concat(html_11);
  html.concat(html_12);
  html.concat(html_13);
  html.concat(html_14);

  server.send(200, "text/html", html);
}

void _30_30(){
  on_from  = now();
  on_to    = now() + 1000 * half_hour * 1;
  off_from = on_to + 1;
  off_to   = now() + 1000 * half_hour * 2;
  
  memory = "_30_30";
  handle_root();
}

void _1_1(){
  on_from  = now();
  on_to    = now() + 1000 * half_hour * 2 * 1;
  off_from = on_to + 1;
  off_to   = now() + 1000 * half_hour * 2 * 2;
  
  memory = "_1_1";
  handle_root();
}

void _1_2(){
  on_from  = now();
  on_to    = now() + 1000 * half_hour * 2 * 1;
  off_from = on_to + 1;
  off_to   = now() + 1000 * half_hour * 2 * 3;

  memory = "_1_2";
  handle_root();
}

void _1_4(){
  on_from  = now();
  on_to    = now() + 1000 * half_hour * 2 * 1;
  off_from = on_to + 1;
  off_to   = now() + 1000 * half_hour * 2 * 5;

  memory = "_1_4";
  handle_root();
}

void _1_6(){
  on_from  = now();
  on_to    = now() + 1000 * half_hour * 2 * 1;
  off_from = on_to + 1;
  off_to   = now() + 1000 * half_hour * 2 * 7;

  memory = "_1_6";
  handle_root();
}

void _0_1(){
  on_from = 0;
  on_to   = 0;
  off_from = now();
  off_to   = now() + 1000 * half_hour * 2 * 1;

  handle_root();
}

void _0_2(){
  on_from = 0;
  on_to   = 0;  
  off_from = now();
  off_to   = now() + 1000 * half_hour * 2 * 2;

  handle_root();
}

void _0_4(){
  on_from = 0;
  on_to   = 0;  
  off_from = now();
  off_to   = now() + 1000 * half_hour * 2 * 4;

  handle_root();
}

void _0_6(){
  on_from = 0;
  on_to   = 0;  
  off_from = now();
  off_to   = now() + 1000 * half_hour * 2 * 6;

  handle_root();
}

void _0_8(){
  on_from = 0;
  on_to   = 0;  
  off_from = now();
  off_to   = now() + 1000 * half_hour * 2 * 8;

  handle_root();
}

void _0_12(){
  on_from = 0;
  on_to   = 0;  
  off_from = now();
  off_to   = now() + 1000 * half_hour * 2 * 12;

  handle_root();
}

void _1_0(){
  on_from = 0;
  on_to   = 2147483647;  
  off_from = 0;
  off_to   = 0;

  handle_root();
  memory = "_1_0";
}


