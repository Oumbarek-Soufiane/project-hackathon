#include <Wire.h> 
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

Servo servo; // Création d'un objet Servo
bool objectDetected = false;
bool flameDetected = false;
bool flameHandled = false; // Flag to indicate if flame detection has been handled
LiquidCrystal_I2C lcd(0x27, 16, 2); 
const int trigPin = 3;
const int echoPin = 8;
const int redLed = 10;
const int greenLed = 6;
const int buzzerPin = 5;
const int flamePin = 2; // Digital pin for flame sensor
long duration, distance;

void setup() {       
  pinMode(redLed, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(flamePin, INPUT); // Flame sensor pin as input
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servo.attach(13); // Attacher le servo moteur à la broche 9
  lcd.init();
  lcd.backlight();
  lcd.print("garbage!");
  servo.write(10);
  Serial.begin(9600);

}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1; // Convert duration to distance in cm
  Serial.println(digitalRead(flamePin));
  // Check distance for object detection
  if (!flameDetected) { // If no flame is detected
    if (distance <= 10 && !objectDetected) { // If an object is detected within 10 cm
      objectDetected = true;
      servo.write(90); // Move the servo to 90 degrees position
      lcd.setCursor(0, 0);
      lcd.print("Throw the trash");
      lcd.setCursor(2, 1);
      lcd.print("please!!");
      digitalWrite(redLed, LOW);
      digitalWrite(greenLed, HIGH);
      digitalWrite(buzzerPin, LOW);
      delay(8000);
    } else if (distance > 8 && objectDetected) { // If no object is detected and was previously detected
      objectDetected = false;
      servo.write(10); // Move the servo to 0 degrees position
      lcd.setCursor(0, 0);
      lcd.print("Trash Thrown !!");
      lcd.setCursor(2, 1);
      lcd.print("Thank You !!");
      digitalWrite(redLed, HIGH);
      digitalWrite(greenLed, LOW);
      digitalWrite(buzzerPin, LOW);
      delay(1000);
    }
  }
  
  // Check for flame detection
  if (!flameDetected && digitalRead(flamePin) == LOW) { // If flame is detected
    flameDetected = true;
    servo.write(90); // Move the servo to 180 degrees position
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Flame Detected!");
    digitalWrite(greenLed, LOW);
   digitalWrite(buzzerPin, HIGH); // Activate the buzzer
    for(int i=0;i<2;i++){
      digitalWrite(redLed, LOW);
      delay(500);
       digitalWrite(redLed, HIGH);
      delay(500);
        digitalWrite(redLed, LOW);
    }
    // Set flag to indicate flame detection handled
    flameHandled = true;
  } else if (flameDetected && digitalRead(flamePin) == HIGH && flameHandled) { // If flame is extinguished and detection has been handled
    flameDetected = false;
    flameHandled = false;
    // Return to normal operation
   lcd.clear();
   servo.write(90); // Move the servo to 90 degrees position
      lcd.setCursor(0, 0);
      lcd.print("Throw the trash");
      lcd.setCursor(2, 1);
      lcd.print("please!!");
      digitalWrite(redLed, LOW);
      delay(200);
      digitalWrite(redLed, HIGH);
      digitalWrite(greenLed, HIGH);
      digitalWrite(buzzerPin, LOW);
      delay(4);
      servo.write(10);

  }
}
