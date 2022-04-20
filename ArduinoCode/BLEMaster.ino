#include <ArduinoBLE.h> //BLE Library.

const int ledPin = LED_BUILTIN; // set ledPin to on-board LED


static BLEDevice slave;

void setup() {
  Serial.begin(9600); //sets the port for serial
  while(!Serial); //does not continue code until serial is established
  Serial.println("Serial Communication begining");

  pinMode(LED_BUILTIN, OUTPUT); //Built-in LED.
  digitalWrite(LED_BUILTIN, LOW);

    if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }
  
}

void loop() {
  
  BLE.scanForUuid("2384bddc-886f-11ec-a8a3-0242ac120000");
  

  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // ...

    Serial.println("Connecting ...");

    if (peripheral.connect()) {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("Connected");
    } else {
      Serial.println("Failed to connect!");
      return;
    }

    // discover peripheral attributes
    Serial.println("Discovering attributes ...");
    if (peripheral.discoverAttributes()) {
      Serial.println("Attributes discovered");
    } else {
      Serial.println("Attribute discovery failed!");
      peripheral.disconnect();
      return;
    }

    BLEService TenseSlave = peripheral.service("2384bddc-886f-11ec-a8a3-0242ac120000");

    if (TenseSlave) {
      // use the service
      Serial.println("tenseSlave service connected too");
      BLECharacteristic Movement = peripheral.characteristic("f1a0");
      int movementValue;

      if (Movement) {
        Serial.println("Movement found!");
        digitalWrite(ledPin, HIGH);
        Movement.read();
        while(movementValue == 1){
          delay(100);
          digitalWrite(ledPin, LOW);
          delay(100);
          digitalWrite(ledPin, HIGH);
        }
        delay(1000);
      } else {
        Serial.println("Movement Not found");
      }
    }    
  }

}
