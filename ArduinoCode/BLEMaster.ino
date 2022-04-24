#include <ArduinoBLE.h>

const int ledPin = LED_BUILTIN; // set ledPin to on-board LED

BLEService TenseArduino1("2384bddc-886f-11ec-a8a3-0242ac120000"); // create service

// create switch characteristic and allow remote device to read and write
BLEByteCharacteristic LedFlash("2384bddc-886f-11ec-a8a3-0242ac120001", BLERead | BLEWrite);
BLEIntCharacteristic Movement("f1ao", BLERead | BLEBroadcast);


void setup() {
  Serial.begin(9600); //sets the port for serial
  while(!Serial); //does not continue code until serial is established
  Serial.println("Serial Communication begining");

  
  pinMode(ledPin, OUTPUT); // use the LED as an output

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // set the local name peripheral advertises
  BLE.setLocalName("Tensegrity Arduino One");
  // set the UUID for the service this peripheral advertises:
  
  BLE.setAdvertisedService(TenseArduino1);

  // add the characteristics to the service
  TenseArduino1.addCharacteristic(LedFlash);
  TenseArduino1.addCharacteristic(Movement);

  // add the service
  BLE.addService(TenseArduino1);

  LedFlash.writeValue(0);

  // start advertising
  BLE.advertise();
  Serial.println(TenseArduino1.uuid());
  Serial.println(LedFlash.uuid());
  Serial.println(Movement.uuid());
}

void loop() {
  // poll for BLE events
  while (BLE.connected()) {
    Serial.println("Device Connected");
    delay(1000);
    if (LedFlash.value() == 22) {
      Serial.println("Value is 22");
      
    }
  }
  Serial.println("No Device Connected");
  LedFlash.writeValue(0);
  delay(1000);
  
}