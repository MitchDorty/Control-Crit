const int speakerPin = 10;   // Pin for the speaker
const int frequency = 340;   // Starting frequency
const int ledPin = 9;        // Pin for the red LED
const int buttonPin = 11;    // Pin for the button

int timer = 25;               // Initial timer value in seconds
int initialDelayTime = 1250;  // Initial delay time for the speaker
int delayTime;

unsigned long lastButtonPress = 0; // Variable to store the last button press time
unsigned long debounceDelay = 50;  // Debounce delay in milliseconds

void setup() {
  pinMode(speakerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);

  Serial.begin(9600); // Initialize Serial communication

  // Initial setup for LED brightness
  analogWrite(ledPin, map(timer, 0, 15, 0, 255));
}

void loop() {
  // Gradually decrease the delay between sounds over 10 seconds
  for (delayTime = initialDelayTime; delayTime >= 50; delayTime -= 50) {
    // Check if the button is pressed and debounced
    if (isButtonPressed()) {
      resetTimer();
      // Clear the Serial Monitor when the button is pressed
      Serial.flush();
      Serial.println("Timer Reset");
    }

    tone(speakerPin, frequency);
    delay(100);
    noTone(speakerPin);
    delay(150);
    tone(speakerPin, frequency);
    delay(100);
    noTone(speakerPin);
    delay(delayTime);

    // Update and display the timer countdown
    updateTimer();
    updateLED();

    // Display the delayTime in the Serial Monitor
    Serial.print("Delay Time: ");
    Serial.println(delayTime);

    // When the timer runs out, break out of the loop
    if (timer <= 0) {
      digitalWrite(ledPin, LOW); // Turn off the LED
      playContinuousTone();       // Play a continuous tone on the speaker
      break; // Exit the loop
    }
  }

  // Pause for a moment before restarting the loop
  delay(2000);
}

bool isButtonPressed() {
  // Read the state of the button
  int buttonState = digitalRead(buttonPin);

  // Check if the button is pressed and debounced
  if (buttonState == HIGH && (millis() - lastButtonPress) > debounceDelay) {
    lastButtonPress = millis(); // Record the last button press time
    return true;
  }

  return false;
}

void resetTimer() {
  timer = 25;                // Reset the timer to 15 seconds
  initialDelayTime = 1250;   // Reset initial delay time
  analogWrite(ledPin, map(timer, 0, 15, 0, 255)); // Reset LED brightness
  // Reset the delayTime to the initialDelayTime value
  delayTime = initialDelayTime;
}

void updateTimer() {
  delay(100); // Update every 100 milliseconds
  timer--;

  // Adjust initial delay time based on the remaining time
  initialDelayTime = map(timer, 0, 15, 500, 1500);

  // Display the timer value in the Serial Monitor
  Serial.print("Timer: ");
  Serial.println(timer);
}

void updateLED() {
  // Adjust LED brightness based on the timer value
  analogWrite(ledPin, map(timer, 0, 15, 0, 255));
}

void playContinuousTone() {
  while (true) {
    tone(speakerPin, frequency); // Play a continuous tone
    delay(200); // Add a longer delay to control the pause between beeps
  }
}