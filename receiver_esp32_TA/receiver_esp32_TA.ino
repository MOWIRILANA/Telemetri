#include "BluetoothSerial.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
int buzzer = 5;
#define LEDPIN 18

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char *pin = "1234"; // Change this to a more secure PIN.
String device_name = "ESP32-BT-Slave";

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LEDPIN, OUTPUT);
  pinMode(buzzer, OUTPUT);
  // Initialize the OLED display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  display.display();
  delay(2000);
  display.clearDisplay();
  
  SerialBT.begin(device_name); // Bluetooth device name
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
  
  #ifdef USE_PIN
    SerialBT.setPin(pin);
    Serial.println("Using PIN");
  #endif
}

void loop() {


  if (Serial.available()) {
    String receivedChar = Serial.readStringUntil('\n');
    // SerialBT.write(receivedChar);
    displayMessage(receivedChar);

  }
  
  if (SerialBT.available()) {
    String receivedChar = SerialBT.readStringUntil('\n');
    // Serial.write(receivedChar);
    displayMessage(receivedChar);
    Serial.println(receivedChar);

    //memisah data
    int separatorIndex = receivedChar.indexOf(',');

     if (separatorIndex != -1) {
        String data1 = receivedChar.substring(0, separatorIndex);
        String data2 = receivedChar.substring(separatorIndex + 1);

        // Konversi data menjadi integer
        int value1 = data1.toInt();
        int value2 = data2.toInt();


        if ((value1 != 0 || data1.startsWith("0")) && (value2 != 0 || data2.startsWith("0"))) {
            Serial.print("Received Integer Values: ");
            Serial.print("Data1: ");
            Serial.print(value1);
            Serial.print(", Data2: ");
            Serial.println(value2);
            

    // int receivedValue = receivedChar.toInt();
    // // Check if the conversion was successful
    // if (receivedValue != 0 || receivedChar.startsWith("0")) {
    //     // Successful conversion
    //     Serial.print("Received Integer Value: ");
    //     Serial.println(receivedValue);

     if(value1 >= 28){
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else if(value1 <= 28){
      digitalWrite(LED_BUILTIN, LOW);
    }
    if(value2 >= 1700){
      digitalWrite(LEDPIN, HIGH);
      alarm();
    }
    else if(value2 <= 1700){
      digitalWrite(LEDPIN, LOW);
    }
  }
  }
  }
  delay(20);
}

void alarm(){
  unsigned char i;
  // while (1)
  {

    for (i = 0; i <80; i++) 
    {
      digitalWrite (buzzer, HIGH) ;
      delay (1) ;
      digitalWrite (buzzer, LOW) ;
      delay (1) ;
    }

    for (i = 0; i <100; i++) 
    {
      digitalWrite (buzzer, HIGH) ;
      delay (2) ;
      digitalWrite (buzzer, LOW) ;
      delay (2) ;
    }
  }
}

void displayMessage(String receivedChar) {
  // Display the received character on the OLED
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
  display.print(receivedChar);
  display.display();
}