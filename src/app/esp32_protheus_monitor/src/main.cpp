#include <Arduino.h>

#include <WiFiManager.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Wire.h>
#include <SPI.h>

#define I2C_SDA 8
#define I2C_SCL 9

#define PIN_AP 23

#define APERTADO LOW

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C
#define SCREEN_RESET -1 // Reset pin not used, set to -1
#define ROTATION 0 // No rotation

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RESET);

char nomeWifi[] = "Protheus Monitor WIFI";
char senhaWifi[] = "12345678";

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  Wire.begin(I2C_SDA, I2C_SCL);

  display.begin(SSD1306_SWITCHCAPVCC,SCREEN_ADDRESS);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Protheus");
  display.display();

}

// put your main code here, to run repeatedly:
void loop()
{
  delay(1000); // Delay for 1 second
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Hello, World!");
  display.display();
  
  
  delay(1000); // Delay for 1 second
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Teste de Display");
  display.display();
  

}
