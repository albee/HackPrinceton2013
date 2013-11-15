#include "Arduino.h"
#include <Servo.h> 

//Range-finding funcion provided by ultrasonic sensor manufacturer.
 
Servo sweeper;
Servo sound;

int sens = 5;
int sensout = 10;
int pos = 30;
int cntr = 0;
int state = 0;
int dist = 80;
long RangeInCentimeters;
int start_pos = 30;
int end_pos = 140;

class Ultrasonic
{
	public:
		Ultrasonic(int pin);
        void DistanceMeasure(void);
		long microsecondsToCentimeters(void);
	private:
		int _pin;
        long duration;
};
Ultrasonic::Ultrasonic(int pin)
{
	_pin = pin;
}

/*Begin the detection and get the pulse back signal*/
void Ultrasonic::DistanceMeasure(void)
{
    pinMode(_pin, OUTPUT);
	digitalWrite(_pin, LOW);
	delayMicroseconds(2);
	digitalWrite(_pin, HIGH);
	delayMicroseconds(5);
	digitalWrite(_pin,LOW);
	pinMode(_pin,INPUT);
	duration = pulseIn(_pin,HIGH,38000);
}

/*The measured distance from the range 0 to 400 Centimeters*/
long Ultrasonic::microsecondsToCentimeters(void)
{
	return duration/29/2;	
}
 
Ultrasonic ultrasonic(7);

void setup()
{
    sweeper.attach(9);
    sound.attach(11);
    Serial.begin(9600);
    sound.write(72);
    pinMode(13,OUTPUT);
    digitalWrite(13,LOW);
}

/*Loop through rotation cycle*/
void loop()
{	
/*First swivel 180 for loop*/
    for(pos = start_pos; pos < end_pos; pos += 1) 
  {                
      ultrasonic.DistanceMeasure();
      RangeInCentimeters = ultrasonic.microsecondsToCentimeters();
      if (state == 0) {
        if (cntr < sens) {
          sweeper.write(pos); 
          delay(5);        
          }
         if (cntr >= sens) {
           sound.write(38);
           digitalWrite(13,HIGH);
           delay(500);
           sound.write(65);
           delay(1000);
           state = 1;
           cntr = 0;
      }
          if (0 < RangeInCentimeters && RangeInCentimeters < dist) {
            cntr = cntr + 1;
       }
          if (RangeInCentimeters >= dist) {
             cntr = 0;}
     }
       else {
           if (cntr < sensout) {
       }
           if (cntr >= sensout) {
              digitalWrite(13,LOW);
              state = 0;
             cntr = 0;
      }
      if (RangeInCentimeters > dist) {
         cntr = cntr + 1; 
       }
      if (RangeInCentimeters <= dist && !(RangeInCentimeters == 0)) {
         cntr = 0;}
     }
   }
     
/*Swivel back 180 in opposite direction*/
  for(pos = end_pos; pos > start_pos; pos -= 1) 
  {    
      ultrasonic.DistanceMeasure();
      RangeInCentimeters = ultrasonic.microsecondsToCentimeters();
       if (state == 0) {
           if (cntr < sens) {
              sweeper.write(pos);
              delay (5);         
          }
       if (cntr >= sens) {
         digitalWrite(13,HIGH);
         sound.write(38);
         delay(500);
         sound.write(65);
         delay(1000);
         state = 1;
         cntr = 0;
      }
      if (0 < RangeInCentimeters && RangeInCentimeters < dist) {
        
         cntr = cntr + 1; 
       }
      if (RangeInCentimeters >= dist) {
         cntr = 0;}
       }  
   else {
       if (cntr < sensout) {
         }
       if (cntr >= sensout) {
         digitalWrite(13,LOW);
         state = 0;
         cntr = 0;
      }
      if (RangeInCentimeters > dist) {
          cntr = cntr + 1; 
       }
    if (RangeInCentimeters <= dist && !(RangeInCentimeters == 0)) {
         cntr = 0;}
     }
  }  
}
