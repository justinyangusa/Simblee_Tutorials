

#include <Wire.h>
#include "Adafruit_MCP9808.h"

#define ANDOut   9
#define BlueLed  A2
#define GreenLed A1
#define LaserLED 12
#define digiOut1 10
#define digiOut2 11
#define Laser    A0
#define digiOut0 12
// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(digiOut1,OUTPUT);
  pinMode(digiOut2,OUTPUT);
  pinMode(ANDOut, INPUT);
  pinMode(digiOut0,OUTPUT);
  pinMode(LaserLED,OUTPUT);
    // Make sure the sensor is found, you can also pass in a different i2c
  // address with tempsensor.begin(0x19) for example
  if (!tempsensor.begin()) {
    Serial.println("Couldn't find MCP9808!");
    while (1);
  }
}

int BlueRead,GreenRead, Out, LaserRead;
int PWMsig;
void loop() {
  // put your main code here, to run repeatedly:
  //Temp Sensor
  {
    //Serial.println("wake up MCP9808.... "); // wake up MSP9808 - power consumption ~200 mikro Ampere
  //tempsensor.wake();   // wake up, ready to read!

  // Read and print out the temperature, then convert to *F
  float c = tempsensor.readTempC();
  float f = c * 9.0 / 5.0 + 32;
  //Serial.println("Temp: "); Serial.print(c); Serial.print("*C\t"); 
  //Serial.print(f); Serial.println("*F");
  
  PWMsig = (int)(c*10*(255.0/1000.0));
  analogWrite(3,PWMsig);
  //Serial.println(PWMsig);
  //Serial.println("Shutdown MCP9808.... ");
  //tempsensor.shutdown(); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere
  
  delay(10);
  }

  //Door IR Sensor Code
    {   BlueRead=analogRead(BlueLed);
       GreenRead=analogRead(GreenLed);
       delay(10);
       if(GreenRead>370){
          digitalWrite(digiOut1,HIGH);
       }
       else { digitalWrite(digiOut1, LOW);}
      // Serial.println(BlueRead);
       if (BlueRead>545){
          digitalWrite(digiOut2, HIGH);
       }
       else{
        digitalWrite(digiOut2, LOW);
       }
      Out = digitalRead(ANDOut);
      //  Serial.println(Out);
    }
  //Laser Code
    {
      LaserRead=analogRead(Laser);
      Serial.println(LaserRead);
      if(LaserRead>900){//should be 900
        //lights are on
        digitalWrite(digiOut0, LOW);
        digitalWrite(LaserLED, LOW);
      }
      else{
        digitalWrite(digiOut0, HIGH);
        digitalWrite(LaserLED, HIGH);
      }
    }





  
}
