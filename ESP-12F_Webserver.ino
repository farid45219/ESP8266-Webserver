
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "wifi_mqqt.h"

WiFiClient   espClient;
PubSubClient client(espClient);
uint32_t     data_packet_counter=0;


void setup() {
  WIFI_Init();
  Serial.begin(9600);
  client.setServer(MQQT_SERVER_IP_ADDR, MQQT_SERVER_PORT);
  client.setBufferSize(2000);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  delay(5000);
  WIFI_Connect();
}

void loop() {
  
  mqqt_msg[0] ='T' ;
  mqqt_msg[1] ='e' ;
  mqqt_msg[2] ='m' ;
  mqqt_msg[3] ='p' ;
  mqqt_msg[4] =' ' ;
  mqqt_msg[5] ='2' ;
  mqqt_msg[6] ='7' ;
  mqqt_msg[7] ='\r';
  mqqt_msg[8] ='\n';
  mqqt_msg[9] ='R' ;
  mqqt_msg[10]='H' ;
  mqqt_msg[11]=' ' ;
  mqqt_msg[12]='6' ;
  mqqt_msg[13]='5' ;
  mqqt_msg[14]='\r';
  mqqt_msg[15]='\n';
    
  digitalWrite(LED_BUILTIN, LOW);
  WIFI_Connect();
  client.loop();
  client.publish(MQQT_PUBLICATION_TOPIC, mqqt_msg);
  
  #ifdef DEBUG_ENABLE
  data_packet_counter++;
  Serial.print("Data Sent, PacketID-");
  Serial.print(data_packet_counter);
  Serial.print("\r\n");
  #endif

  digitalWrite(LED_BUILTIN, HIGH);
  MQQT_Empty_Bin();
  delay(5000);
  
}
