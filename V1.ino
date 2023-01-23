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
// It's exactly midnight, calculate the sun's position
int minutesSinceMidnight = now.hour() * 60 + now.minute();
 // COMPROBAR CON https://www.suncalc.org/#/38.0477,-1.1953,13/2023.01.12/08:18/1/0
SunPosition sun(38.05, -1.24, now.unixtime());


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

   // Check if the current time is 5 minutes before the calculated sunrise time
    if (minutesSinceMidnight == sun.sunrise() - 5) {
        // Start motor 1 in backward direction for 5 seconds
        digitalWrite(motor1A, HIGH);
        digitalWrite(motor1B, LOW);
        delay(5000);
        Serial.println("LOCK1 closed");
        // Stop motor 1
        digitalWrite(motor1A, LOW);
        digitalWrite(motor1B, LOW);
        delay(2000);

        // Start motor 2 in forward direction for 5 seconds
        digitalWrite(motor2A, LOW);
        digitalWrite(motor2B, HIGH);
        delay(5000);
        Serial.println("LOCK2 opened");
        // Stop motor 2
        digitalWrite(motor2A, LOW);
        digitalWrite(motor2B, LOW);
        delay(2000);

        // Start motor 3 in forward direction for 30 seconds
        digitalWrite(motor3A, LOW);
        digitalWrite(motor3B, HIGH);
        delay(30000);
        // Stop motor 3
        digitalWrite(motor3A, LOW);
        digitalWrite(motor3B, LOW);
        Serial.println("System is UP");
    }
    
    if (minutesSinceMidnight >= sun.sunrise() && minutesSinceMidnight < sun.noon()) {
        if (minutesSinceMidnight % 10 == 0) {
            // Start motor 3 in backward direction for 3 seconds
        digitalWrite(motor3A, HIGH);
       digitalWrite(motor3B, LOW);
       delay(3000);
       Serial.println("DOWN BY 2 DEGREES");

            // Stop motor 3
       digitalWrite(motor3A, LOW);
       digitalWrite(motor3B, LOW);
        }
    }


    if (minutesSinceMidnight >= sun.noon() && minutesSinceMidnight < sun.sunset()) {
        if (minutesSinceMidnight == sun.noon() ) {

              // Stop motor 3
            Serial.println("minutesSinceMidnight is equal to sun.noon");

            digitalWrite(motor3A, LOW);
            digitalWrite(motor3B, LOW);
            // Start motor 1 in backward direction for 5 seconds
            digitalWrite(motor1A, HIGH);
            digitalWrite(motor1B, LOW);
            delay(5000);
                    Serial.println("LOCK1 OPENED");

            // Stop motor 1
            digitalWrite(motor1A, LOW);
            digitalWrite(motor1B, LOW);
            Serial.println("motor 1 is stopped");

            // Start motor 2 in forward direction for 5 seconds
            digitalWrite(motor2A, LOW);
            digitalWrite(motor2B, HIGH);
            delay(5000);
            Serial.println("LOCK2 CLOSED");

            // Stop motor 2
            digitalWrite(motor2A, LOW);
            digitalWrite(motor2B, LOW);
            Serial.println("motor 2 is stopped");
        }

if (minutesSinceMidnight % 10 == 0) {
    // Start motor 3 in forward direction for 3 seconds
            digitalWrite(motor3A, LOW);
            digitalWrite(motor3B, HIGH);
            delay(3000);
            // Stop motor 3
            digitalWrite(motor3A, LOW);
            digitalWrite(motor3B, LOW);
            Serial.println("motor 3 is stopped");
        }
    }

 if (minutesSinceMidnight == sun.sunset()){
// Stop motor 3
    digitalWrite(motor3A, LOW);
    digitalWrite(motor3B, LOW);
     delay(3000);

    }

}
