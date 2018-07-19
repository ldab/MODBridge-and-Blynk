#include <FS.h>             //this needs to be first, or it all crashes and burns...

#define BLYNK_PRINT Serial  /* Comment this out to disable prints and save space */

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ModbusMaster.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#define ku16MBResponseTimeout 1000   //MODBUS timeout

#define Host_Name ("MODBridge" + String(ESP.getChipId(), HEX))
#define BOARD_BUTTON_PIN              0                    // Pin where user button is attached
#define BOARD_BUTTON_ACTIVE_LOW       true                 // true if button is "active-low"
#define BOARD_LED_PIN_R               14                   // Set R,G,B pins
#define BOARD_LED_PIN_G               12
#define BOARD_LED_PIN_B               13
#define BOARD_LED_INVERSE             true                 // true if LED is common anode, false if common cathode
#define BOARD_LED_BRIGHTNESS          100                  // 0..255 brightness control
#define BOARD_PWM_MAX                 1023                 //ESP8266 10 bits PWM

#define DIMM(x)    (((x)*(BOARD_LED_BRIGHTNESS))/255)
#define RGB(r,g,b) (DIMM(r) << 16 | DIMM(g) << 8 | DIMM(b) << 0)

#define BLYNK_GREEN     "#23C48E"
#define BLYNK_YELLOW    "#ED9D00"
#define BLYNK_RED       "#D3435C"
#define BLYNK_BLUE      "#04C0F8"

enum Colors {
  COLOR_BLACK   = RGB(0x00, 0x00, 0x00),
  COLOR_WHITE   = RGB(0xFF, 0xFF, 0xE7),
  COLOR_BLUE    = RGB(0x0D, 0x36, 0xFF),
  COLOR_BLYNK   = RGB(0x2E, 0xFF, 0xB9),
  COLOR_RED     = RGB(0xFF, 0x10, 0x08),
  COLOR_MAGENTA = RGB(0xA7, 0x00, 0xFF),
};

uint32_t colorToBlink = COLOR_BLACK;

//Variables to be read
float kw_mod = 0;
String strkw_mod = "---";
float kwh_mod = 0;
String strkwh_mod = "---";
float volts_mod = 0;
String strvolts_mod = "---";
float amps_mod = 0;
String stramps_mod = "---";
float pf_mod = 0;
String strpf_mod = "---";
float thd_mod = 0;
String strthd_mod = "---";

uint8_t NotifyCounter = 0;          //Notification counter to avoid multiple App Notification
uint8_t result;                     //return flag from MODBUS read

char mqtt_server[40];
char mqtt_port[6] = "1883";
char blynk_token[34] = "YOUR_BLYNK_TOKEN";

//timerID names
int timerTOread;
int timerTOsend;
int timerTOblink;
int timerToLED;

bool NextAddress = false;           //variable to continue reading other registers if succesful
bool shouldSaveConfig = false;      //flag for saving data
bool LEDflick = false;

ModbusMaster MODBridge;       //initialize Modbus Master
BlynkTimer timer;             //Up To 16 Timers
WidgetRTC rtc;                //Initiate RTC
WidgetLED ledStatus(V5);      //LED Status
WidgetLED ledWIFI(V10);       //LED WiFi Status

BLYNK_CONNECTED() {
  rtc.begin();                  //Sync RTC
  timer.enable(timerTOsend);    //Enable timer when connect
  timer.enable(timerTOblink);   //Enable timer when connect
  ledWIFI.setColor("#FFFFFF");  //Set wifi LED color
  ledWIFI.on();                 //Turn it on
}

void preTransmission(){}          //Change if you need to do something before sending MODBUS comand
void postTransmission(){}         //Change if you need to do something after sending MODBUS comand

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void SendStuff()   // This function sends every 10 second to Virtual Pin
{
  Blynk.virtualWrite(V1, strkw_mod+"W");
  Blynk.virtualWrite(V11, strkw_mod);
  Blynk.virtualWrite(V2, stramps_mod+"A");
  Blynk.virtualWrite(V12, stramps_mod);
  Blynk.virtualWrite(V0, strvolts_mod+"V");
  Blynk.virtualWrite(V14, strvolts_mod);
  Blynk.virtualWrite(V3, strpf_mod);
  Blynk.virtualWrite(V13, strpf_mod);
  Blynk.virtualWrite(V4, strkw_mod+"KWH");
  Blynk.virtualWrite(V7, strthd_mod+"%");

  int RSSI = WiFi.RSSI();
  if(RSSI>=-75)         ledWIFI.setColor(BLYNK_GREEN);
  else if(RSSI<-85)     ledWIFI.setColor(BLYNK_YELLOW);
  else                  ledWIFI.setColor(BLYNK_RED);

  //Blynk.virtualWrite(V0, String(RSSI) + "dBm"); //Uncomment if you want V0 to show WiFistrength
}

String currentTime(){
  String hnow;
  String mnow;
      if (hour()<10) hnow = "0"+ String(hour());
    else hnow = String(hour());
      if (minute()<10) mnow = "0" + String(minute());
    else mnow = String(minute());
  return hnow+":"+mnow;
}

String DateAndTime(){
    String currentDate = String(day()) + "/" + month() + "/" + (year()-2000);
    return currentDate + " " + currentTime();
}

void NotifyControllerOffline()
{
  NextAddress = false;
  NotifyCounter++;
  if(NotifyCounter>4) NotifyCounter=4;
    if (NotifyCounter == 2) {
      strkw_mod = "---";
      strkwh_mod = "---";
      strvolts_mod = "---";
      stramps_mod = "---";
      strpf_mod = "---";
      strthd_mod = "---";

      Blynk.notify("🚨 {DEVICE_NAME} - OFF and/or disconnected!");
      Blynk.email(DateAndTime()+" -🚨 {DEVICE_NAME} ALARM", "OFF and/or disconnected!");
    }
}

void blinkButton()
{
    if(NotifyCounter) ledStatus.setColor(BLYNK_RED);
    else ledStatus.setColor(BLYNK_GREEN);

        //color set, now blink:
        if (ledStatus.getValue()) {
        ledStatus.off();
      }
      else  ledStatus.on();
}

void READModbus(){
    result = MODBridge.readInputRegisters(0x0000, 32); // Read 32 registers starting at 0x0000)
    if (result == MODBridge.ku8MBSuccess)
    {
      NextAddress = true;                                   //if success, read alarms and other variables
      uint32_t supportcalc = ((MODBridge.getResponseBuffer(0x00)<<16) | MODBridge.getResponseBuffer(0x01));
      volts_mod = *(float*)&supportcalc;
      supportcalc = ((MODBridge.getResponseBuffer(0x06)<<16) | MODBridge.getResponseBuffer(0x07));    //read buffer register 06
      amps_mod = *(float*)&supportcalc;
      supportcalc = ((MODBridge.getResponseBuffer(0x0C)<<16) | MODBridge.getResponseBuffer(0x0D));    //read buffer register 0C
      kw_mod = *(float*)&supportcalc;
      supportcalc = ((MODBridge.getResponseBuffer(0x1E)<<16) | MODBridge.getResponseBuffer(0x1F));    //read buffer register 1E
      pf_mod = *(float*)&supportcalc;

      NotifyCounter = 0;

      //Convert Float to String and then we can set to "---" when disconnected;
      strkw_mod=String(kw_mod);
      stramps_mod=String(amps_mod);
      strvolts_mod=String(volts_mod);
      strpf_mod=String(pf_mod);
    }
    else  NotifyControllerOffline();

    if (NextAddress) {
      result = MODBridge.readInputRegisters(0x48, 2); // Read 2 registers starting at 0x0048)
      if (result == MODBridge.ku8MBSuccess)   kwh_mod = ((MODBridge.getResponseBuffer(0x00)<<16) | MODBridge.getResponseBuffer(0x01));    //read buffer register 00
      else kwh_mod = sqrt(-1);

      result = MODBridge.readInputRegisters(0xEA, 2); // Read 2 registers starting at 0x00EA)
      if (result == MODBridge.ku8MBSuccess)   thd_mod = ((MODBridge.getResponseBuffer(0x00)<<16) | MODBridge.getResponseBuffer(0x01));    //read buffer register 00
      else thd_mod = sqrt(-1);

      strkw_mod=String(kwh_mod);
      strthd_mod=String(thd_mod);
    } // end of NextAddress
}//end of READModbus

void initLED() {
  pinMode(BOARD_LED_PIN_R, OUTPUT);
  pinMode(BOARD_LED_PIN_G, OUTPUT);
  pinMode(BOARD_LED_PIN_B, OUTPUT);
}

void setRGB(uint32_t color) {
  uint8_t r = (color & 0xFF0000) >> 16;
  uint8_t g = (color & 0x00FF00) >> 8;
  uint8_t b = (color & 0x0000FF);
  #if BOARD_LED_INVERSE
  analogWrite(BOARD_LED_PIN_R, BOARD_PWM_MAX - r);
  analogWrite(BOARD_LED_PIN_G, BOARD_PWM_MAX - g);
  analogWrite(BOARD_LED_PIN_B, BOARD_PWM_MAX - b);
  #else
  analogWrite(BOARD_LED_PIN_R, r);
  analogWrite(BOARD_LED_PIN_G, g);
  analogWrite(BOARD_LED_PIN_B, b);
  #endif
}

void printChipInfo(){
  Serial.print ("SDK:");
  Serial.println (ESP.getSdkVersion());
  Serial.print ("Core:");
  Serial.println (ESP.getCoreVersion());
  Serial.print ("Flash Chip size:");
  Serial.println (ESP.getFlashChipSize());
  Serial.print ("Sketch size:");
  Serial.println (ESP.getSketchSize());
  Serial.print ("Free sketch space:");
  Serial.println (ESP.getFreeSketchSpace());
  Serial.print ("Free Heap:");
  Serial.println (ESP.getFreeHeap());
  Serial.print ("Reset Reason:");
  Serial.println (ESP.getResetReason());
  Serial.println("local ip");
  Serial.println(WiFi.localIP());
}

void button_change(void)
{
          WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
          WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 6);
          WiFiManagerParameter custom_blynk_token("blynk", "blynk token", blynk_token, 34);

          //Local intialization. Once its business is done, there is no need to keep it around
          WiFiManager wifiManager;

          //set config save notify callback
          wifiManager.setSaveConfigCallback(saveConfigCallback);

          //add all your parameters here
          wifiManager.addParameter(&custom_mqtt_server);
          wifiManager.addParameter(&custom_mqtt_port);
          wifiManager.addParameter(&custom_blynk_token);

          //reset settings - for testing
          //wifiManager.resetSettings();

          //sets timeout until configuration portal gets turned off
          //useful to make it all retry or go to sleep in seconds
          wifiManager.setTimeout(120);

          if (!wifiManager.startConfigPortal(Host_Name.c_str())) {
          Serial.println("failed to connect and hit timeout");
          delay(3000);
          ESP.reset();
          delay(5000);
        }

        strcpy(mqtt_server, custom_mqtt_server.getValue());
        strcpy(mqtt_port, custom_mqtt_port.getValue());
        strcpy(blynk_token, custom_blynk_token.getValue());

        //save the custom parameters to FS
        if (shouldSaveConfig) {
          Serial.println("saving config");
          DynamicJsonBuffer jsonBuffer;
          JsonObject& json = jsonBuffer.createObject();
          json["mqtt_server"] = mqtt_server;
          json["mqtt_port"] = mqtt_port;
          json["blynk_token"] = blynk_token;

          File configFile = SPIFFS.open("/config.json", "w");
          if (!configFile) {
            Serial.println("failed to open config file for writing");
          }
          json.printTo(Serial);
          json.printTo(configFile);
          configFile.close();
          //end save
        }
}

void button_init()
{
#if BOARD_BUTTON_ACTIVE_LOW
  pinMode(BOARD_BUTTON_PIN, INPUT_PULLUP);
#else
  pinMode(BOARD_BUTTON_PIN, INPUT);
#endif
  //attachInterrupt(BOARD_BUTTON_PIN, button_change, LOW);
}

void spiffsinit(){
  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          strcpy(mqtt_server, json["mqtt_server"]);
          strcpy(mqtt_port, json["mqtt_port"]);
          strcpy(blynk_token, json["blynk_token"]);

        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
}

void blinkLED(){
    if (LEDflick) {
      setRGB(COLOR_BLACK);
      LEDflick = !LEDflick;
    }
    else {
      setRGB(colorToBlink);
      LEDflick = !LEDflick;
    }
}

void setup() {
    // put your setup code here, to run once:
    WiFi.hostname(Host_Name);

    Serial.begin(2400);               // Modbus communication runs at 2400 baud
    Serial.println();
    MODBridge.begin(1, Serial);       // Modbus slave ID 1

    initLED();                        //LED intialization
    button_init();                    //Config on request button intialization
    spiffsinit();                     //SPIFFS intialization

    //clean FS, for testing
    //SPIFFS.format();

    timerTOread = timer.setInterval(10000L, READModbus); //read the controller every 10 sec
    timerTOsend = timer.setInterval(15000L, SendStuff); //send stuff to the cloud every 15 second
    timerTOblink = timer.setInterval(800L, blinkButton);  //blink virtual LED every 0.8sec - LED
    timerToLED = timer.setInterval(500L, blinkLED);  //blink virtual LED every 0.8sec - LED
    timer.disable(timerTOsend);
    timer.disable(timerTOblink);
    timer.disable(timerTOread);

    printChipInfo();                  //Print board info
    Blynk.config(blynk_token);
    ArduinoOTA.begin();
}

void loop() {
    // put your main code here, to run repeatedly:
    timer.run();
    ArduinoOTA.handle();

    if (!digitalRead(BOARD_BUTTON_PIN)) button_change();      //If button is pressed, start configuration portal

    if(WiFi.status()!=WL_CONNECTED) {
      timer.disable(timerTOread); //DISABLE READINGS IF WIFI NOT CONNECTED TO AVOID CRASH
      colorToBlink = COLOR_RED;
    }
    else {
      colorToBlink = COLOR_BLYNK;
      Blynk.run();
      timer.enable(timerTOread);
    }
}
