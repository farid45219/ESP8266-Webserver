
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "wifi_mqqt.h"
#include "softwarei2c.h"
#include "datahandler.h"

#define  SERIAL_CHARACTER_THRESHOLD 100
#define  SERIAL_TIME_THRESHOLD      4000

WiFiClient   espClient;
PubSubClient client(espClient);
uint32_t     data_packet_counter=0;
uint32_t     LastMillis=0;

void setup() {
  Serial.begin(38400);
  WIFI_Init();
  client.setServer(MQQT_SERVER_IP_ADDR, MQQT_SERVER_PORT);
  client.setBufferSize(2000);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  delay(5000);
  WIFI_Connect();
  Soft_I2C_Init();
  Data_Handler_Init();
  MQQT_Empty_Bin();
  LastMillis=millis();
  delay(3000);
}


void loop() {


  while((millis()-LastMillis)<SERIAL_TIME_THRESHOLD){
    if( Serial.available() > 0 ){
      mqqt_msg[DataHandler.DataIndex]=Serial.read();
      DataHandler.DataIndex++;
      LastMillis=millis();
    }
  }

  
    // int32_t data[2];
    // HDC1080_Read_Temperature_Humidity(&data[0], &data[1]);
    // Data_Handler_Add_Field_Text("HDC1080_Temp ");
    // Data_Handler_Add_Field_Value(data[0]);
    // Data_Handler_Add_Field_Text("\r\n");
    // Data_Handler_Add_Field_Text("HDC1080_Humidity ");
    // Data_Handler_Add_Field_Value(data[1]);
    // Data_Handler_Add_Field_Text("\r\n");

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
    Data_Handler_Init();
    

}
