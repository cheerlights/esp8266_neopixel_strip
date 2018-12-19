/*
 Set Latest CheerLights Color on a WS2812 NeoPixel Strip via ESP8266 Wi-Fi

 This sketch periodically checks the CheerLights color that is stored in
 ThingSpeak channel 1417 and sets the color of a WS2812-based NeoPixel strip.

 Requirements:

   * ESP8266 Wi-Fi Device
   * Arduino 1.8.8+ IDE
   * Additional Boards URL: http://arduino.esp8266.com/stable/package_esp8266com_index.json
   * Library: esp8266 by ESP8266 Community
   * Library: ThingSpeak by MathWorks
   * Library: Adafruit NeoPixel by Adafruit

 Setup Wi-Fi:
  * Enter SECRET_SSID in "secrets.h"
  * Enter SECRET_PASS in "secrets.h"

 CheerLights Project: http://cheerlights.com

 Created: Dec 19, 2018 by Hans Scharler (http://nothans.com)
*/

#include "ThingSpeak.h"
#include "secrets.h"

unsigned long cheerLightsChannelNumber = 1417;

#include <ESP8266WiFi.h>

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key index number (needed only for WEP)
WiFiClient  client;

#include <Adafruit_NeoPixel.h>

#define PIN            13    // Enter the NeoPixel strip pin number
#define NUMPIXELS      24    // Enter how many NeoPixels are connected on the strip

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800); // Set up the NeoPixel library

void setup() {

  Serial.begin(115200);
  delay(100);

  pixels.begin(); // Initialize the NeoPixel library
  pixels.setBrightness(200); // set brightness of the strip

  WiFi.mode(WIFI_STA);

  ThingSpeak.begin(client);

}

// Define the supported CheerLights colors and their RGB values
String colorName[] = {"red", "pink", "green", "blue", "cyan", "white", "warmwhite", "oldlace", "purple", "magenta", "yellow", "orange"};

int colorRGB[][3] = { 255,   0,   0,  // "red"
                      255, 192, 203,  // "pink"
                        0, 255,   0,  // "green"
                        0,   0, 255,  // "blue"
                        0, 255, 255,  // "cyan"
                      255, 255, 255,  // "white"
                      255, 223, 223,  // "warmwhite"
                      255, 223, 223,  // "oldlace"
                      128,   0, 128,  // "purple"
                      255,   0, 255,  // "magenta"
                      255, 255,   0,  // "yellow"
                      255, 165,   0}; // "orange"

void loop() {

  int statusCode = 0;

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected");
  }

  // Read CheerLights color from ThingSpeak channel
  String color = ThingSpeak.readStringField(cheerLightsChannelNumber, 1);

  // Check the status of the read operation to see if it was successful
  statusCode = ThingSpeak.getLastReadStatus();

  if(statusCode == 200) {
    Serial.println("Latest CheerLights Color: " + color);
    setColor(color);
  }
  else {
    Serial.println("Problem reading channel. HTTP error code " + String(statusCode));
  }

  // Wait 20 seconds before checking again
  delay(20000);
}

void setColor(String color) {
  // Loop through the list of colors to find the matching color
  for (int colorIndex = 0; colorIndex < 12; colorIndex++) {
    if (color == colorName[colorIndex]) {
      // Set the color of each NeoPixel on the strip
      for (int pixel = 0; pixel < NUMPIXELS; pixel++) {
        pixels.setPixelColor(pixel, pixels.Color (colorRGB [colorIndex][0], colorRGB [colorIndex][1], colorRGB [colorIndex][2]));
      }
      pixels.show();
    }
  }
}