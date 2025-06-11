#include <Arduino.h>
#include <Wire.h>             //I2C Communication
#include <Adafruit_GFX.h>     //Graphics lib for display
#include <Adafruit_SSD1306.h> //Monochrome OLED display library
#include <DHT.h>

#define SCREEN_WIDTH 128 // OLED width, in pixels
#define SCREEN_HEIGHT 64 // OLED height, in pixels
#define DHT11PIN 4
#define DHT22PIN 13

#define greenLed 11
#define redLed 12

int counter = 0;

DHT dht11(DHT11PIN, DHT11); // Objects for both sensors
DHT dht22(DHT22PIN, DHT22);

// OLED reset pin (set to -1 if sharing Arduino reset pin)
#define OLED_RESET -1                                                     // Resets display if Arduino gets reset
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // Declares display object

void setup()
{
    Serial.begin(9600);
    dht11.begin(); // Initialize sensors
    dht22.begin();
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    { // 0x3C is common address, can be 0x3D sometimes
        Serial.println(F("SSD1306 allocation failed"));
    }

    pinMode(greenLed, OUTPUT);
    pinMode(redLed, OUTPUT);

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Nistkasten Temp");
    display.println("Startet...");
    display.display(); // Writing frame buffer to display
    delay(2000);
    display.setTextSize(1);
}

void loop()
{
    float humidity_DHT22 = dht22.readHumidity();
    float temperature_DHT22 = dht22.readTemperature(); // Celsius

    for (int i = 0; i < 2; i++)
    {
        float humidity_DHT11 = dht11.readHumidity();
        float temperature_DHT11 = dht11.readTemperature(); // Celsius

        float deltaTemp = abs(temperature_DHT22 - temperature_DHT11);

        display.clearDisplay();
        display.setCursor(0, 0);

        display.println("Innen-");
        display.print("temperatur:   ");
        display.print(temperature_DHT22);
        display.println(" C");
        /*
        display.print("Feuchtigkeit: ");
        display.print(humidity_DHT22);
        display.println(" % \n");
        */

        display.println("Aussen-");
        display.print("temperatur:   ");
        display.print(temperature_DHT11);
        display.println(" C \n");
        /*
        display.print("Feuchtigkeit: ");
        display.print(humidity_DHT11);
        display.println(" %");
        */

        display.println("Delta Temperatur:");
        display.print(deltaTemp);
        display.println("C");

        display.display();

        if ((temperature_DHT22 - temperature_DHT11) > 0.2)
        {
            counter++;
        }
        else
        {
            counter = 0;
        }

        if (counter >= 60)
        {
            digitalWrite(greenLed, 1);
            digitalWrite(redLed, 0);
        }
        else
        { // falls einmal gemessen wurde, dass die Innentemperatur kleiner ist, dann braucht es wieder eine ganze Minute, bis die grüne LED wieder angeht
            digitalWrite(redLed, 1);
            digitalWrite(greenLed, 0);
        }

        delay(1000);
    }
}
