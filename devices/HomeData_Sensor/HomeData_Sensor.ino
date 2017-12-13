#include <HomeData.h>

#define IS_HOMEDATA_REPEATER  (1)
#define HAS_DHT               (1)
#define MY_NODE_ID        GARAGE

#include <Adafruit_Sensor.h>

#if HAS_DHT

  #include <DHT.h>
  #include <DHT_U.h>

  // DHT Sensor
  #define DHTPIN 5
  #define DHTTYPE DHT22

#endif

// Radio
//#define MY_DEBUG
//#define MY_DEBUG_VERBOSE_RF24
#define MY_RADIO_NRF24

// Physical Hookup
#define MY_RF24_CE_PIN    9
#define MY_RF24_CS_PIN    10

// 2.410-2.411 GHz
#define MY_RF24_CHANNEL   10

#if IS_HOMEDATA_REPEATER
  #define MY_REPEATER_FEATURE
  #define MY_RF24_PA_LEVEL  RF24_PA_HIGH
#else
  #define MY_RF24_PA_LEVEL  RF24_PA_LOW
#endif

#define MY_RF24_DATARATE  RF24_250KBPS

bool metric = true;

#include <MySensors.h>

#if HAS_DHT
  DHT_Unified dht(DHTPIN, DHTTYPE);

  MyMessage tempMsg(TEMPERATURE_REPORT, V_TEMP);
  MyMessage humMsg(HUMIDITY_REPORT, V_HUM);
#endif

void presentation()
{
  sendSketchInfo("Garage", "1.0");

#if HAS_DHT
  present(TEMPERATURE_REPORT, S_TEMP);
  present(HUMIDITY_REPORT, S_HUM);
#endif

}

void setup() {

#if HAS_DHT
  dht.begin();
#endif

  Serial.println("Starting up.");
}

void loop() {

#if HAS_DHT
  // Read DHT22
  sensors_event_t tempSense, humSense;
  
  dht.temperature().getEvent(&tempSense);
  dht.humidity().getEvent(&humSense);

  Serial.print("Read: temp ");
  Serial.print(tempSense.temperature);
  Serial.print("degrees C, humidity ");
  Serial.print(humSense.relative_humidity);
  Serial.println("%");
  
  tempMsg.set(tempSense.temperature, 1);
  humMsg.set(humSense.relative_humidity, 1);

  wait(100);
  send(tempMsg, true);
  wait(1000);
  send(humMsg, true);

#endif

  wait(30000);
}
