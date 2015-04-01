// evoRGBv3
// Testing EVOthings
// Thomas Olson
// teo20150303.02
// teo20150307.01 evoRGBv3
// Add sending status of LED states over BLE still using buf[0].
// For this application the LEDs SHOULD NOT be turned off with a
// disconnect from BLE.
#include <RFduinoBLE.h>
#define LED_RED 2
#define LED_GREEN 6
#define LED_BLUE 4
#define ButtonA 5
#define ButtonB 3

// Use buf[0] for status of Buttons A and B and LEDs
// 0x01 is Button A
// 0x02 is Button B
// 0x10 is LED_RED
// 0x20 is LED_GREEN
// 0x40 is LED_BLUE
int btnAbaby = 0;
int last = 0;
char buf[20] = {};
int millisA;
int millisB;
int diff;
int count;
int sum;
int avg;

void setup()
{
  Serial.begin(9600);
  Serial.println("EVOthings Testing - evoRGBv3");

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(ButtonA, INPUT);
  pinMode(ButtonB, INPUT_PULLDOWN);

  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);
  

  RFduinoBLE.begin();
}


void loop()
{ 
//buf[0] = (buf[0] & 0xFC) | (0x03 & (unsigned byte)readButtons());
  //Serial.println(buf[0],HEX);

  // Eventhough I send the whole 20 byte packet one is all that
  // is used by this APP. The purpose is to update APP of status
  // of Buttons and LEDs.
  RFduinoBLE.sendInt(readButtons()); //For this APP only need send 1.
  Serial.println(avg);
  RFduino_ULPDelay(10);
}


void RFduinoBLE_onConnect()
{
  Serial.println("RFduino BLE connected");
}

void RFduinoBLE_onDisconnect()
{
  Serial.println("RFduino BLE disconnected");
  // DO NOT TURN OFF LEDs on DISCONNECT.
}

void RFduinoBLE_onReceive(char *data, int len)
{
// Print the value receieved in first byte of packet. This APP only
// expecting to see one byte encoded with the LED ON/OFF commands.
  Serial.print("DATA[0]: ");
  Serial.println(data[0],HEX);

  switch(data[0]){
    case 1:
      analogWrite(LED_RED,0x2c); // Brightness of LED
      buf[0] = buf[0] | 0x10;
      break;
    case 2:
      analogWrite(LED_RED,0x00); // Brightness to OFF
      buf[0] = buf[0] & 0xEF;
      break;      
    case 3:
      analogWrite(LED_GREEN,0x2c);
      buf[0] = buf[0] | 0x20;
      break;
    case 4:
      analogWrite(LED_GREEN,0x00);
      buf[0] = buf[0] & 0xDF;
      break;
    case 5:
      analogWrite(LED_BLUE,0x2c);
      buf[0] = buf[0] | 0x40;
      break;  
    case 6:
      analogWrite(LED_BLUE,0x00);
      buf[0] = buf[0] & 0xBF;
      break;
    default:
      break;
  }

}

/*
In this example the RGB Button shield buttons are encoded
into first byte of send packet.
0x01 is button A
0x02 is button B
0x03 is both buttons
*/

int readButtons(){
int btnB;

  btnB = analogRead(ButtonB); 
 //Serial.println(btnB); 
  if(btnB >= 500 && last == 0)
   {last = 1;
   btnAbaby = btnAbaby + 1;
   millisA = millis() ;
     }
   else if(btnB <= 500 && last == 1)
   {last = 0;
   millisB = millis();
   diff = millisB - millisA;
   count = count + 1;
   sum = sum + diff;
   avg = sum/count;
   Serial.println(avg);
   }
  
  return btnAbaby;
}

