#include <Joystick.h>

// Constants: Pin numbers for buttons and other inputs
const int heaterPin = 4;  // Pushbutton A pin number
const int fanPin = 2;     // Pushbutton B pin number
const int timerPin = 3;  // Input timer pin number
const int potFeedbackPin = A0;   // Analog pin for pot

bool heater = false; 
bool fan = false; 
bool timerState = false;
int potValue = 0;

const int unknown = -1;
const int bake = 0; // it will never reach bake because we dont have enough info
const int broil = 1;
const int toast = 2;
const int airfry = 3;
int previousState = unknown;

// Create Joystick instance
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD, 5, 0, true, true, false, false, false, false, false, false, false, false, false);

void setup() {
  Serial.begin(9600);

  // Initialize input pins as inputs with internal pull-up resistors
  pinMode(heaterPin, INPUT_PULLUP);
  pinMode(fanPin, INPUT_PULLUP);
  pinMode(timerPin, INPUT_PULLUP);

  // Initialize Joystick library
  Joystick.begin();
}

void loop() {
  // Read the inputs
  heater = digitalRead(heaterPin) == LOW;
  fan = digitalRead(fanPin) == LOW;
  timerState = digitalRead(timerPin) == LOW;
  potValue = analogRead(potFeedbackPin);

  // Determine the current cook setting
  int currentState = unknown;

  // fan and heater off? broiling!
  if (!heater && !fan) {
    currentState = broil;

  // fan and heater on? air frying!
  } else if (heater && fan) {
    currentState = airfry;

  // dont know current output? maybe toasting idk
  } else {
    currentState = toast;
  }

  // Print the current button states and the current state for debugging
  Serial.print("Heater: ");
  Serial.print(heater);
  Serial.print(" | Fan: ");
  Serial.print(fan);
  Serial.print(" | timer: ");
  Serial.print(timerState);
  Serial.print(" | pot: ");
  Serial.print(potValue);
  Serial.print(" | State: ");
  Serial.println(currentState);

  // Determine the mode based on the current state and control HID buttons
  switch (currentState) {
    case airfry:
      Joystick.setButton(0, HIGH);
      Joystick.setButton(1, LOW);
      Joystick.setButton(2, LOW);
      Joystick.setButton(3, LOW);
      break;

    case toast:
      Joystick.setButton(0, LOW);
      Joystick.setButton(1, HIGH);
      Joystick.setButton(2, LOW);
      Joystick.setButton(3, LOW);
      break;

    case broil:
      Joystick.setButton(0, LOW);
      Joystick.setButton(1, LOW);
      Joystick.setButton(2, HIGH);
      Joystick.setButton(3, LOW);
      break;

    case bake:
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

  // Set the timer state as a joystick button
  if (!timerState) {
    Joystick.setButton(4, HIGH);
  } else {
    Joystick.setButton(4, LOW);
  }

  // Set the pot value as a joystick axis
  Joystick.setXAxis(map(potValue, 0, 1023, 0, 1023));  // Scale value to joystick range (0-1023)

  // Update the previous state
  previousState = currentState;

  // Simple debounce delay
  delay(50);
}
