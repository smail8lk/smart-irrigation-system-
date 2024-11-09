#include <Servo.h>
#include <Wire.h> // I2C library to deal with LCD i2c module
#include <LiquidCrystal_I2C.h> // Driver Library for the LCD Module


// Connections
int trigPin =6; // ultrasonic
int echoPin= 4; 
int LED =7; 

int rain_sensor_pin = 48;
int tap_servo_pin = 5; 

int VANNE_P1 =24; // Valves
int VANNE_P2 =26 ;
int VANNE_P3 =28 ; 


int POMPE = 10 ;
int BUZZER = 9;

int BLUELED = 30;
int GREENLED = 32;
int REDLED = 34;


#define ldr A1 // LDR sensor

#define An1 A0 // def d'humidite plantes 1 2 3 
#define An2 A2
#define An3 A3


// Objects (Classes)
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo tap_servo;


// Variables
int outputValue1 ;
int outputValue2 ;
int outputValue3 ;

int value_1=0,mesure_1=0;
int value_2=0,mesure_2=0;
int value_3=0,mesure_3=0;

int valueldr;
int val;
long duration, distance;
int n;
int servo_state;

unsigned long last_millis;

void setup() {

tap_servo.attach(tap_servo_pin);
tap_servo.write(0); // starting Angel

pinMode(rain_sensor_pin,INPUT); // init I/O pins
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
pinMode(BLUELED,OUTPUT);
pinMode(GREENLED,OUTPUT);
pinMode(REDLED,OUTPUT);
pinMode(VANNE_P1,OUTPUT);
pinMode(VANNE_P2,OUTPUT);
pinMode(VANNE_P3,OUTPUT);
pinMode(POMPE,OUTPUT);
pinMode(BUZZER,OUTPUT);

digitalWrite(BLUELED,LOW);   // Clear I/O pins
digitalWrite(GREENLED,LOW);
digitalWrite(REDLED,LOW);
digitalWrite(POMPE,LOW);
digitalWrite(VANNE_P1,LOW);
digitalWrite(VANNE_P2,LOW); 
digitalWrite(VANNE_P3,LOW);
digitalWrite(BUZZER,HIGH);  delay(50);   // peeb peeb peeb peeb
digitalWrite(BUZZER,LOW); delay(50); 
digitalWrite(BUZZER,HIGH);  delay(50); 
digitalWrite(BUZZER,LOW); delay(50); 
digitalWrite(BUZZER,HIGH);  delay(50); 
digitalWrite(BUZZER,LOW); delay(50); 
digitalWrite(BUZZER,HIGH);  delay(50); 
digitalWrite(BUZZER,LOW); delay(50); 

  delay(4000);
 
digitalWrite(BUZZER,HIGH);  delay(50);   // peeb peeb 
digitalWrite(BUZZER,LOW); delay(50); 
digitalWrite(BUZZER,HIGH);  delay(50); 
digitalWrite(BUZZER,LOW); delay(50); 
   

last_millis = millis();


Serial.begin(9600); 
  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the LCD backlight
  lcd.begin(16,2);
  for(int i= 0; i<= 3; i++)
{ 
  lcd.clear(); 
  lcd.setCursor(i,0); 
  lcd.print("Welcome to");
  delay(300); 

  lcd.setCursor(0,1);
  lcd.print("   EST MEKNES");
 }
  delay(5000);
    for(int i= 0; i<= 3; i++)
{ 
  lcd.clear(); 
  lcd.setCursor(i,0); 
  lcd.print("electrovert");
  delay(300); 

  lcd.setCursor(0,1);
  lcd.print("   represente");
 }
  delay(5000);
lcd.clear(); 
 }
void loop() {

measure_ultrasonic();

if(distance > 20){ // turn buzzer on when the water level is low (so the distance between sensor and water more than 20 cm)
    digitalWrite(LED ,HIGH);
    digitalWrite(BUZZER,HIGH);
  }
else {
    digitalWrite( LED,LOW);
    digitalWrite(BUZZER,LOW);
  }

val = digitalRead(rain_sensor_pin); // measure rain sensor on D0 pin
if (val==1 && servo_state==1) servo_open(); // dry
else   if (val==0 && servo_state==0) servo_close(); // rain

measure_plantes(); // soil moisture sensors

  if(mesure_1<50){ 
    digitalWrite(BLUELED,HIGH);
    digitalWrite(POMPE,HIGH);
    digitalWrite(VANNE_P1,HIGH);
  }
  else {
    digitalWrite(BLUELED,LOW);
    digitalWrite(POMPE,LOW);
    digitalWrite(VANNE_P1,LOW);    
  }

  if(mesure_2<40){
    digitalWrite(GREENLED,HIGH);
    digitalWrite(POMPE,HIGH);
    digitalWrite(VANNE_P2,HIGH);
  }
  else{
    digitalWrite(GREENLED,LOW);
    digitalWrite(POMPE,LOW);
    digitalWrite(VANNE_P2,LOW);
  }

  if(mesure_3<60){
    digitalWrite(REDLED,HIGH);
    digitalWrite(POMPE,HIGH);
    digitalWrite(VANNE_P3,HIGH);
  }
  else{
    digitalWrite(REDLED,LOW);
    digitalWrite(POMPE,LOW);
    digitalWrite(VANNE_P3,LOW);
  }   

if(millis() > (last_millis + 4000)){ // 4000 is the time for each sensor to display ( 4 seconds)

if(n<2) n++; // counter for 3 screens
else n=0;
LCD_display(n);
last_millis = millis() ;
}

value_display(n);
delay(500);
}

void LCD_display(int k){
lcd.setCursor(0,0);

if(k==0)      lcd.print(" Sensor 1 Value ");
else if(k==1) lcd.print(" Sensor 2 Value ");
else if(k==2) lcd.print(" Sensor 3 Value ");
}

void value_display(int j){
lcd.setCursor(4,1);

if(j==0)      lcd.print(mesure_1);
else if(j==1) lcd.print(mesure_2); 
else if(j==2) lcd.print(mesure_3);

lcd.print(" %");
}

void measure_ultrasonic(){
  digitalWrite(trigPin, LOW);        
  delayMicroseconds(2);              
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);           
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  Serial.print("distance = ");
  Serial.print(distance);
  Serial.println(" cm");
  
}

void measure_plantes(){
value_1=analogRead(An1); // get data from 0 to 1023
value_2=analogRead(An2);
value_3=analogRead(An3);

mesure_1=map(value_1,1023,20,0,100); // scale data to percentage values 0 to 100%
mesure_2=map(value_2,1023,300,0,100);
mesure_3=map(value_3,1023,300,0,100);

// print on Serial monitor
Serial.print("l'humidité de sol 1 = ");
Serial.print(mesure_1);
Serial.print(" %     2 = ");
Serial.print(mesure_2);
Serial.print(" %     3 = ");
Serial.println(mesure_3);

}

void servo_close(){
servo_state = 1;
Serial.println("Rain weather --- Closing servo motor");
for(int i =0; i<180 ; i++)
{tap_servo.write(i);
 delay(25); // this delay determine closing speed (higher delay slower closing)
}
}

void servo_open(){
servo_state = 0;
Serial.println("dry weather ---  opening servo motor");
for(int i =180; i>0 ; i--)
{tap_servo.write(i);
 delay(10); // this delay determine opening speed (higher delay slower opening)
}


}
