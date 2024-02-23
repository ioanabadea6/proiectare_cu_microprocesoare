#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Keypad.h>
#include <Key.h>
#include <SoftwareSerial.h>

//declarare buzzer
#define buzzer 11

//declarare senzor ultrasonic
#define trigPin 13
#define echoPin 12

//declarare keypad
const byte rows = 4;
const byte cols = 3;
char keypadMap[rows][cols] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
char keyPressed;
byte rowPins[rows] = {9, 8, 7, 6};
byte colPins[cols] = {5, 4, 3};
Keypad keypad = Keypad(makeKeymap(keypadMap), rowPins, colPins, rows, cols);

//declarare lcd
LiquidCrystal_I2C lcd(0x27, 16, 2);

//distanta
long microsec;
int distance = 0;
int initialDistance = 10;
int currentDistance = 0;

String password = "";
String tempPassword = "";
boolean activated = true;
boolean activateAlarm = false;
boolean alarmActivated = false;
boolean enteredPassword = true;
boolean passwordChange = true;

int i = 0;

//pentru modul gsm
SoftwareSerial mySerial(10, 2);// tx si rx

void setup(){
  //initializare lcd
  lcd.init();
  lcd.backlight();

  //setare buzzer, trigPin, echoPin ca iesire
  pinMode(buzzer, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //comunicare seriala arduino si modul
  Serial.begin(9600);
  mySerial.begin(9600);
  // Serial.println("Initializing..."); 
  // delay(1000);
  // mySerial.println("AT"); 
  // updateSerial();
  // mySerial.println("AT+CMGF=1"); 
  // updateSerial();
  // mySerial.println("AT+CNMI=1,2,0,0,0");
  // updateSerial();
}

void loop(){
  //activare alarma
  if(activateAlarm == true){
    lcd.clear();
    int sec = 10;
    while(sec != 0){
      lcd.setCursor(0, 0);
      lcd.print("Alarm will be");
      lcd.setCursor(0, 1);
      lcd.print("activated in ");
      lcd.print(sec);
      sec = sec - 1;
      tone(buzzer, 800, 100);
      delay(1000);
      lcd.clear();
    }
    lcd.clear();
    if(sec == 0){
      lcd.setCursor(3, 0);
      lcd.print("ACTIVATED!");
    }
    initialDistance = getDistance();
    alarmActivated = true;
    activateAlarm = false;
  }

  if(alarmActivated == true){
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("ACTIVATED!");
    currentDistance = getDistance() + 10;
    if (initialDistance > currentDistance){
  //      Serial.begin(9600);
  // mySerial.begin(9600);
  Serial.println("Initializing1..."); 
  delay(1000);
      mySerial.println("AT+CMGF=1");
      updateSerial();
      mySerial.println("AT+CMGS=\"+40761187211\"");
      updateSerial();
      mySerial.print("ALERT!");
      updateSerial();
      mySerial.write(26);

      Serial.end();
      mySerial.end();
      
      tone(buzzer, 1000, 500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter pass:");
      delay(1000);
      i = 0; 
      while(activated){
        keyPressed = keypad.getKey();
        if(keyPressed == '1' || keyPressed == '2' || keyPressed == '3' || 
          keyPressed == '4' || keyPressed == '5' || keyPressed == '6' || 
          keyPressed == '7' || keyPressed == '8' || keyPressed == '9' || keyPressed == '0'){
          tempPassword += keyPressed;
          lcd.setCursor(i, 1);
          lcd.print("*");
          i++;
        }
        if(keyPressed == '#'){
          activated = false;
          if(tempPassword == password){
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("CORRECT");
            delay(2000);
            alarmActivated = false;
            noTone(buzzer);
          }
          else{
            activated = false;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("INCORRECT");
            delay(2000);
            tone(buzzer, 1000, 200);
          }
        }
      }     
    }
  }

  if(alarmActivated == false) {
    //ce face
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("*-ACTIVATE");

    // lcd.setCursor(0, 1);
    //lcd.print("D-SET C-CHANGE");
    lcd.clear();
    
    //activare alarma
    keyPressed = keypad.getKey();
    if (keyPressed == '*'){
      activateAlarm = true;
      //tone(buzzer, 1000, 200);
    }

    //setare parola
    // if(keyPressed = "#"){
    // //   Serial.begin(9600);
    // //   mySerial.begin(9600);
    //   Serial.println("Initializing2..."); 
    //   delay(1000);
    //   mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
    //   updateSerial2();
    //   mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
    //   updateSerial2();
    //   mySerial.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
    //   updateSerial2();
    // }

    //setare parola
    // if (keyPressed == '*'){
    //   enteredPassword = true;
    //   lcd.clear();
    //   tone(buzzer, 1000, 200);
    //   lcd.setCursor(0, 0);
    //   lcd.print("Set pass");
    //   delay(2000);
    //   lcd.clear();
    //   lcd.setCursor(0, 0);
    //   lcd.print("Enter pass:");
    //   while(enteredPassword){
    //     keyPressed = keypad.getKey();
    //     if(keyPressed == '1' || keyPressed == '2' || keyPressed == '3' || 
    //       keyPressed == '4' || keyPressed == '5' || keyPressed == '6' || 
    //       keyPressed == '7' || keyPressed == '8' || keyPressed == '9' || keyPressed == '0'){
    //       password += keyPressed;
    //       lcd.setCursor(i, 1);
    //       lcd.print("*");
    //       i++;
    //     }
    //     if(keyPressed == '#'){
    //       enteredPassword = false;
    //       lcd.clear();
    //       lcd.setCursor(0, 0);
    //       lcd.print("PASSWORD IS SET!");
    //       delay(3000);
    //     }
    //   }
    // }

    if (keyPressed == '#'){
      passwordChange = true;
      password = "";
      i = 0;
      lcd.clear();
      tone(buzzer, 1000, 200);
      lcd.setCursor(0, 0);
      lcd.print("Change pass");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter pass:");
      while(passwordChange){
        keyPressed = keypad.getKey();
        if(keyPressed == '1' || keyPressed == '2' || keyPressed == '3' || 
          keyPressed == '4' || keyPressed == '5' || keyPressed == '6' || 
          keyPressed == '7' || keyPressed == '8' || keyPressed == '9' || keyPressed == '0'){
          password += keyPressed;
          lcd.setCursor(i, 1);
          lcd.print("*");
          i++;
        }
        if(keyPressed == '#'){
          passwordChange = false;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("PASS IS CHANGED!");
          delay(3000);
        }
      }
    }
  }
}

long getDistance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);
  microsec = pulseIn(echoPin, HIGH);
  distance = microsec / 29 / 2; 
  return distance;
}

void updateSerial(){
  delay(500);
  while(Serial.available()){
    mySerial.write(Serial.read());
  }
  while(mySerial.available()){
    Serial.write(mySerial.read());
  }
}

// void updateSerial2(){
//   delay(500);
//   while (Serial.available()) {
//     mySerial.write(Serial.read());
//     if(mySerial.read()){
//       password += mySerial.read();
//     }
//   }
//   while(mySerial.available()) {
//     Serial.write(mySerial.read());
//     if(mySerial.read()){
//       password += mySerial.read();
//     }
//   }
// }
