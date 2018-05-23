#include <DHT.h>

// Configuration
const int CONFIG_PIN_TO_DHT = 2;
const int CONFIG_DHT_TYPE = 11;

const int CONFIG_THRESHOLD_TEMPERATURE = 0;
const int CONFIG_THRESHOLD_HUMIDITY = 5;

// Modules
DHT dhtSensor(CONFIG_PIN_TO_DHT, CONFIG_DHT_TYPE);

// Variables
String currentState = "NONE";
float currentTemperature = 0;
float currentHumidity = 0;

void setup() {
  // Open console Serial port
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for connection.
  }

  // Debug info
  Serial.print("Sketch:   ");   Serial.println(__FILE__);
  Serial.print("Uploaded: ");   Serial.println(__DATE__);
  Serial.println(" ");

  // Open sensors
  dhtSensor.begin();

  Serial.println("Started !");
}

void loop() {
  bool isChange = false;

  float humidity = dhtSensor.readHumidity();
  float temperature = dhtSensor.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Check State changes
  if (currentState == "NONE") {
    isChange = true;
    currentState = "INIT";

  } else if (temperature - currentTemperature > CONFIG_THRESHOLD_TEMPERATURE) {
    isChange = true;
    currentState = "HEAT_UP";

  } else if (currentTemperature - temperature > CONFIG_THRESHOLD_TEMPERATURE) {
    isChange = true;
    currentState = "COOL_DOWN";

  } else if (humidity - currentHumidity > CONFIG_THRESHOLD_HUMIDITY) {
    isChange = true;
    currentState = "HUMIDITY_UP";

  } else if (currentHumidity - humidity > CONFIG_THRESHOLD_HUMIDITY) {
    isChange = true;
    currentState = "HUMIDITY_DOWN";
  }

  // Send changes
  if (isChange == true) {
    currentTemperature = temperature;
    currentHumidity = humidity;

    Serial.print((String)currentTemperature + " C, ");
    Serial.print((String)currentHumidity + " %");
    Serial.println(" (" + currentState + ") ");
  }

  delay(2000);
}
