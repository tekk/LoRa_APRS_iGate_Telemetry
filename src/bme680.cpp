#include "bme680.h"

bool setup_bme_680() {
  if (!bme.begin()) {
    Serial.println(F("Could not find a valid BME680 sensor, check wiring!"));
    return false;
  }

  Serial.println(F("BME680 sensor found."));

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms

  return true;
}

// we can call it only once per 2 seconds max.!
BME680_data read_bme680() {

  BME680_data data = {0, 0, 0, 0, 0};

  // Tell BME680 to begin measurement.
  unsigned long endTime = bme.beginReading();
  if (endTime == 0) {
    Serial.println(F("Failed to begin reading :("));
    return data;
  }

  yield();

  if (!bme.endReading()) {
    Serial.println(F("Failed to complete reading :("));
    return data;
  }

  Serial.println("------------------------------------");
  Serial.print(F("Reading completed at "));
  Serial.println(millis());

  Serial.print(F("Temperature = "));
  Serial.print(bme.temperature);
  Serial.println(F(" *C"));
  data.temperature = bme.temperature;

  Serial.print(F("Pressure = "));
  Serial.print(bme.pressure / 100.0);
  Serial.println(F(" hPa"));
  data.pressure = bme.pressure / 100.0;

  Serial.print(F("Humidity = "));
  Serial.print(bme.humidity);
  Serial.println(F(" %"));
  data.humidity = bme.humidity;

  Serial.print(F("Gas = "));
  Serial.print(bme.gas_resistance / 1000.0);
  Serial.println(F(" KOhms"));
  data.gas = bme.gas_resistance / 1000.0;

  float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  Serial.print(F("Approx. Altitude = "));
  Serial.print(altitude);
  Serial.println(F(" m"));
  data.altitude = altitude;

  Serial.println();
  return data;
}