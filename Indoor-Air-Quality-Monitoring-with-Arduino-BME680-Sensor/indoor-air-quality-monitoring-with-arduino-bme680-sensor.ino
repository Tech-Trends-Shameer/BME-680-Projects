#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <LiquidCrystal_I2C.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme; // I2C
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change address if necessary

void setup() 
{
  Serial.begin(9600);
  while (!Serial);
  Serial.println(F("BME680 Weather Station"));
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("Tech Trends");
  lcd.setCursor(5, 1);
  lcd.print("Shameer");
  delay(2000);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("BME680 Weather");
  lcd.setCursor(6, 1);
  lcd.print("Station");
  delay(2000);
  lcd.clear();

  if (!bme.begin()) 
  {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
}

void loop() 
{
  if (! bme.performReading()) 
  {
    Serial.println("Failed to perform reading :(");
    return;
  }
  
  float temperature = bme.temperature;
  float pressure = bme.pressure / 100.0;
  float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  float humidity = bme.humidity;
  float gas = bme.gas_resistance / 1000.0;
  double dewPoint = dewPointFast(temperature, humidity);
  
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" *C");
 
  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" hPa");
 
  Serial.print("Humidity = ");
  Serial.print(humidity);
  Serial.println(" %");
 
  Serial.print("Dew Point = ");
  Serial.print(dewPoint);
  Serial.println(" *C");
 
  Serial.print("Approx. Altitude = ");
  Serial.print(altitude);
  Serial.println(" m");
 
  Serial.print("Gas = ");
  Serial.print(gas);
  Serial.println(" KOhms");

  // Display values on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(temperature);
  lcd.print(" C");
  
  lcd.setCursor(0, 1);
  lcd.print("Hum:");
  lcd.print(humidity);
  lcd.print(" %");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pres:");
  lcd.print(pressure);
  lcd.print(" hPa");
  
  lcd.setCursor(0, 1);
  lcd.print("Alt:");
  lcd.print(altitude);
  lcd.print(" m");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gas:");
  lcd.print(gas);
  lcd.print(" KOhm");
  
  lcd.setCursor(0, 1);
  lcd.print("Dew:");
  lcd.print(dewPoint);
  lcd.print(" C");
  delay(2000);
  
  Serial.println();
  delay(2000);
}
 
double dewPointFast(double celsius, double humidity)
{
  double a = 17.271;
  double b = 237.7;
  double temp = (a * celsius) / (b + celsius) + log(humidity * 0.01);
  double Td = (b * temp) / (a - temp);
  return Td;
}
