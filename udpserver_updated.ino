/*
 * Modified by Wasee Ahmed(waseeahmed@live.com)
 * Copyright (C) 2013 by Norbert Truchsess (norbert.truchsess@t-online.de)
 * 
 * This UdpServer example sets up a udp-server at 192.168.0.2 on port 5000.
 * send packet via upd to test
 * 
 * Mac Address = 0x01 0x02 0x03 0x04 0x05 0x06
 * IP Address = 192.168.0.109
 * Port = 5000
 */

#include <UIPEthernet.h>
#include <LiquidCrystal.h>
EthernetUDP udp;
const int rs = 6, en = 7, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
boolean displayCountFlag = true;
int displayCount = 0;
void setup()
{
  Serial.begin(9600);
  uint8_t mac[6] = {0x01,0x02,0x03,0x04,0x05,0x06};
  Ethernet.begin(mac,IPAddress(192,168,0,109));
  udp.begin(5000);
  lcd.begin(16, 2);
  lcd.print("Micro 1:");
}

void loop()
{
  lcd.setCursor(8, 0);
  lcd.print(millis() / 1000);
  lcd.print("s");
  int packetSize = udp.parsePacket();//check for new udp-packet:
  if (packetSize > 0) 
  {
    do
    { 
      if(displayCountFlag)
      {
        lcd.setCursor(0, 1);
        lcd.print(displayCount);
        lcd.print(":");
        displayCountFlag = false;
        displayCount++;
      }
      char* msg = (char*)malloc(packetSize+1);//Memory Allocation
      int len = udp.read(msg,packetSize+1);//length of the massage
      msg[len]=0;//NULL char for print() termination
      lcd.print(msg);
      Serial.print(msg);//Print Massage
      free(msg);//Free memory
    }
    while((packetSize = udp.available())>0);
    udp.flush();
    int success;
    do{success = udp.beginPacket(udp.remoteIP(),udp.remotePort());}
    while(!success);
    udp.println("Hi Micro2");
    udp.endPacket();
    udp.stop();
    udp.begin(5000);
  }
  displayCountFlag = true;
}
