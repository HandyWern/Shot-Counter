/////////////////////////////////
//      Shot_Counter v1.0      //
//   Replacement for EPS 711   //
/////////////////////////////////

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

byte trigger;          // Set to TRUE if the interrupt has triggered at least once since the last loop
volatile int count;
unsigned long t0, t1;  // The time in milliseconds that the interrupt fired q
float rpm;
int shots;
int tray = 4;


void setup(){

 Serial.begin(9600);
 lcd.begin();
 lcd.backlight();
 attachInterrupt(0, rpm_fun, RISING);  // to pin2 of the Arduino
 attachInterrupt(1, counter, HIGH);  // to pin3 of the Arduino
 pinMode(tray,OUTPUT);
 digitalWrite(tray,LOW); 
 
 t0 = 0;  // Previous time = 0
 t1 = 0;  // This time = 0
 trigger = 0;
 count = 0;
 shots = 0;
}


void loop() {
 
 float g = 0;

 if (trigger != 0){
   g = t1 - t0; // If the interrupt has been fired, work out the new RPM
   rpm = (1/g) * 60000000; //g = g/1000; // Convert to milliseconds
   trigger = 0;  // Reset the interrupt
 }

 if (g == 0){
  rpm = 0;
 }

 shots_pos();
 if (count>=shots){
  digitalWrite(tray,LOW);
  count = 0;
 }
  
 // Print RPM every second, or thereabouts, provided there's at least one average
 Serial.print("RPM: ");
 Serial.println(rpm);
 Serial.println(count);
 Serial.println(shots);

 // Print data to LCD
 lcd.clear();
 lcd.setCursor(3,0);
 lcd.print("RPM: ");
 lcd.print(rpm);
 lcd.setCursor(3,1);
 lcd.print("Shots: ");
 lcd.print(shots);
  
 // Wait around 1s
 delay(500);
}


void rpm_fun(){

 // Each rotation, this interrupt function is run twice
 // (is it?? Why? A: It's not (necessarily), it depends on the magnet.
 if (t0 == 0)
   t0 = micros();
 else {
   t0 = t1;
   t1 = micros();
   trigger = 1;
   count++;
 }
}


void counter(){
  
  count = 0;
  digitalWrite(tray, HIGH);
}


void shots_pos(){
  
  byte pos_1 = 5;
  byte pos_2 = 6;
  byte pos_3 = 7;
  byte pos_4 = 8;
  
  if (digitalRead(pos_1) == HIGH){
    shots = 50;
  }
  if (digitalRead(pos_2) == HIGH){
    shots = 100;
  }
  if (digitalRead(pos_3) == HIGH){
    shots = 150;
  }
  if (digitalRead(pos_4) == HIGH){
    shots = 200;
  }
}
 
