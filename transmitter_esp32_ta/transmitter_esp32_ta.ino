#include "BluetoothSerial.h"
#include <DHT.h>

#define USE_NAME // Comment this to use MAC address instead of a slaveName
const char *pin = "1234"; // Change this to reflect the pin expected by the real slave BT device

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

#ifdef USE_NAME
String slaveName = "ESP32-BT-Slave"; // Change this to reflect the real name of your slave BT device
#else
String MACadd = "AA:BB:CC:11:22:33"; // This only for printing
uint8_t address[6] = {0xAA, 0xBB, 0xCC, 0x11, 0x22, 0x33}; // Change this to reflect the real MAC address of your slave BT device
#endif

String myName = "ESP32-BT-Master";

//DHT Setup
#define DHTPIN 21
#define LEDPIN 18  
#define LEDPIN1 2
#define DHTTYPE DHT22 // Tipe sensor DHT (DHT11 atau DHT22)
DHT dht(DHTPIN, DHTTYPE);

// Sensor gas MQ-2
int gasSensorPin = 34; // Misalnya sensor gas terhubung ke pin analog A0 (pin 34)

void setup() {
  pinMode(LEDPIN, OUTPUT);
  pinMode(LEDPIN1, OUTPUT);
 bool connected;
  Serial.begin(115200);
  SerialBT.begin(myName, true);
  Serial.printf("The device \"%s\" started in master mode, make sure slave BT device is on!\n", myName.c_str());

#ifndef USE_NAME
  SerialBT.setPin(pin);
  Serial.println("Using PIN");
#endif

  // connect(address) is fast (up to 10 secs max), connect(slaveName) is slow (up to 30 secs max) as it needs
  // to resolve slaveName to address first, but it allows to connect to different devices with the same name.
  // Set CoreDebugLevel to Info to view devices Bluetooth address and device names
#ifdef USE_NAME
  connected = SerialBT.connect(slaveName);
  Serial.printf("Connecting to slave BT device named \"%s\"\n", slaveName.c_str());
#else
  connected = SerialBT.connect(address);
  Serial.print("Connecting to slave BT device with MAC ");
  Serial.println(MACadd);
#endif

  if (connected)
  {
    Serial.println("Connected Successfully!");
  }
  else
  {
    while (!SerialBT.connected(10000))
    {
      Serial.println("Failed to connect. Make sure the remote device is available and in range, then restart the app.");
    }
  }

  // Initiate DHT sensor
  dht.begin();
}

void loop()
{
  // Read sensor data
  // float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Read gas sensor data
  int gasValue = analogRead(gasSensorPin);

  // Send sensor data via Bluetooth   
  // SerialBT.print("Humidity: ");
  // SerialBT.print(humidity);
  // SerialBT.print(", Temperature: ");
  SerialBT.print(temperature);
  SerialBT.print(",");
  //SerialBT.print(", Gas Value: ");
  SerialBT.print(gasValue);

  // Echo sensor data to Serial Monitor
  // Serial.print("Humidity: ");
  // // Serial.println(humidity);
  // // Serial.print("%, Temperature: ");
    if(temperature >= 27){
    digitalWrite(LEDPIN1, HIGH);
    }
    else if(temperature <= 27 ){
      digitalWrite(LEDPIN1, LOW);
}
   Serial.println(temperature);
   Serial.print(",");
  // Serial.print("C, Gas Value: ");
  Serial.println(gasValue);
  if(gasValue >= 1700){
    digitalWrite(LEDPIN, HIGH);
    }
    else if(gasValue <= 1700){
      digitalWrite(LEDPIN, LOW);
}
  // Serial.print(gasValue);
  // Serial.print(" "); // a space ' ' or  tab '\t' character is printed between the two values

  delay(2000); // Delay for 2 seconds before reading the sensor again
}