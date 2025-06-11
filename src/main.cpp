#include <Arduino.h>
#include <Wire.h> //I2C Communication
#include <Adafruit_GFX.h> //Graphics lib for display
#include <Adafruit_SSD1306.h> //Monochrome OLED display library
#include <DHT.h>

#define SCREEN_WIDTH 128 // OLED width, in pixels
#define SCREEN_HEIGHT 64 // OLED height, in pixels
#define DHT11PIN 4
#define DHT22PIN 13

DHT dht11(DHT11PIN, DHT11); //Objects for both sensors
DHT dht22(DHT22PIN, DHT22);

// OLED reset pin (set to -1 if sharing Arduino reset pin)
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
    Serial.begin(9600);
    dht11.begin();
    dht22.begin();
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    { // 0x3C is common address, can be 0x3D sometimes
        Serial.println(F("SSD1306 allocation failed"));
    }
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Nistkasten Temp");
    display.println("Startet...");
    display.display();
    delay(2000);
    display.setTextSize(1);
}

void loop()
{
    float humidity_DHT22 = dht22.readHumidity();
    float temperature_DHT22 = dht22.readTemperature(); //   Celsius

    for (int i = 0; i < 2; i++)
    {
        float humidity_DHT11 = dht11.readHumidity();
        float temperature_DHT11 = dht11.readTemperature(); //   Celsius

        float deltaTemp = abs(temperature_DHT22 - temperature_DHT11);

        display.clearDisplay();
        display.setCursor(0, 0);

        display.println("DHT-22:");
        display.print("Temperatur:   ");
        display.print(temperature_DHT22);
        display.println(" C");
        /*
        display.print("Feuchtigkeit: ");
        display.print(humidity_DHT22);
        display.println(" % \n");
        */

        display.println("DHT-11:");
        display.print("Temperatur:   ");
        display.print(temperature_DHT11);
        display.println(" C \n");
        /*
        display.print("Feuchtigkeit: ");
        display.print(humidity_DHT11);
        display.println(" %");
        */

        display.println("Delta Temperatur :");
        display.print(deltaTemp);
        display.println("C");

        display.display();

        delay(500);
    }
}
