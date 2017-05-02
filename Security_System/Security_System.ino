/****************************************************************************
 * Read_Input Read a Simblee input from smart phone 
 * Mike Hord @ SparkFun Electronics 26 Jan 2016 
 * https://github.com/sparkfun/Simblee_Tutorials
 * 
 * This example demonstrates the use of the SimbleeForMobile library to read
 * the state of a pin on a Simblee module from a phone app. 
 * 
 * Resources: Please install the Simblee support files before attempting to use
 * this sketch; see
 * https://learn.sparkfun.com/tutorials/simblee-concepts#setting-up-arduino for
 * details.
 * 
 * Development environment specifics: Arduino.cc IDE v1.6.7
 * 
 * This code is beerware; if you see me (or any other SparkFun employee) at the
 * local, and you've found our code helpful, please buy us a round!
 * ****************************************************************************/

// To use the SimbleeForMobile library, you must include this file at the top
// of your sketch. **DO NOT** include the SimbleeBLE.h file, as it will cause
// the library to silently break.
#include <SimbleeForMobile.h>
#include <Wire.h>

#define analogPin 11

#define diginput 12

const int led = 2; // The Simblee BOB (WRL-13632) has an LED on pin 2.
int ledState = LOW;

uint8_t btnID;
uint8_t switchID;
uint8_t textID;

const int btn = 9; // The Simblee BOB (WRL-13632) has a button on pin 3.

// Every draw command returns a uint8_t result which is the object id that was
// created. If you wish to change the object later, you'll need this value, and
// if you want to catch an event created by an object, you'll need it there,
// too. Make sure you create these id variables outside of any function, as
// you'll need to refer to them in many other functions.

uint8_t boxID;

double frequency;

char buf[9];
int counter;

void setup() 
{
  pinMode(diginput, INPUT);
  counter = 0;

  Wire.beginOnPins(12, 15);

  pinMode(led, OUTPUT);
  pinMode(analogPin, INPUT);
  digitalWrite(led, ledState);
  
  // Protip: using INPUT_PULLUP very rarely causes any problems but can solve
  // a lot of problems with input signals that aren't pulled strongly.
  pinMode(btn, INPUT_PULLUP);
  
  // advertisementData shows up in the app as a line under deviceName. Note
  // that the length of these two fields combined must be less than 16
  // characters!
  SimbleeForMobile.deviceName = "Meme";
  SimbleeForMobile.advertisementData = "Security";

  // txPowerLevel can be any multiple of 4 between -20 and +4, inclusive. The
  // default value is +4; at -20 range is only a few feet.
  SimbleeForMobile.txPowerLevel = -4;

  // This must be called *after* you've set up the variables above, as those
  // variables are only written during this function and changing them later
  // won't actually propagate the settings to the device.
  SimbleeForMobile.begin();
  Serial.begin(9600);
  Serial.println(btn);
//  buf[2] = '.';
//  buf[5] = 0;
}

void loop() 
{
  bool laser_alert = digitalRead(diginput);
  Serial.println(laser_alert);
  double onTime = pulseIn(analogPin, HIGH);
  frequency = onTime / 32.;
  frequency = 1.58231*frequency + 1.20509;
  int freq = frequency;
  int frac = ((int) (frequency * 100)) % 100;
  Serial.println(sprintf(buf, "%d.%02d oC", freq, frac));
  buf[6] = 176;
  //Serial.print("Frequency: ");
  //Serial.println(frequency);
  //Serial.print("buf: ");
  //Serial.println(buf);
  //Serial.println(1.58231*frequency + 1.20509);
  // All we want to do is detect when the button is pressed and make the box on
  // the screen white while it's pressed.

  // This is important: before writing *any* UI element, make sure that the UI
  // is updatable!!! Failure to do so may crash your whole program.
  if (SimbleeForMobile.updatable)
  {
    // Okay, *now* we can worry about what the button is doing. The
    // updateColor() function takes the id returned when we created the box and
    // tells that object to change to the color parameter passed.
    if (digitalRead(btn) && !laser_alert) SimbleeForMobile.updateColor(boxID, BLACK);
    else { Serial.println("angery"); SimbleeForMobile.updateColor(boxID, RED); }
    
    counter++;
    if (counter > 25) {
      SimbleeForMobile.updateText(textID, buf);
      counter = 0;
    }
    
    
  }
  else { Serial.println("SAD!"); }
  // This function must be called regularly to process UI events.
  SimbleeForMobile.process();
  
  //delay(1000);
  //fflush(buf);
}

// (15.55, 25.81), (19.62, 32.25)

// ui() is a SimbleeForMobile specific function which handles the specification
// of the GUI on the mobile device the Simblee connects to.
void ui()
{
  // color_t is a special type which contains red, green, blue, and alpha 
  // (transparency) information packed into a 32-bit value. The functions rgb()
  // and rgba() can be used to create a packed value.
  color_t darkgray = rgb(85,85,85);

  // These variable names are long...let's shorten them. They allow us to make
  // an interface that scales and scoots appropriately regardless of the screen
  // orientation or resolution.
  uint16_t wid = SimbleeForMobile.screenWidth;
  uint16_t hgt = SimbleeForMobile.screenHeight;

  // The beginScreen() function both sets the background color and serves as a
  // notification that the host should try to cache the UI functions which come
  // between this call and the subsequent endScreen() call.
  SimbleeForMobile.beginScreen(darkgray);

  // SimbleeForMobile doesn't really have an kind of indicator- but there IS a
  // drawRect() function, and we can freely change the color of the rectangle
  // after drawing it! The x,y coordinates are of the upper left hand corner.
  // If you pass a second color parameter, you'll get a fade from top to bottom
  // and you'll need to update *both* colors to get the whole box to change.
  boxID = SimbleeForMobile.drawRect(0, 0, wid, hgt, BLACK);
//  boxID = SimbleeForMobile.drawRect(
//                          (wid/2) - 50,        // x position
//                          (hgt/2) + 75,        // y positon
//                          1000,                 // x dimension
//                          1000,                 // y dimensionrectangle
//                          BLACK);              // color of rectangle.

  // Create a button slightly more than halfway down the screen, 100 pixels
  // wide, in the middle of the screen. The last two parameters are optional;
  // see the tutorial for more information about choices for them. The BOX_TYPE
  // button has a bounding box which is roughly 38 pixels high by whatever the
  // third parameter defines as the width.
  btnID = SimbleeForMobile.drawButton(
                              (wid/2) - 75,          // x location
                              (hgt/2) - 22 + 150,          // y location
                              150,                   // width of button
                              "Reverse LED",         // text shown on button
                              WHITE,                 // color of button
                              BOX_TYPE);             // type of button

  // Buttons, by default, produce only EVENT_PRESS type events. We want to also
  // do something when the user releases the button, so we need to invoke the
  // setEvents() function. Note that, even though EVENT_PRESS is default, we
  // need to include it in setEvents() to avoid accidentally disabling it.
  SimbleeForMobile.setEvents(btnID, EVENT_PRESS | EVENT_RELEASE);

  // Create a switch above the button. Note the lack of a title option; if you
  // want to label a switch, you'll need to create a textBox object separately.
  // A switch's bounding box is roughly 50 by 30 pixels.
  switchID = SimbleeForMobile.drawSwitch(
                              (wid/2) - 25,          // x location
                              (hgt/2)+22 + 150,            // y location
                              BLUE);                 // color (optional)

  textID = SimbleeForMobile.drawText(wid/2-65, hgt/2-22, buf, WHITE, 45);
  //textID = SimbleeForMobile.drawText(wid/2-45, hgt/2-250, 1.58231*frequency + 1.20509, WHITE, 45);
                          
  SimbleeForMobile.endScreen();
}

// This function is called whenever a UI event occurs. Events are fairly easy
// to predict; for instance, touching a button produces a "PRESS_EVENT" event.
// UI elements have default event generation settings that match their expected
// behavior, so you'll only rarely have to change them.
void ui_event(event_t &event)
{
  // We created the btnID and switchID variables as globals, set them in the
  // ui() function, and we'll use them here.

  
  if (event.id == btnID)
  {
    if (event.type == EVENT_PRESS)
    {
      if (ledState == HIGH) digitalWrite(led, LOW);
      else digitalWrite(led, HIGH);
    }    
    if (event.type == EVENT_RELEASE)
    {
      if (ledState == HIGH) digitalWrite(led, HIGH);
      else digitalWrite(led, LOW);
    }
  }

  // If the event was a switch press, we want to toggle the ledState variable
  // and then write it to the pin.
  if (event.id == switchID)
  {
    if (ledState == HIGH) ledState = LOW;
    else ledState = HIGH;
    digitalWrite(led, ledState);
  }
}


