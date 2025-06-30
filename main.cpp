#include <Servo.h>
#include <LiquidCrystal.h>

// LCD Pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int buttonPin = 7;
int buttonState;
int lastButtonState = HIGH;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

int selection = 0; // 0-3
Servo servos[4];
int servoPins[] = {8, 9, 10, 6}; // Connect servos to these pins

void setup() {
  lcd.begin(16, 2);
  pinMode(buttonPin, INPUT_PULLUP);

  for (int i = 0; i < 4; i++) {
    servos[i].attach(servoPins[i]);
  }

  displayMenu();
}

void loop() {
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW && buttonState == HIGH) {
      selection = (selection + 1) % 4; // Cycle through 0 to 3
      displayMenu();
    }

    // Hold button for 2 seconds to dispense
    if (reading == LOW && millis() - lastDebounceTime > 2000) {
      dispenseProduct(selection);
    }
  }

  buttonState = reading;
  lastButtonState = reading;
}

void displayMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Select Product:");
  lcd.setCursor(0, 1);
  lcd.print(">> Product ");
  lcd.print((char)('A' + selection));
}

void dispenseProduct(int index) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dispensing ");
  lcd.print((char)('A' + index));
  
  servos[index].write(90); // Spin servo (90 = start rotation for FS90R)
  delay(1000); // Adjust based on how long it takes to push product
  servos[index].write(0); // Stop servo

  delay(1000); // Pause before returning
  displayMenu();
}
