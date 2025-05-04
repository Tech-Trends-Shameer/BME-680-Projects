//Tech Trends Shameer
//Temperature Monitor Using Whatsapp

#include <WiFi.h> 
#include <ThingESP.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme; // I2C


//ThingESP32 thing("username", "ProjectName", "DeviceCredentials");


ThingESP32 thing("username", "ProjectName", "DeviceCredentials");
 

void setup()
{
  Serial.begin(115200);

 // pinMode(LED, OUTPUT);

  thing.SetWiFi("WiFi Username", "WiFi Password");

  thing.initDevice();

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


String HandleResponse(String query)
{ 
  
  float temperature = bme.temperature; 
  float pressure = bme.pressure / 100.0;
  float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  float humidity = bme.humidity;
  float gas = bme.gas_resistance / 1000.0;
  //double dewPoint = dewPointFast(temperature, humidity);
  
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" *C");
 
  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" hPa");
 
  Serial.print("Humidity = ");
  Serial.print(humidity);
  Serial.println(" %");
  
 
  Serial.print("Approx. Altitude = ");
  Serial.print(altitude);
  Serial.println(" m");
 
  Serial.print("Gas = ");
  Serial.print(gas);
  Serial.println(" KOhms");
 
  String humid = "Humidity: " + String(humidity) + " % \n";
  String temp = "Temperature: " + String(temperature) + "°C \n";
  String p = "Pressure: " + String(pressure) + " hPa \n";
  String alt = "Altitude: " + String(altitude) + " m \n";
  String g = "Gas: " + String(gas) + " KOhms \n";

  
  if (query == "temperature") {
   
    return temp;
  }

  else if (query == "humidity") {
  
    return humid;
  }

    else if (query == "pressure") {
  
    return p;
  }

    else if (query == "altitude") {
  
    return alt;
  }

    else if (query == "gas") {
  
    return g;
  }
 
  else return "Your query was invalid..";

}


void loop()
{ 

  thing.Handle();

}
 
 
