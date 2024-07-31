#include <Joystick.h>

// Constants: Pin numbers for buttons and other inputs
const int heaterPin = 2;
const int fanPin = 4;
const int switchPin = 7;
const int potPin = 6;  

bool heater = false; 
bool fan = false; 
bool switchState = false;
int potValue = 0;

const int unknown = -1;
const int bake = 0;
const int broil = 1;
const int toast = 2;
const int airfry = 3;
int previousState = unknown;

// Create Joystick instance
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD, 5, 0, true, true, false, false, false, false, false, false, false, false, false);

void setup() {
  Serial.begin(9600);

  // Initialize pushbutton pins as inputs with internal pull-up resistors
  pinMode(heaterPin, INPUT_PULLUP);
  pinMode(fanPin, INPUT_PULLUP);
  pinMode(switchPin, INPUT_PULLUP);  // Initialize switch pin as input with internal pull-up resistor
  pinMode(potPin, INPUT);            // Initialize potentiometer pin as input

  // Initialize Joystick library
  Joystick.begin();
}

void loop() {
  // Read the state of the pushbuttons
  heater = digitalRead(heaterPin) == LOW;
  fan = digitalRead(fanPin) == LOW;
  switchState = digitalRead(switchPin) == LOW;  // Read the state of the switch
  potValue = analogRead(potPin);  // Read the value of the potentiometer
  // Determine the current state based on button states
  int currentState = unknown;

  if (!heater && !fan) {
    currentState = broil;
  } else if (heater && fan) {
    currentState = airfry;
  } else if (previousState == toast) {
    currentState = toast;
  } else if (previousState == bake) {
    currentState = bake;
  } else if (previousState == airfry) {
    currentState = toast;
  } else if (previousState == broil) {
    currentState = bake;
  }

  // Print the current button states and the current state for debugging
  // Serial.print("Heater: ");
  // Serial.print(heater);
  // Serial.print(" | Fan: ");
  // Serial.print(fan);
  // Serial.print(" | Switch: ");
  // Serial.print(switchState);
  // Serial.print(" | Potentiometer: ");
  // Serial.print(potValue);
  // Serial.print(" | State: ");
  // Serial.println(currentState);

  // Determine the mode based on the current state and control HID buttons
  switch (currentState) {
    case airfry:
      // Serial.println("Mode: Air Fry");
      Joystick.setButton(0, HIGH);
      Joystick.setButton(1, LOW);
      Joystick.setButton(2, LOW);
      Joystick.setButton(3, LOW);
      break;

    case toast:
      // Serial.println("Mode: Toast");
      Joystick.setButton(0, LOW);
      Joystick.setButton(1, HIGH);
      Joystick.setButton(2, LOW);
      Joystick.setButton(3, LOW);
      break;

    case broil:
      // Serial.println("Mode: Broil");
      Joystick.setButton(0, LOW);
      Joystick.setButton(1, LOW);
      Joystick.setButton(2, HIGH);
      Joystick.setButton(3, LOW);
      break;

    case bake:
      // Serial.println("Mode: Bake");
      Joystick.setButton(0, LOW);
      Joystick.setButton(1, LOW);
      Joystick.setButton(2, LOW);
      Joystick.setButton(3, HIGH);
      break;

    default:
      // Default case (should not be reached)
      Serial.println("Mode: Unknown");
      Joystick.setButton(0, LOW);
      Joystick.setButton(1, LOW);
      Joystick.setButton(2, LOW);
      Joystick.setButton(3, LOW);
      break;
  }

  // Set the switch state as a joystick button
  if (switchState) {
  Joystick.setButton(4, HIGH);
  } else {
  Joystick.setButton(4, LOW);
  }

  // Set the potentiometer value as a joystick axis
  Joystick.setXAxis(potValue * 15);  // Scale value to joystick range (0-1023)

  // Update the previous state
  previousState = currentState;

  // Simple debounce delay
  delay(50);
}
