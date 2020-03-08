/*
By: Sebastian Whitehead
Credit: Dejan (howtomechatronics.com) *Gyroscope code modified

==== Instructions ====
1. Shake to roll the dice (default d10000)
2. Use the buttons to cycle between the different dice types
3. To mute and unmute hold the selection up button until the 'Shake me' screen appears then release
4. Adjust the potentiometer to regulate text clarity

Distrobuted under the MIT license see LICENSE.md for more details
*/


#include <Wire.h>
#include <LiquidCrystal.h>

// == Pins == //
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int DownPin = 7;
const int UpPin = 8;
const int Speaker = 6;

// == Variables == //
const int MPU = 0x68;         // MPU6050 I2C address
float GyroX, GyroY, GyroZ;
long MaxRoll = 10001;         // Default Setting
int Dice = 9;                 // Number of dice types
long result = 0;              // Role result
unsigned long LastAction = 0; // Time of last screen effecting action
bool UpCompleted = false;     // Variables for preventing rapidly recurring presses
bool DownCompleted = false;
int MuteStart = 0;            // Start Time for mute function
int MuteStop = 0;             // Stop Time for mute function

// ==== Options ==== //
int IdleDelay = 30000;  // Standard delay before screen reverts to idle state
bool Mute = true; // Default Mute Setting



void setup() {
  Serial.begin(19200);
  pinMode(UpPin, INPUT);    //Button Pins
  pinMode(DownPin, INPUT);

  // Starting Gyro //
  Wire.begin();                      // Initialize comunication
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        // end the transmission
  delay(20);

  // Start LCD //
  lcd.begin(8, 2);

  // Starting Tune //
  tone(Speaker, 300);
  delay(250);
  tone(Speaker, 400);
  delay(250);
  tone(Speaker, 500, 250);
  idle(); // Idle Screen
}

void loop() {
  Gyro(); // Position sensing funktion
  if (millis() > LastAction + IdleDelay) { // Returns to idle after 30 seconds
    idle();
  }

  // ==== Push Button Up ==== //
  if (digitalRead(UpPin) == HIGH && UpCompleted == false) {
    Dice ++;
    Change();
    UpCompleted = true;
    MuteStart = millis();

    // ==== Release Button Up ==== //
  } else if (digitalRead(UpPin) == LOW && UpCompleted == true) {
    UpCompleted = false;
    MuteStop = millis();

    // == Long Press Mute Toggle == //
    if (MuteStop - MuteStart > 3000 && Mute == true) {
      Mute = false;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Unmuted");
    } else if (MuteStop - MuteStart > 3000 && Mute == false) {
      Mute = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  Muted");
    }
    IdleDelay = 3000;
    LastAction = millis();
  }

  // ==== Push Button Down ==== //
  if (digitalRead(DownPin) == HIGH && DownCompleted == false) {
    Dice --;
    Change();
    DownCompleted = true;

    // ==== Relase Button Up ==== //
  } else if (digitalRead(DownPin) == LOW) {
    DownCompleted = false;
  }
}

// === Read gyroscope data === //
void Gyro() {
  Wire.beginTransmission(MPU);
  Wire.write(0x43); // Gyro data first register address 0x43
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 4 registers total, each axis value is stored in 2 registers
  GyroX = (Wire.read() << 8 | Wire.read()) / 131.0; // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
  GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;

  // Produces values for each rotation axsis between 0 -> 255
  GyroX = GyroX + 0.56; // GyroErrorX ~(-0.56)
  GyroY = GyroY - 2;    // GyroErrorY ~(2)
  GyroZ = GyroZ + 0.79; // GyroErrorZ ~ (-0.8)

  /*
    // Print the values on the serial monitor for testing
    Serial.print(GyroX);
    Serial.print("/");
    Serial.print(GyroY);
    Serial.print("/");
    Serial.println(GyroZ);
  */

  // === Roll Paramiters === //
  if (GyroX >= 250 || GyroY >= 240 || GyroZ >= 240 || GyroX <= -245 || GyroY <= -245 || GyroZ <= -245) {
    roll();
    LastAction = millis(); // Time for last roll
    IdleDelay = 30000;
  }
}

void idle() {
  // ==== Idle funktion ==== //
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SHAKE");
  lcd.setCursor(0, 1);
  lcd.print("     ME!");
  LastAction = millis();
  IdleDelay = 30000;  //Reset Idle Timeout
}

void roll() {
  randomSeed(analogRead(0));    //Sets the seed for the random function to seconds since program start
  result = random(1, MaxRoll);  // 'Rolls The Dice'

  // ==== Rolling Animation + Sound ==== //
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Rolling");
  Serial.println("Rolling"); // print for testing
  Serial.println("");
  delay(250);
  lcd.setCursor(0, 1);
  sound(350, 15);
  lcd.print(".");
  delay(250);
  sound(375, 15);
  lcd.print(".");
  delay(250);
  sound(400, 15);
  lcd.print(".");
  delay(250);
  sound(350, 15);
  lcd.setCursor(0, 1);
  lcd.print("        ");
  lcd.setCursor(0, 1);
  lcd.print(".");
  delay(250);
  sound(375, 15);
  lcd.print(".");
  delay(250);
  sound(400, 15);
  lcd.print(".");
  delay(500);
  lcd.clear();
  lcd.setCursor(0, 0);

  // === Heads / Tails for Coin === //
  if (Dice == 0) {
    if (result == 1) {
      lcd.print(" Heads");
    } else if (result == 2) {
      lcd.print(" Tails");
    }

    // === 'Result' Text Centering === //
  } else if (result < 100) {
    lcd.print("   ");
    lcd.print (result);
  } else {
    lcd.print ("  ");
    lcd.print (result);
  }
  sound(400, 200);
  delay(200);
  sound(500, 200);
}

// === Muteable Sound Function === //
void sound(int i, int n) {
  if (Mute == false) {
    tone(Speaker, i, n);
  }
}

// ==== Cycle Between Different Dice Types ==== //
void Change() {
  // == looping value == //
  if (Dice > 9) {
    Dice = 0;
  } else if (Dice < 0) {
    Dice = 9;
  }

  if (Dice == 0) {         // == Coin Flip == //
    MaxRoll = 2 + 1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  Coin");

  } else if (Dice == 1) {  // == d4 == //
    MaxRoll = 4 + 1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   d4");

  } else if (Dice == 2) {   // == d6 == //
    MaxRoll = 6 + 1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   d6");

  } else if (Dice == 3) {   // == d8 == //
    MaxRoll = 8 + 1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   d8");

  } else if (Dice == 4) {   // == d10 == //
    MaxRoll = 10 + 1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   d10");

  } else if (Dice == 5) {   // == d12 == //
    MaxRoll = 12 + 1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   d12");

  } else if (Dice == 6) {   // == d20 == //
    MaxRoll = 20 + 1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   d20");

  } else if (Dice == 7) {   // == d100 == //
    MaxRoll = 100 + 1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  d100");

  } else if (Dice == 8) {   // == d1000 == //
    MaxRoll = 1000 + 1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  d1000");

  } else if (Dice == 9) {   // == d10000 == //
    MaxRoll = 10000 + 1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" d10000");
  }

  LastAction = millis();
  IdleDelay = 3000; // Temporarily Reduce Idle Timeout
}
