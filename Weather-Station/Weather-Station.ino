//Pins used 2.1 -- SCL
//          2.2 -- SDL
//          GND -- GND
//          VCC -- VCC

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
  delay(250);

}

