//Global Constants
//const int ledPin = 2;
const int stepPinA = 3;
const int dirPinA = 2;
const int stepPinB = 5;
const int dirPinB = 4;
const int stepDelay = 1500; // Delay (microseconds) of the motor between steps.
const int stepCounter = 50; // # of time the move commands loop 
int stopValueA = 1000; // sets max value of stepper motor, min is set to 0.
int stopValueB = 1000; // sets max value of stepper motor, min is set to 0.
int oscilationCounter;

int counterA; // global variables
int counterB;

#include <ArduinoBLE.h> 
#include <Arduino_LSM6DS3.h> 
#include <Arduino.h>

BLEService tenseRobo("0fcf5793-df75-475c-be63-62affc3fbbf9"); //GUID for identifiying this robot
BLEByteCharacteristic motorAPosition("aaf8", BLERead | BLENotify); //UUID for monitering
BLEByteCharacteristic motorAControl("aaf7", BLERead | BLENotify | BLEWrite); //UUID for Writing to A
BLEByteCharacteristic motorBPosition("bbf8", BLERead | BLENotify); //UUID for monitering
BLEByteCharacteristic motorBControl("bbf7", BLERead | BLENotify | BLEWrite); //UUID for Writing to B

 
void setup() {
  oscilationCounter = 0;
  pinMode(stepPinA, OUTPUT);
  pinMode(dirPinA, OUTPUT);
  pinMode(stepPinB, OUTPUT);
  pinMode(dirPinB, OUTPUT);
  counterA = (stopValueA / 2); //initilizes counter variable: sets to 500 becuase that is the middle of the spectrum
  counterB = (stopValueB / 2);
  
  /*
   * Does not use //Serial becuase it is not connected to a computer. If //Serial is desired, use //SerialTenseRobot
   * 
   * //Serial.begin(9600); //sets the port for //Serial
   *while(!Serial); //does not continue code until //Serial is established
   *Serial.println("Serial Communication begining");
   */
  
  if (!BLE.begin()) {
    while (1); //stop program becuase it won't work
  } //notifies via //Serial if BLE is not working
  
  BLE.setLocalName("TensegrityRobot"); //sets name for device
  BLE.setAdvertisedService(tenseRobo);
  tenseRobo.addCharacteristic(motorAPosition); //adds  to controlLed
  tenseRobo.addCharacteristic(motorAControl); //adds  to controlLed
  tenseRobo.addCharacteristic(motorBPosition); //adds  to controlLed
  tenseRobo.addCharacteristic(motorBControl); //adds  to controlLed
  BLE.addService(tenseRobo); //adds  to TensegrityRobot
  motorAPosition.writeValue(counterA); //Sets initial value to counter
  motorAControl.writeValue(0); //Sets initial value (0 = off)
  motorBPosition.writeValue(counterA); //Sets initial value to counter
  motorBControl.writeValue(0); //Sets initial value (0 = off)
  BLE.advertise(); //advertises TenseRobo 

}

void loop() {
BLEDevice central = BLE.central(); //Sets a boolean "central" to notify if connection has occured
  if (central) { // central = device connected
    digitalWrite(LED_BUILTIN, HIGH); // turns built in LED on
    
    while(central.connected()) {
      motorAPosition.writeValue(counterA); //displays counter on an external variable
      motorBPosition.writeValue(counterB); //displays counter on an external variable
      
      //This if tree tells the motor what to do
      if (motorAControl.value() == 0) {
        int oscilationCounter = 0;//resets the counter after a different command is executed
        }
      if (motorAControl.value() != 0) {
        //default and debug statuses
        if(motorAControl.value() == 1) { //default response
          //Serial.println("Previous command executed. Send another.");
          motorAControl.writeValue(0);
        }else if (motorAControl.value() == 2) { //counter has reached stopValue
          //Serial.println("The motor has reached its maximum value, try a command that moves it down.");
          //Serial.print("This maximum value is set by globally defined integer 'stopValue', which is currently: ");
          //Serial.println(stopValue);
          motorAControl.writeValue(0);
        }else if (motorAControl.value() == 3) { //counter has reached 0
          //Serial.println("The motor has reached its minimum value, 0, try a command that moves it up.");
          motorAControl.writeValue(0);
        }
        //these cases move counter up
        else if (motorAControl.value() == 16 ) { //Hex Value 10
          if (counterA < stopValueA) {
            moveAUpOne();
          } else {
            motorAControl.writeValue(2);
          }
        }else if (motorAControl.value() == 17 ) { //Hex Value 11
          if (counterA < stopValueA) {
            moveAUpOne();
            motorAControl.writeValue(1);
          } else {
            motorAControl.writeValue(2);
          }
        }else if (motorAControl.value() == 21 ) { //Hex Value 15
          if (counterA < stopValueA) {
            for (int i = 0; i<5; i++) { 
              moveAUpOne();
            }
            motorAControl.writeValue(1);
          } else {
            motorAControl.writeValue(2);
          }
        }
        //these cases move counter down
        else if (motorAControl.value() == 32 ) { //Hex Value 20
          if (counterA > 0) {
            moveADownOne();
          } else {
            motorAControl.writeValue(3);
          }
        }else if (motorAControl.value() == 33 ) { //Hex Value 21
          if (counterA > 0) {
            moveADownOne();
            motorAControl.writeValue(1);
          } else {
            motorAControl.writeValue(3);
          }
        }else if (motorAControl.value() == 37 ) { //Hex Value 25
          if (counterA > 0) {
            for (int i = 0; i<5; i++) { 
              moveADownOne();
            }
            motorAControl.writeValue(1);
          } else {
            motorAControl.writeValue(3);
          }
        }
          
//---------------------------------------------------------------------------------------------------------------------------------------
        
        //These cases are used to zero in the counter variable and position

        //This case is when it has reached the end on the down direction (Hex Values in the 20s), sets the counter to 0 so it cannot go further
        else if(motorAControl.value() == 49 ){ //Hex Value 31
          counterA = 0; 
        }
        //This case sets the maximum value of counter to be what the counter currently is. This is the other endpoint
        else if(motorAControl.value() == 50 ){ //Hex Value 32
          stopValueA = counterA;
        }
        //Resets the counter to the middle value if messed up
        else if(motorAControl.value() == 51 ){ //Hex Value 33
          counterA = (stopValueA /2); 
        }
        //This case sets the maximum value of counter to 1000, serves as reset of Hex value 32.
        else if(motorAControl.value() == 52 ){ //Hex Value 34
          stopValueA = 1000;
        }
        //These cases are the movement commands for oscilations

        //Oscilate motor A once
        else if (motorAControl.value() == 53 ) { //Hex Value 35
          if (counterA == stopValueA) {
            oscilationCounter = 1;
          }
          else if (counterA == 0){
           oscilationCounter = 0;
          }
          
          if ((counterA < stopValueA) && (oscilationCounter == 0)) {
            while ( counterA < stopValueA ) {
              moveAUpOne(); 
            }
            while (counterA > 0){
              moveADownOne();
            }
          }
          else if ((counterA > 0)&&(oscilationCounter == 1)) {
            while(counterA > 0){
              moveADownOne();
            }
            while (counterA < stopValueA) {
              moveAUpOne(); 
            }
          }
           motorAControl.writeValue(1); 
        }

        //oscilate motor A until another command is input. 
        else if (motorAControl.value() == 55 ) { //Hex Value 37
          if (counterA == stopValueA && counterB == stopValueB) {
            oscilationCounter = 1;
          }
          else if (counterA == 0 && counterB ==0 ){
           oscilationCounter = 0;
          }
          
          if ((counterA < stopValueA) && (oscilationCounter == 0)) {
            moveAUpOne(); 
          }
          else if ((counterB < stopValueB) && (oscilationCounter == 0)){
            moveBUpOne();
          }
          
          else if ((counterA > 0)&&(oscilationCounter == 1)) {
            moveADownOne();
          }
          else if ((counterB > 0)&&(oscilationCounter == 1)) {
            moveBDownOne();
          }  
        }

        //oscilate both motors until another command is input 
        //order is A up, B up, A down, B Down
        else if (motorAControl.value() == 56 ) { //Hex Value 38
          if (counterA == stopValueA) {
            oscilationCounter = 1;
          }
          else if (counterA == 0){
           oscilationCounter = 0;
          }
          if ((counterA < stopValueA) && (oscilationCounter == 0)) {
            moveAUpOne(); 
          }
          else if ((counterA > 0)&&(oscilationCounter == 1)) {
            moveADownOne();
          } 
        }
        
        //final catch of cases that don't work
        else {
          //Serial.print("Written Value of ");
          //Serial.print(motorAControl.value());
          //Serial.println(" does not match any commands, try another value."); 
          motorAControl.writeValue(0);
        }
       }//End of motor A's Loop


//----------------------------------------------------------------------------------------------------------------------------------------------------------

        if (motorBControl.value() != 0) {
        //default and debug statuses
        if(motorBControl.value() == 1) { //default response
          //Serial.println("Previous command executed. Send another.");
          motorBControl.writeValue(0);
        }else if (motorBControl.value() == 2) { //counter has reached stopValue
          //Serial.println("The motor has reached its maximum value, try a command that moves it down.");
          //Serial.print("This maximum value is set by globally defined integer 'stopValue', which is currently: ");
          //Serial.println(stopValue);
          motorBControl.writeValue(0);
        }else if (motorBControl.value() == 3) { //counter has reached 0
          //Serial.println("The motor has reached its minimum value, 0, try a command that moves it up.");
          motorBControl.writeValue(0);
        }
        //these cases move counter up
        else if (motorBControl.value() == 16 ) { //Hex Value 10
          if (counterB < stopValueB) {
            moveBUpOne();
          } else {
            motorBControl.writeValue(2);
          }
        }else if (motorBControl.value() == 17 ) { //Hex Value 11
          if (counterB < stopValueB) {
            moveBUpOne();
            motorBControl.writeValue(1);
          } else {
            motorBControl.writeValue(2);
          }
        }else if (motorBControl.value() == 21 ) { //Hex Value 15
          if (counterB < stopValueB) {
            for (int i = 0; i<5; i++) { 
              moveBUpOne();
            }
            motorBControl.writeValue(1);
          } else {
            motorBControl.writeValue(2);
          }
        }
        //these cases move counter down
        else if (motorBControl.value() == 32 ) { //Hex Value 20
          if (counterB > 0) {
            moveBDownOne();
          } else {
            motorBControl.writeValue(3);
          }
        }else if (motorBControl.value() == 33 ) { //Hex Value 21
          if (counterB > 0) {
            moveBDownOne();
            motorBControl.writeValue(1);
          } else {
            motorBControl.writeValue(3);
          }
        }else if (motorBControl.value() == 37 ) { //Hex Value 25
          if (counterB > 0) {
            for (int i = 0; i<5; i++) { 
              moveBDownOne();
            }
            motorBControl.writeValue(1);
          } else {
            motorBControl.writeValue(3);
          }
        }

        else if (motorBControl.value() == 53 ) { //Hex Value 35
          while(counterB < stopValueB){
            moveBUpOne();
          }
          while(counterB > 0){
            moveBDownOne();
          }
          motorBControl.writeValue(1);
        }
        else if (motorBControl.value() == 54 ) { //Hex Value 36
          while (motorBControl.value() == 54){
            while(counterB < stopValueB){
              if(motorBControl.value() != 54){
                break;
              }
              moveBUpOne();
            }
            while(counterB > 0){
              if(motorBControl.value() != 54){
                break;
              }
              moveBDownOne();
            } 
          }

          motorBControl.writeValue(1);
        }
        //These cases are used to zero in the counter variable and position

        //This case is when it has reached the end on the down direction (Hex Values in the 20s), sets the counter to 0 so it cannot go further
        else if(motorBControl.value() == 49 ){ //Hex Value 31
          counterB = 0; 
        }
        //This case sets the maximum value of counter to be what the counter currently is. This is the other endpoint
        else if(motorBControl.value() == 50 ){ //Hex Value 32
          stopValueB = counterB;
        }
        //Resets the counter to the middle value if messed up
        else if(motorBControl.value() == 51 ){ //Hex Value 33
          counterB = (stopValueB /2); 
        }
        //This case sets the maximum value of counter to 1000, serves as reset of Hex value 32.
        else if(motorBControl.value() == 52 ){ //Hex Value 34
          stopValueB = 1000;
        }
        //final catch of cases that don't work
        else {
          //Serial.print("Written Value of ");
          //Serial.print(motorBControl.value());
          //Serial.println(" does not match any commands, try another value."); 
          motorBControl.writeValue(0);
        }//end of Motor B's Loop

        
      }// end of if(motorBControl.value() != 0) loop  
    }//end of while(central.connected) loop

    //this code occurs when device disconnects from BLE
    motorAPosition.writeValue(counterA);
    motorBPosition.writeValue(counterB);
    digitalWrite(LED_BUILTIN, LOW); //
  }
}

void moveAUpOne() { //moves the stepper motor up one counter
   digitalWrite(dirPinA, HIGH);
   digitalWrite(LED_BUILTIN, HIGH);
   for(int x = 0; x < stepCounter; x++){
    digitalWrite(stepPinA, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(stepPinA, LOW);
    delayMicroseconds(stepDelay);
  }
  digitalWrite(LED_BUILTIN, LOW);
  counterA++;
}

void moveADownOne() { //moves the stepper motor up one counter
   digitalWrite(dirPinA, LOW);
   digitalWrite(LED_BUILTIN, HIGH);
   for(int x = 0; x < stepCounter; x++){
    digitalWrite(stepPinA, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(stepPinA, LOW);
    delayMicroseconds(stepDelay);
  }
  digitalWrite(LED_BUILTIN, LOW);
  counterA--;
}

void moveBUpOne() { //moves the stepper motor up one counter
   digitalWrite(dirPinB, HIGH);
   digitalWrite(LED_BUILTIN, HIGH);
   for(int x = 0; x < stepCounter; x++){
    digitalWrite(stepPinB, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(stepPinB, LOW);
    delayMicroseconds(stepDelay);
  }
  digitalWrite(LED_BUILTIN, LOW);
  counterB++;
}

void moveBDownOne() { //moves the stepper motor up one counter
   digitalWrite(dirPinB, LOW);
   digitalWrite(LED_BUILTIN, HIGH);
   for(int x = 0; x < stepCounter; x++){
    digitalWrite(stepPinB, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(stepPinB, LOW);
    delayMicroseconds(stepDelay);
  }
  digitalWrite(LED_BUILTIN, LOW);
  counterB--;
}
