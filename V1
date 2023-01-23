//FUTURE IDEAS
//https://create.arduino.cc/iot/
//https://demo.thingsboard.io/home
//https://blynk.io/developers

#include <Ethernet.h>
#include <SunPosition.h>
#include <Wire.h>
#include <RTClib.h>
#include <HMC5883L.h>
#include <math.h> // added for trigonometry functions




//DEFINIMOS LAS ENTRADAS Y SALIDAS DEL MOTOR 

int motor1A = 4;  // pin for motor 1 input A
int motor1B = 5;  // pin for motor 1 input B
int motor2A = 6; // pin for motor 2 input A
int motor2B = 7; // pin for motor 2 input B
int motor3A = 2;  // pin for motor 3 input A
int motor3B = 3;  // pin for motor 3 input B

unsigned long startTime; // Start time of the motor movement

//INDICAMOS EL MODULO RTC 

RTC_DS3231 myRTC;
char t[32];
int minutesSinceMidnight;
SunPosition sun;



// Set up the variables for storing the GPS coordinates and time
float latitude;
float longitude;
int year;
int month;
int day;
int hour;
int minute;
int second;
float timeZone;
bool isCalculated = false;
bool isPrinted = false;

bool isActivated = false;


void setup() {

Serial.begin(9600);
 Wire.begin();
myRTC.begin();

  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);
  pinMode(motor3A, OUTPUT);
  pinMode(motor3B, OUTPUT);
}

void loop() {
  DateTime now = myRTC.now();
//if (now.hour() == 5 && now.minute() == 45 && now.second() == 0 && !isCalculated) {
// It's exactly midnight, calculate the sun's position
int minutesSinceMidnight = now.hour() * 60 + now.minute();
 // COMPROBAR CON https://www.suncalc.org/#/38.0477,-1.1953,13/2023.01.12/08:18/1/0
SunPosition sun(38.05, -1.24, now.unixtime());
isCalculated = true;
//}

if (now.hour() == 5 && now.minute() == 45 && now.second() == 10 && !isPrinted) {
// It's exactly 00:05:10, print the sun's position
Serial.print("MIDDAY ZENITH - ");
Serial.println(sun.zenith());
Serial.print("ALTITUDE - ");
Serial.println(sun.altitude());
Serial.print("SUNRISE - ");
Serial.println(sun.sunrise()); 
Serial.print("NOON TIME - ");
Serial.println(sun.noon());
Serial.print("SUNSET - ");
Serial.println(sun.sunset());
Serial.print("Minutes passed since midnight : ");
Serial.println(minutesSinceMidnight);
isPrinted = true;

}
if (minutesSinceMidnight == sun.sunrise()  && !isActivated) {
  // Deschidem Clapanu 1
digitalWrite(motor1A, LOW);
    digitalWrite(motor1B, HIGH);
    delay(3000);
    Serial.println("Clapan 1 deschis ");

    // Inchidem Clapanu 2
    digitalWrite(motor2A, HIGH);
        delay(3000);
    digitalWrite(motor2B, LOW);
    Serial.println("Clapan 2 inchis ");
    isActivated = true;
    Serial.println("System wake up ");

    // MOTORU · SUS
    digitalWrite(motor3A, HIGH);
    delay(30000); // wait for 30 seconds
    digitalWrite(motor3B, LOW);
    Serial.println("motoru 3 SUS  ");
}
if (minutesSinceMidnight >= sun.sunrise() && minutesSinceMidnight < sun.noon()) {
    if ((minutesSinceMidnight - sun.sunrise()) % 10 == 0) {
        digitalWrite(motor3A, LOW);
        digitalWrite(motor3B, HIGH); //MOTOR 3 SE COBOARA JOS FIECARE 10 MIN PENTR O SECUNDA
                delay(1000);

                      Serial.println("Down by 2 degree ");

    }
    if (minutesSinceMidnight == sun.noon()) {
        digitalWrite(motor3A, LOW);
        digitalWrite(motor3B, LOW);
            Serial.println("System is at base");

   // Deschidem Clapanu 2
digitalWrite(motor2A, LOW);
    digitalWrite(motor2B, HIGH);
        delay(3000);
    Serial.println("Clapan 2 DESCHIS ");
              // Inchidem Clapanu 1
digitalWrite(motor1A, HIGH);
        delay(3000);
    digitalWrite(motor1B, LOW);
    Serial.println("Clapan 1 INCHIS ");

    }
}

if (minutesSinceMidnight > sun.noon() && minutesSinceMidnight < sun.sunset()) {
    if ((minutesSinceMidnight - sun.sunrise()) % 10 == 0) {
        digitalWrite(motor3A, HIGH);
            delay(1000);
        digitalWrite(motor3B, LOW);
       Serial.println("Up by 2 degree ");

    }
    if (minutesSinceMidnight == sun.sunset()) {
        digitalWrite(motor3A, LOW);
        digitalWrite(motor3B, LOW);
            Serial.println("System is at pick");

   // MOTORU 3 · JOS
    digitalWrite(motor3A, LOW);
    digitalWrite(motor3B, HIGH);
        delay(30000); // wait for 30 seconds
    Serial.println("motoru 3 JOS  ");


              // inchidem Clapanu 2
digitalWrite(motor2A, HIGH);
    digitalWrite(motor2B, LOW);
    Serial.println("Clapan 2 DESCHIS ");
    isActivated = true;

}
}

}
