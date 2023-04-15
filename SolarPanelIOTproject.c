#include <Servo.h> //defining library for Servos
#include<LiquidCrystal.h> //defining library for lcd display

//defining lcd. This code initializes the LCD display by creating an object of the LiquidCrystal_I2C class. The parameters passed to the class constructor are the I2C address of the LCD, the number of columns, and the number of rows.
LiquidCrystal_I2C lcd(0x27,16,2); 

//calculating the light intensity
double intensity(int ldrval){
  		double vout=ldrval*0.0048828125;   //to convert a raw ADC reading from an LDR sensor into a corresponding value of illuminance (brightness).conversion factor 0.0048828125, which is the reciprocal of 2^8 (since ADC0 is presumably an 8-bit value).
  		int lux=((2500/vout-500)/10); //formula for calculating the illuminance in lux
  		return lux;
	}
//end

//defining values for voltage sensor
const int voltageSensor = A0; //initialize the value of A0 pin
float vOUT = 0.0; // is used to store the voltage reading from an analog pin of the Arduino.
float vIN = 0.0; // used to store the calculated input voltage from the solar panel circuit.
float R1 = 30000.0; //represent the resistance of resistor in voltage driver circuit
float R2 = 7500.0; //represent the resistance of resistor in voltage driver circuit
int value = 0; //used to store the raw analog reading from the solar panel

//defining values for servo 
int servoverti;
int servovertical = 0;  //current position of servo with initial position 0;
int servovLimitHigh = 180; //initializes it with the value 180. This variable is used to define the maximum limit of the servo motor's vertical movement. 
int servovLimitLow = 20;  //initializes it with the value 20. This variable is used to define the minimum limit of the servo motor's vertical movement.

void setup () 
{

lcd.init();  // initializes the LCD display
lcd.backlight();  // turns on the backlight of the LCD display.
servoverti.attach(9);   //servo attached to digital pin 9
servoverti.write(0);  //initial position of servo motor to 0
delay(50);  
Serial.begin(9600); //starts serial communication with baud rate 9600

}

void loop()
{ 
int ldr1=analogRead(A1);  //ldr attached to analog 1
  int ldr2=analogRead(A3);  //ldr attached to analog 3

  servovertical = servoverti.read();  // the current position of the servo motor is read and stored in the variable “servovertical”.  
// calculating analog average values of each LDR  
  int avgldr1 = ldr1; 
  int avgldr2 = ldr2;  
  
// calculating which direction servo to move 
    if (avgldr1 < avgldr2) 
    {
      // it means that the second light source is brighter, so the servo motor is instructed to move one step up by incrementing its current angle by 1
      servoverti.write(servovertical +1); 
      if(servovertical > servovLimitHigh) 
      {
// the code checks whether the new angle would exceed the upper limit for the servo, “servovLimitHigh”. If it would, the angle is set to the maximum value with
        servovertical = servovLimitHigh; 
      }
      delay(1);
    }
    else if (avgldr2 < avgldr1) 
    {
	// it means that the first light source is brighter, so the servo motor is instructed to move one step down by decrementing its current angle by 1
      servoverti.write(servovertical -1);
      if (servovertical < servovLimitLow) 
      {
	// the code checks whether the new angle would go below the lower limit for the servo, “servovLimitLow”. If it would, the angle is set to the minimum value
        servovertical = servovLimitLow; 
      }
      delay(1); 
    }
    else
    {
// If the two light sources have the same average brightness, the servo angle is not changed, and the current angle is maintained
      servoverti.write(servovertical); 
    }
  
//calculating the voltage from the solar panel
    value = analogRead(voltageSensor);  // reads the voltage value
    vOUT = (value * 5.0) / 1024.0;  // calculates the output voltage
    vIN = vOUT / (R2/(R1+R2));   // calculates the input voltage
    delay(50); 
  
//printing the values in lcd display
    lcd.setCursor(0,0);       // sets the cursor position to the beginning of the first row.   
    lcd.print("intensity:");   // prints the string "intensity:" on the first row.
    lcd.print(int(intensity(ldr1)));  // prints the integer value of the intensity of the first LDR sensor on the first row, right after the string "intensity:”.
    lcd.setCursor(0,1);   // sets the cursor position to the beginning of the second row.
    lcd.print("voltage : ");
    lcd.print(vIN);    // prints the value of the voltage read from the voltage sensor on the second row, right after the string "voltage : ".

//Printing the value in serial monitor 
  Serial.print("Light intensity : ");
  Serial.print(int(intensity(ldr1)));
  Serial.print(" lux \n");
  delay(100);
  Serial.print("voltage : ");
  Serial.print(vIN);
  Serial.print("\n");
}