#include <Wire.h> // Including Wire library for the I2C Connected
to the LCD Display
#include <LiquidCrystal_I2C.h> // Including Liquid crystal library for I2C
#include "DHT.h" // Including DHT library for DHT11 Temperature
and Humidity Sensor
#include <LedControl.h>
#include <Servo.h>

// Including LED library for controlling 8X8 dot matrix using MAX7219
#define DHTPIN 7 // Defining the sensor pin of DHT11 sensor connected to Arduino
#define DHTTYPE DHT11 // Defining the type of DHT sensor, since this library is built also for DHT22

const int en = 2,rw = 1, rs = 0,d4 = 4,d5 = 5,d6 = 6,d7 = 7, bl = 3; // LCD pinouts for the I2C
const int i2c_addr = 0x27; // I2C Address, found using another programme
const int msensor = A1; // Moisture Sensor data pin Connected to analog pin A1 in Arduino
const int Vin = A0; // Pin to receive analog readings from the photoresistor
const int DIN = 10; // Data pin of the MAX7219 8X8 dot matrix controller
const int CS = 9; // Another Data pin of the MAX7219 8X8 dot matrix controller
const int CLK = 8; // Clock Data pin of the MAX7219 8X8 dot matrix controller
const int buzz = 12;
int pos = 0;

LedControl lc = LedControl(DIN, CLK, CS, 0); // Defininng pins to control the 8X8 dot matrix
LiquidCrystal_I2C lcd(0x27, 16, 2); // Defining LCD connections to I2C bus
DHT dht(DHTPIN, DHTTYPE); // Defining and Setting up DHT sensor
Servo myservo;
int msvalue = 0; // Initial Value of Moisture Sensor
int light = 0; // Initial value of voltage across photoresistor
float tempC = 0.0; // Assigning variable to calculate the Temperature score
float humC = 0.0; // Assigning variable to calculate the Humidity score
float msenC = 0.0; // Assigning variable to calculate the Moisture score
float lightC = 0.0; // Assigning variable to calculate the Sunlight score
float hum; // Stores humidity value in percent
float temp; // Stores temperature value in Celcius
float score = 0.0;

byte smile[8] = {0x3C,0x42,0xA5,0x81,0xA5,0x99,0x42,0x3C}; // Byte array to store the "Smile Expression"
byte neutral[8] = {0x3C,0x42,0xA5,0x81,0xBD,0x81,0x42,0x3C}; // Byte array to store the "Neutral Expression"
byte sad[8] = {0x3C,0x42,0xA5,0x81,0x99,0xA5,0x42,0x3C}; //Byte array to store the "Sad Expression"

void setup() {
Serial.begin(9600); //Setting up serial monitor
pinMode(msensor, INPUT); //giving analog pin address (A1) to Arduino for Moisture sensor
pinMode(Vin, INPUT); //giving analog pin address (A2) to Arduino for photoresistor
dht.begin(); //Initialize DHT-22
myservo.attach(13);
lcd.begin(); // Set display type as 16 char, 2 rows
lcd.backlight();
lc.shutdown(0, false); //Initializing the MAX7219 8X8 dot matrix controller
lc.setIntensity(0, 2); //Toggle to adjust LED brightness, maximum is 15
lc.clearDisplay(0); //Clearing any previous display from the dot matrix
lcd.setCursor(0, 0);
delay(2000);
lcd.print(" Welcome"); //Intro lines
lcd.setCursor(0, 1);
lcd.print(" to"); //Intro lines
delay(2000);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print(" Plant"); //Intro lines
lcd.setCursor(0, 1);
lcd.print(" Communicator"); //Intro lines
delay(2000);
lcd.clear();
lcd.setCursor(0, 0);
lcd.print(" Micro 3003"); //Intro lines
lcd.setCursor(0, 1);
lcd.print("J COMP "); //Intro lines
delay(2000);
lcd.clear();
//Serial.println("Connection to NODE MCU initiated");
}
void loop() {
hum = dht.readHumidity(); //Get Humidity value
temp = dht.readTemperature(); //Get Temperature value
//Serial.println(hum);
//Serial.println(temp);
lcd.clear(); //Clear the display
//Temperature Parameters
if (temp >= 18 && temp <= 26) {
lcd.setCursor(0, 0);
lcd.print("Temp: ");
lcd.print(temp);
lcd.print(" C");
lcd.setCursor(0, 1);
lcd.print(" PERFECT");
tempC = 3.0;
delay(2000);
} else if (temp >= 6 || temp <= 34) {
lcd.setCursor(0, 0);
lcd.print("Temp: ");
lcd.print(temp);
lcd.print(" C");
lcd.setCursor(0, 1);
lcd.print(" FINE");
tempC = 2.0;
delay(2000);
} else {
lcd.setCursor(0, 0);
lcd.print("Temp: ");
lcd.print(temp);
lcd.print(" C");
lcd.setCursor(0, 1);
lcd.print(" BAD");
tempC = 1.0;
delay(2000);
}
lcd.clear();
//Humidity Parameters
if (hum >= 50 && hum <= 60) {
lcd.setCursor(0, 0);
lcd.print("Humidity: ");
lcd.print(hum);
lcd.print("%");
lcd.setCursor(0, 1);
lcd.print(" PERFECT");
humC = 3.0;
delay(2000);
} else if (hum >= 40 || hum <= 80) {
lcd.setCursor(0, 0);
lcd.print("Humidity: ");
lcd.print(hum);
lcd.print("%");
lcd.setCursor(0, 1);
lcd.print(" FINE");
humC = 2.0;
delay(2000);
} else {
lcd.setCursor(0, 0);
lcd.print("Humidity: ");
lcd.print(hum);
lcd.print("%");
lcd.setCursor(0, 1);
lcd.print(" BAD");
humC = 1.0;
delay(2000);
}
lcd.clear();
//Soil Moisture Parameters
msvalue = analogRead(msensor); //Reading Value from Moisture Sensor //Get Soil Moisture values from
analog pin
//Serial.println(msvalue);
if (msvalue >= 200 && msvalue <= 600) {
lcd.setCursor(0, 0);
lcd.print(" Soil Moisture");
lcd.setCursor(0, 1);
lcd.print(" PERFECT");
msenC = 3.0;
delay(2000);
} else if (msvalue > 600) {
lcd.setCursor(0, 0);
lcd.print(" Soil Moisture");
lcd.setCursor(0, 1);
lcd.print(" LOW");
msenC = 1.0;
delay(2000);
} else {
lcd.setCursor(0, 0);
lcd.print(" Soil Moisture");
lcd.setCursor(0, 1);
lcd.print("Excess Water");
msenC = 2.0;
delay(2000);
}
lcd.clear();

//Sunlight Parameters
light = analogRead(Vin); //Reading Voltage across the photoresistor to calculate the light intensity
//Serial.print("The PhotoResistor value is:");
//Serial.println(light);
//Serial.println(Vin);
if (light >= 750) {
lcd.setCursor(0, 0);
lcd.print(" Sunlight");
lcd.setCursor(0, 1);
lcd.print(" PERFECT");
lightC = 3.0;
delay(2000);
} else if (light >= 300) {
lcd.setCursor(0, 0);
lcd.print(" Sunlight");
lcd.setCursor(0, 1);
lcd.print("Could be Better");
lightC = 2.0;
delay(2000);
} else {
lcd.setCursor(0, 0);
lcd.print(" Sunlight");
lcd.setCursor(0, 1);
lcd.print("Needs Sunlight!!");
lightC = 1.0;
delay(2000);
}
lcd.clear();

//All parameters have been checked and we now calculate the score
score = ((tempC + humC + msenC + lightC) / 4);
if (score >= 2.5) {
lcd.setCursor(0, 0);
lcd.print("Score: ");
lcd.print(score);
lcd.print("/3.00");
lcd.setCursor(0, 1);
lcd.print(" Plant is HAPPY "); //Print how the plant feels on bottom line
printByte(smile); //Display Plant's expression on the DOT Matrix
delay(3000);
} else if (score >= 2) {
lcd.setCursor(0, 0); //Print Caretaking score on top line
lcd.print("Score: ");
lcd.print(score);
lcd.print("/3.00");
lcd.setCursor(0, 1); //Print how the plant feels on bottom line
lcd.print(" Plant is Fine"); //Display Plant's expression on the DOT Matrix
myservo.write(0);
printByte(neutral);
delay(3000);
} else {
lcd.setCursor(0, 0); //Print Caretaking score on top line
lcd.print("Score: ");
lcd.print(score);
lcd.print("/3.00");
lcd.setCursor(0, 1); //Print how the plant feels on bottom line
lcd.print(" Plant is SAD"); //Display Plant's expression on the DOT Matrix
printByte(sad);
tone(buzz,261);
myservo.write(90); // tell servo to go to position in variable 'pos'
delay(15);
delay(500);
tone(buzz,440);
delay(500);
noTone(buzz);
delay(2000);
}
lcd.clear();
Serial.println(score);
}
void printByte(byte character[]) //Function, called to print Expressions on the DOT Matrix Display
{
int i = 0;
for (i = 0; i < 8; i++) {
lcd.setRow(0, i, character[i]);
}
