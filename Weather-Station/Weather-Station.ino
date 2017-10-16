/**THIS CODE WILL NOT WORK WITH ENERGIA VERSION <18!!!**/
/**TOO ALLOW IT TO WORK WITH VERSION <18 YOU MUST CHANGE THE PIN DEFINITIONS FOR I2C TO 1.7/1.6 AND MOVE THE BCD ENCODER PINS AS NECESSARY**/

//Pins used 2.1 -- SCL
//          2.2 -- SDL
//          GND -- GND
//          VCC -- VCC
//          1.0 -- UV INDICATOR
//          1.6 -- TEMP INDICTATOR
//          1.1 -- Analog Read
//          1.2 -- A1
//          2.3 -- B1
//          1.4 -- C1
//          1.5 -- D1
//          2.0 -- A2
//          1.7 -- B2
//          2.5 -- C2
//          2.4 -- D2  
//          1.3 -- Push Button
#include <Wire.h>
//#include<driverlib.h>
//#define a_1 P1_2
//#define b_1 P2_3
//#define c_1 P1_4
//#define d_1 P1_5

//#define a_2 P2_0
//#define b_2 P1_7
//#define c_2 P2_5
//#define d_2 P2_4

//#define pushbutton P1_3
//#define analogInPin P1_1
// TUNING VARIABLE
// Range: 0 to 10
float scale = 0.9; // Increase for less lights, decrease for more lights

int voltage = 0;  // Read value from the analog input pin
int disp_value = 99; //Value to display on the 7-seg-display

int a_1 = P1_2;
int b_1 = P2_3;
int c_1 = P1_4;
int d_1 = P1_5;

int a_2 = P2_0;
int b_2 = P1_7;
int c_2 = P2_5;
int d_2 = P2_4;

int UVInd = P1_0;
int TEMPInd = P1_6;
int pushbutton = P1_3;
int analogInPin = P1_1;

volatile int flag = LOW;
volatile int current_mode = TEMPInd;
int thermo_address = 72; //I2C Address of the device
byte read_temp_command = 0x00; //Command to be sent to the thermometer
void setup() {
  delay(100);
  Wire.begin(); //Start wire library
  delay(100);
  pinMode(a_1, OUTPUT);
  pinMode(b_1, OUTPUT);
  pinMode(c_1, OUTPUT);
  pinMode(d_1, OUTPUT);
  pinMode(a_2, OUTPUT);
  pinMode(b_2, OUTPUT);
  pinMode(c_2, OUTPUT);
  pinMode(d_2, OUTPUT);
  pinMode(UVInd, OUTPUT);
  pinMode(TEMPInd, OUTPUT);
  digitalWrite(TEMPInd,HIGH);
  digitalWrite(UVInd,LOW);
  pinMode(pushbutton,INPUT_PULLUP);
  attachInterrupt(pushbutton,changeread,FALLING);
  
}

void loop() 
{
  digitalWrite(current_mode,LOW);
  delay(500);
  if(flag == LOW)
  {
    
    //change_display(1);
    Wire.beginTransmission(thermo_address);
    //Write the command to the thermometer
    Wire.write(read_temp_command);
   //End the transmission
    Wire.endTransmission();

    //Request 1 byte from the thermo_address
    Wire.requestFrom(thermo_address, 1);

    //While there is no info available
    while (Wire.available() == 0);

    //Temperature that is read in celsius
    disp_value = Wire.read(); 
  }
  if(flag == HIGH)
  {
     disp_value= analogRead(analogInPin);
     disp_value = disp_value / 11; //Make 1023 map to below 99
  }
  change_display(disp_value);
}
/**Change what the thing will read**/
void changeread()
{
  
  if(flag==HIGH)
  {
    flag = LOW;
    digitalWrite(TEMPInd,HIGH);
    digitalWrite(UVInd,LOW);
    current_mode = TEMPInd;
  }
  else if(flag==LOW)
  {
    flag = HIGH;
    digitalWrite(TEMPInd,LOW);
    digitalWrite(UVInd,HIGH);
    current_mode = UVInd;
  }
}
void change_display(int disp_value)
{
  int tens = get_tens(disp_value);
  int ones = get_ones(disp_value);
  set_display_pins(a_1,b_1,c_1,d_1,ones);
  set_display_pins(a_2,b_2,c_2,d_2,tens);
  digitalWrite(current_mode,HIGH);
  delay(500);
}
int get_tens(int value)
{
  int result = 0;
  while(value>9)
  {
    value -= 10;
    result = result + 1;
  }
  return result;
}
int get_ones(int value)
{
  while(value>9)
  {
    value -= 10;  
  }
  return value;
}
void set_display_pins(int a,int b,int c, int d,int value)
{
  if(value == 0)
  {
    digitalWrite(a,LOW);
    digitalWrite(b,LOW);
    digitalWrite(c,LOW);
    digitalWrite(d,LOW);
  }
  else if(value == 1)
  {
    digitalWrite(a,HIGH);
    digitalWrite(b,LOW);
    digitalWrite(c,LOW);
    digitalWrite(d,LOW);
  }
  else if(value == 2)
  {
    digitalWrite(a,LOW);
    digitalWrite(b,HIGH);
    digitalWrite(c,LOW);
    digitalWrite(d,LOW);
  }
  else if(value == 3)
  {
    digitalWrite(a,HIGH);
    digitalWrite(b,HIGH);
    digitalWrite(c,LOW);
    digitalWrite(d,LOW);
  }
  else if(value == 4)
  {
    digitalWrite(a,LOW);
    digitalWrite(b,LOW);
    digitalWrite(c,HIGH);
    digitalWrite(d,LOW);
  }
  else if(value == 5)
  {
    digitalWrite(a,HIGH);
    digitalWrite(b,LOW);
    digitalWrite(c,HIGH);
    digitalWrite(d,LOW);
  }
  else if(value == 6)
  {
    digitalWrite(a,LOW);
    digitalWrite(b,HIGH);
    digitalWrite(c,HIGH);
    digitalWrite(d,LOW);
  }
  else if(value == 7)
  {
    digitalWrite(a,HIGH);
    digitalWrite(b,HIGH);
    digitalWrite(c,HIGH);
    digitalWrite(d,LOW);
  }
  else if(value == 8)
  {
    digitalWrite(a,LOW);
    digitalWrite(b,LOW);
    digitalWrite(c,LOW);
    digitalWrite(d,HIGH);
  }
  else if(value == 9)
  {
    digitalWrite(a,HIGH);
    digitalWrite(b,LOW);
    digitalWrite(c,LOW);
    digitalWrite(d,HIGH);
  }
  else
  {
    digitalWrite(a,LOW);
    digitalWrite(b,LOW);
    digitalWrite(c,HIGH);
    digitalWrite(d,LOW);
  }
  
  return;
}



