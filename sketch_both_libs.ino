#include <Wire.h>
#include <SparkFun_Qwiic_Humidity_AHT20.h>
#include <SparkFun_SGP40_Arduino_Library.h>
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"


AHT20 ahtSensor;
SGP40 sgpSensor;
ArduinoLEDMatrix matrix;

void setup() {
  Serial.begin(9600);
  matrix.begin();
  Wire1.begin();  //Join I2C bus
  //Check if the AHT20 will acknowledge
  if (ahtSensor.begin(Wire1) == false) {
    Serial.println("AHT20 not detected. Check connections. Freezing.");
    while (1)
      ;
  }
  Serial.println("AHT20 acknowledged.");

  if (sgpSensor.begin(Wire1) == false) {
    Serial.println(F("SGP40 not detected. Check connections. Freezing..."));
    while (1)
      ;
  }
  Serial.println("SGP40 acknowledged.");
}

void loop() {
  //If a new measurement is available
  if (ahtSensor.available() == true) {
    //Get the new temperature and humidity value
    float temperature = ahtSensor.getTemperature();
    float humidity = ahtSensor.getHumidity();
    int voc = sgpSensor.getVOCindex(humidity, temperature);

    //Print the results
    Serial.print("Temperature: ");
    Serial.print(temperature, 2);
    Serial.print(" C\t");
    Serial.print("Humidity: ");
    Serial.print(humidity, 2);
    Serial.print("% RH\t");
    Serial.print("VOC Index is: ");
    Serial.println(voc);

    Serial.println();

    // Make it scroll!
    matrix.beginDraw();

    matrix.stroke(0xFFFFFFFF);
    matrix.textScrollSpeed(50);

    // add the text
    const String text = "   Temp: " + String(temperature) + " Humidity: " + String(humidity) + " VOC: " + String(voc) + "   ";
    matrix.textFont(Font_5x7);
    matrix.beginText(0, 1, 0xFFFFFF);
    matrix.println(text);
    matrix.endText(SCROLL_LEFT);

    matrix.endDraw();
  }

  //The AHT20 can respond with a reading every ~50ms. However, increased read time can cause the IC to heat around 1.0C above ambient.
  //The datasheet recommends reading every 2 seconds.
  delay(2000);
}