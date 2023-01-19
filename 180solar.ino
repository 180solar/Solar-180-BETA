//FUTURE IDEAS
//https://create.arduino.cc/iot/
//https://demo.thingsboard.io/home
//https://blynk.io/developers

#include <Ethernet.h>
#include <SunPosition.h>
#include <Wire.h>
#include <DS3231.h>       //Jarzebski DS3231
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

RTClib myRTC;
HMC5883L compass;
float angleDifference = angleDifference;
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


// Initialize HMC5883L
 Serial.println("Initialize HMC5883L");
while (!compass.begin())
{
 Serial.println("Could not find a valid HMC5883L sensor, check wiring!");
 delay(500);
}
  
  // Set measurement range
  // +/- 0.88 Ga: HMC5883L_RANGE_0_88GA
  // +/- 1.30 Ga: HMC5883L_RANGE_1_3GA (default)
  // +/- 1.90 Ga: HMC5883L_RANGE_1_9GA
  // +/- 2.50 Ga: HMC5883L_RANGE_2_5GA
  // +/- 4.00 Ga: HMC5883L_RANGE_4GA
  // +/- 4.70 Ga: HMC5883L_RANGE_4_7GA
  // +/- 5.60 Ga: HMC5883L_RANGE_5_6GA
  // +/- 8.10 Ga: HMC5883L_RANGE_8_1GA
  compass.setRange(4); // Set the range to 4 gauss

  // Set measurement mode
  // Idle mode:              HMC5883L_IDLE
  // Single-Measurement:     HMC5883L_SINGLE
  // Continuous-Measurement: HMC5883L_CONTINOUS (default)
  compass.setMeasurementMode(HMC5883L_CONTINOUS);
 
  // Set data rate
  //  0.75Hz: HMC5883L_DATARATE_0_75HZ
  //  1.50Hz: HMC5883L_DATARATE_1_5HZ
  //  3.00Hz: HMC5883L_DATARATE_3HZ
  //  7.50Hz: HMC5883L_DATARATE_7_50HZ
  // 15.00Hz: HMC5883L_DATARATE_15HZ (default)
  // 30.00Hz: HMC5883L_DATARATE_30HZ
  // 75.00Hz: HMC5883L_DATARATE_75HZ
compass.setDataRate(HMC5883L_DATARATE_15HZ);

  // Set number of samples averaged
  // 1 sample:  HMC5883L_SAMPLES_1 (default)
  // 2 samples: HMC5883L_SAMPLES_2
  // 4 samples: HMC5883L_SAMPLES_4
  // 8 samples: HMC5883L_SAMPLES_8
compass.setSamples(HMC5883L_SAMPLES_1);

// GET DATA FROM THE COMPASS

Vector raw = compass.readRaw();
Vector norm = compass.readNormalize();
Serial.print("Current position compass: ");
 Serial.print(" Xraw = ");
  Serial.print(raw.XAxis);
  Serial.print(" Yraw = ");
 Serial.print(raw.YAxis);
   Serial.print(" Zraw = ");
  Serial.print(raw.ZAxis);
 Serial.print(" Xnorm = ");
   Serial.print(norm.XAxis);
  Serial.print(" Ynorm = ");
  Serial.print(norm.YAxis);
  Serial.print(" ZNorm = ");
 Serial.print(norm.ZAxis);
 Serial.println();  
  delay(100);

// Calculate tilt angle (inclination)
   float tilt = atan2(norm.YAxis, norm.XAxis);
   tilt = tilt * 180 / M_PI; // convert to degrees

       // Print tilt angle
     Serial.println("Tilt angle: " + String(tilt) + " degrees");
    delay(500);


//OBTENEMOS LA HORA LOCAL IN MINUTE DE LA 00:00

 // Initialize the RTC module

Wire.begin();
DateTime now = myRTC.now();
time_t nowTime = now.unixtime();
tm *tm = localtime(&nowTime);
int minutesSinceMidnight = tm->tm_hour * 60 + tm->tm_min;
Serial.print("Minutes passed since midnight : ");
Serial.println(minutesSinceMidnight);


Serial.print("Current time: ");
Serial.print(now.hour());
Serial.print(':');
Serial.print(now.minute());
Serial.print(':');
Serial.println(now.second());


  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);
  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);
  pinMode(motor3A, OUTPUT);
  pinMode(motor3B, OUTPUT);


//INDICAMOS LA FUNCCION SUN  PARA LOS DATOS GENERALES  
// COMPROBAR CON https://www.suncalc.org/#/38.0477,-1.1953,13/2023.01.12/08:18/1/0
SunPosition sun(55.75, 37.62, now.unixtime());
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


// Calculate the angle of incidence of sunlight on the solar panels
float sunAltitude = sun.altitude();
float angleDifference = sunAltitude + tilt;
Serial.println("The angle of Difference is:  " + String(angleDifference) + " degrees");

}

void loop() {




//
 // -------------------------------------------------------------------------------

if (minutesSinceMidnight >= (sun.sunrise() - 5) && minutesSinceMidnight < sun.sunrise()) {
    // rotate motor 2  forward to close the right clapan
      digitalWrite(motor2A, HIGH);
digitalWrite(motor2B, LOW);
delay(2000);  // wait for 5 seconds

    // rotate motor 1 back to close the  left clapan
      digitalWrite(motor1A, LOW);
digitalWrite(motor1B, HIGH);
delay(2000);  // wait for 5 seconds


// Stop the 2 motor  motor 
 digitalWrite(motor2A, LOW);
 digitalWrite(motor2B, LOW);
 delay(2000);  // wait for 5 seconds
 
// Stop the 1 motor  motor 
 digitalWrite(motor1A, LOW);
 digitalWrite(motor1B, LOW);
 delay(2000);  // wait for 5 seconds


// Rotate FORWARD main motor for  30 seconds 
digitalWrite(motor1A, HIGH);
digitalWrite(motor1B, LOW);
delay(30000);

// stop the  main motor
digitalWrite(motor1A, LOW);
digitalWrite(motor1B, LOW);
//print message
Serial.println("The system is up");
}

  // check if the current time is after sunrise but before solar noon
else if (minutesSinceMidnight >= sun.sunrise() && minutesSinceMidnight < sun.noon()) {
    // rotate main  motor backward every 10 minutes
if (minutesSinceMidnight % 10 == 0) {
 digitalWrite(motor3A, LOW);
digitalWrite(motor3B, HIGH);
delay(6000);

// stop the motor
digitalWrite(motor1A, LOW);
 digitalWrite(motor1B, LOW);

// print message
Serial.println("The system declined 2.5 degrees");
 }
}

// check if the current time is solar noon
 else if (minutesSinceMidnight == sun.noon()) {
 // stop the main motor
 digitalWrite(motor3A, LOW);
digitalWrite(motor3B, LOW);

 // rotate motor 2  forward to CLOSE the right clapan
      digitalWrite(motor2A, HIGH);
digitalWrite(motor2B, LOW);
delay(2000);  // wait for 5 seconds

    // rotate motor 1 BACK to OPEN the  left clapan
      digitalWrite(motor1A, LOW);
digitalWrite(motor1B, HIGH);
delay(2000);  // wait for 5 seconds


// Stop the 2 motor  motor 
 digitalWrite(motor2A, LOW);
 digitalWrite(motor2B, LOW);
 delay(2000);  // wait for 5 seconds
 
// Stop the 1 motor  motor 
 digitalWrite(motor1A, LOW);
 digitalWrite(motor1B, LOW);
 delay(2000);  // wait for 5 seconds
 // print message
Serial.println("The system is in its base position");


  }


// check if the current time is after solar noon but before sunset
 else if (minutesSinceMidnight > sun.noon() && minutesSinceMidnight < sun.sunset()) {
 // rotate main motor forward every 10 minutes
if (minutesSinceMidnight % 10 == 0) {
digitalWrite(motor1A, HIGH);
  digitalWrite(motor1B, LOW);
      // print message
 Serial.println("The system elevated by  2.5 degrees");
}
}


// check if the current time is solar noon
 else if (minutesSinceMidnight == sun.sunset()) {
 // stop the main motor
 digitalWrite(motor3A, LOW);
digitalWrite(motor3B, LOW);

 // rotate motor 2  forward to CLOSE the right clapan
 digitalWrite(motor2A, HIGH);
digitalWrite(motor2B, LOW);
delay(2000);  // wait for 5 seconds

// rotate motor 1 forward to close the  left clapan
 digitalWrite(motor1A, LOW);
digitalWrite(motor1B, LOW);
delay(2000);  // wait for 5 seconds


// Stop the 2 motor  motor 
 digitalWrite(motor2A, LOW);
 digitalWrite(motor2B, LOW);
 delay(2000);  // wait for 5 seconds
 
// Stop the 1 motor  motor 
 digitalWrite(motor1A, LOW);
 digitalWrite(motor1B, LOW);
 delay(2000);  // wait for 5 seconds
 // print message
Serial.println("The system isat sleep");


  }

//aici termina

}
