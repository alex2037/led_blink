//HARDWARE PINOUT PIN 6 -> 220ohm -> RED LED anode, GND -> RED LED catode

#include "ReedSolomon.h"

#define LEDPIN 6
#define PWM 127
#define DELAY 200
#define LED_START_STATE 0

void setup() {
  pinMode(LEDPIN, OUTPUT);
  Serial.begin(115200);
  digitalWrite(LEDPIN, LED_START_STATE);
}


uint8_t buffer[13] = {
  0, 0, 0, 0, 0, 0, 0, 0, // 8 байтов служебной информации
  0, 0, 0, 0, 0   // это полезная информация
};

void code_info()
{
  ReedSolomonEncoding::rsEncode(buffer, 4, 13);
}

void transmit_info()
{

  for(int i=0; i<13; i++)
  {
    for(int j=0; j<8; j++)
    {
      if(bitRead(buffer[i],j)) digitalWrite(LEDPIN,HIGH);
      delay(DELAY);
      digitalWrite(LEDPIN, LOW);
    }
  }

}


void DECODE()
{
  const int errorsCorrected = ReedSolomonEncoding::rsDecode(buffer, 4, 13);
  for(int i=8; i<13; i++)
      {

        Serial.print((char)buffer[i]);
      }
      Serial.println("--------------------");
}


void erase_send_buffer()
{
  for(int i = 0; i<13; i++)
  {
    buffer[i] = 0;
  }
}

String STR;

void loop() {
  String tempstr = "";
  while (Serial.available() > 0) {
    STR = Serial.readString();
    for(int i=0; i < STR.length(); i++)
    {
      if(((byte)STR[i]>96)&&((byte)STR[i]<123))
      {
        tempstr+=STR[i];
        if (tempstr.length() == 5)
        {
          for(int i=8; i<13; i++)
          {
            buffer[i]=tempstr[i-8];
          }
          code_info();
          transmit_info();
          erase_send_buffer();
          tempstr="";
        }
      }
    }
    for(int i=0; i<tempstr.length(); i++)
    {
      buffer[i+8]=tempstr[i];
    }
    code_info();
    transmit_info();
    erase_send_buffer();
  } 
}
