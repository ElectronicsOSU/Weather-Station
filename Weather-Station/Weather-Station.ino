//Pins used 2.1 -- SCL
//          2.2 -- SDL
//          GND -- GND
//          VCC -- VCC
//          2.3 -- Green Led
//          2.4 -- Yello Led
//          2.5 -- Red Led
// TUNING VARIABLE
// Range: 0 to 10
float scale = 0.9; // Increase for less lights, decrease for more lights

const int analogInPin = A0; // Analog input pin connected to P1_0 (ADC)
const int green_led = P2_3; // Green LED connected to P2_0
const int yellow_led = P2_4; // Yellow LED connected to P2_1
const int red_led = P2_5; // Red LED connected to P2_2

int voltage = 0;  // Read value from the analog input pin
#include <Wire.h>
#define button 5
#define LED 2
volatile int flag = HIGH;
int thermo_address = 72; //I2C Address of the device
byte read_temp_command = 0x00; //Command to be sent to the thermometer
void setup() {
  Serial.begin(9600); 
  Serial.println("Serial Started");
  Wire.begin(); //Start wire library
  Serial.println("Wire Started");
  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);
  
  // Declare the 3 LED pins as outputs
  pinMode(green_led, OUTPUT);
  pinMode(yellow_led, OUTPUT);
  pinMode(red_led, OUTPUT);
  
  // Initialize the LEDs to be off
  digitalWrite(green_led, LOW);
  digitalWrite(yellow_led, LOW);
  digitalWrite(red_led, LOW);
}

void loop() {
  //Start Transmission
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
  int c = Wire.read(); 

  //Print info to the user
  Serial.print("Temperature: ");
  Serial.print(c);
  Serial.print("\n");
  
  //Delay between readings
  voltage = analogRead(analogInPin);
  
  
  if (voltage > scale*100) // Light all 3 LEDs
  {
    digitalWrite(green_led, HIGH);
    digitalWrite(yellow_led, HIGH);
    digitalWrite(red_led, HIGH);
  }
  else if (voltage > scale*66) // Light 2 LEDs (green/yellow)
  {
    digitalWrite(green_led, HIGH);
    digitalWrite(yellow_led, HIGH);
    digitalWrite(red_led, LOW);
  }
  else if (voltage > scale*33) // Light 1 LED (green)
  {
    digitalWrite(green_led, HIGH);
    digitalWrite(yellow_led, LOW);
    digitalWrite(red_led, LOW);
  }
  else // Turn off all 3 LEDs
  {
    digitalWrite(green_led, LOW);
    digitalWrite(yellow_led, LOW);
    digitalWrite(red_led, LOW);
  }

}

