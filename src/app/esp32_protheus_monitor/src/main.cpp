#include <Arduino.h>

#include <WiFiManager.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Wire.h>
#include <SPI.h>

#define I2C_SDA 8
#define I2C_SCL 9

#define PIN_AP 23

#define PIN_BUZZER 20

#define APERTADO LOW

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C
#define SCREEN_RESET -1 // Reset pin not used, set to -1
#define ROTATION 0      // No rotation

#include "logo.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RESET);

char nomeWifi[] = "Protheus Monitor WIFI";
char senhaWifi[] = "12345678";

int nDelay = 10000; // Set the delay time in milliseconds

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  Wire.begin(I2C_SDA, I2C_SCL);

  // Initialize the display and show the logo
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.drawBitmap(0, 0, epd_bitmap_logo_esp32_protheus_monitor, 128, 64, WHITE);
  display.display();

  delay(5000); // Wait for 2 seconds to display the logo

  display.clearDisplay();

  pinMode(PIN_BUZZER, OUTPUT);
}

// put your main code here, to run repeatedly:
void loop()
{

  digitalWrite(PIN_BUZZER, HIGH); // Activate the buzzer

  display.setCursor(0, 0); // Parte amarela (topo)
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("MONITOR PROTHEUS");

  display.setCursor(0, 16); // Parte azul
  display.setTextSize(1);
  display.println("Licencas:  12");
  display.println("Usuarios:  4");
  display.println("CPU:      38%");
  display.display();

  delay(1000); // Delay for 1 second
}
