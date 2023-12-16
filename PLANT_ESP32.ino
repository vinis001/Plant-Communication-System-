#include "WiFi.h"
#include <ThingSpeak.h>;
#define RXp2 16
#define TXp2 17
WiFiClient client;

char* ssid = "Micro"; //Enter SSID
char* password = "1234567*()"; //Enter Password

unsigned long myChannelNumber = 1927063; //Your Channel Number (Without Brackets)
char * myWriteAPIKey = "M221Z4AHV6N1Z3WJ"; //Your Write API Key

String a;
float dat = 0.0;
void setup() {
// put your setup code here, to run once:
Serial.begin(115200);
Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED)
{
delay(500);
Serial.print("*");
}
ThingSpeak.begin(client);
Serial.println("");
Serial.println("WiFi connection Successful");
Serial.print("The IP Address of ESP32 Module is: ");
Serial.println(WiFi.localIP());// Print the IP address
}
void loop() {

if(Serial2.available() > 0) {
a = Serial2.readString();
Serial.println("Value Fetched");
dat = a.toFloat();
Serial.println(dat);
}
ThingSpeak.writeField(myChannelNumber, 1,dat, myWriteAPIKey); //Update in ThingSpeak
delay(4000);
}
