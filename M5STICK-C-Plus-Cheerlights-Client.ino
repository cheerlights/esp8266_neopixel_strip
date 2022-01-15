/*
 Set Latest CheerLights Color as screen color on M5STICK-C Plus

 This sketch periodically checks the CheerLights color that is stored in
 ThingSpeak channel 1417 and sets the color on the screen of M5STICK-C plus.

 Requirements:

   * M5STICK-C Plus Device 
   * Arduino 1.8.8+ IDE
   * Library: ThingSpeak by MathWorks
   * Library: M5StickCPlus by Expressif

 Setup Wi-Fi:
  * Enter SECRET_SSID in "secrets.h"
  * Enter SECRET_PASS in "secrets.h"

 CheerLights Project: http://cheerlights.com

 History:
 Created 15 Jan 22 by David Marks https://github.com/marksyuk/M5STICK-C-Plus-Cheerlights-Client
 
 Acknowledgements:
 This was shamelessly hacked from https://github.com/cheerlights/esp8266_neopixel_strip created: Dec 19, 2018 by Hans Scharler (http://nothans.com)
*/
#include <M5StickCPlus.h>
#include "ThingSpeak.h"
#include "secrets.h"

unsigned long cheerLightsChannelNumber = 1417;

#include <WiFi.h>

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key index number (needed only for WEP)
WiFiClient  client;

#define PIN            13    // Enter the NeoPixel strip pin number
#define NUMPIXELS      24    // Enter how many NeoPixels are connected on the strip

// clearlight colours in RGB565, since that's what STICK-C uses for screen colour
#define CL_red 0xF800
#define CL_pink 0xFE19
#define CL_green 0x07E0
#define CL_blue 0x001F
#define CL_cyan 0x07FF
#define CL_white 0xFFFF
#define CL_warmwhite 0xFEFB
#define CL_oldlace 0xFEFB
#define CL_purple 0x8010
#define CL_magenta 0xF81F
#define CL_yellow 0xFFE0
#define CL_orange 0xFD20

void setup() {

  // initialize the M5StickC object
  M5.begin();
  M5.Lcd.fillScreen(CL_red);

  Serial.begin(115200);
  delay(100);

  WiFi.mode(WIFI_STA);

  ThingSpeak.begin(client);

}

// Define the supported CheerLights colors and their RGB values
String colorName[] = {"red", "pink", "green", "blue", "cyan", "white", "warmwhite", "oldlace", "purple", "magenta", "yellow", "orange"};

int colorCL[][1] = { CL_red,           // "red"
                    CL_pink,          // "pink"
                    CL_green,         // "green"
                    CL_blue,          // "blue"
                    CL_cyan,          // "cyan"
                    CL_white,         // "white"
                    CL_warmwhite,     // "warmwhite"
                    CL_oldlace,       // "oldlace"
                    CL_purple,        // "purple"
                    CL_magenta,       // "magenta"
                    CL_yellow,        // "yellow"
                    CL_orange};       // "orange"


void loop() {

  int statusCode = 0;

  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected");
    M5.Lcd.printf("connected");
  }

  // Read CheerLights color from ThingSpeak channel
  String color = ThingSpeak.readStringField(cheerLightsChannelNumber, 1);
  String status = "";
  // Check the status of the read operation to see if it was successful
  statusCode = ThingSpeak.getLastReadStatus();

  if (statusCode == 200) {
    Serial.println("Latest CheerLights Color: " + color);
    M5.Lcd.print(color);
    setColor(color);
  }
  else {
    Serial.println("Problem reading channel. HTTP error code " + String(statusCode));
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 10);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.printf("Problem reading channel. HTTP error code ");

  }

  // Wait 20 seconds before checking again
  delay(20000);
}

void setColor(String color) {
  // Loop through the list of colors to find the matching color
  for (int colorIndex = 0; colorIndex < 12; colorIndex++) {
    if (color == colorName[colorIndex]) {
    // set the colour of the screen
    M5.Lcd.fillScreen(colorCL [colorIndex][0]);
    }
  }
}
