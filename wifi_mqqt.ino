#include <ESP8266WiFi.h>
#include <PubSubClient.h>


typedef struct wifi_target_t{
  uint8_t Found;
  String  SSID;
  int     RSSI;
  uint8_t AltSSID;
  uint8_t Connected;
  uint8_t ClientConnected;
}wifi_target_t;

typedef struct wifi_list_t{
  int     DevicesFound;
  String  SSID[30];
  int     RSSI[30];
  int     StrongestRSSI;
  int     StrongestRSSIIndex;
}wifi_list_t;


typedef struct wifi_device_t{
  wifi_list_t   All;
  wifi_list_t   Shortlisted;
  wifi_target_t Target;
}wifi_device_t;

wifi_device_t WIFI_type;
wifi_device_t *WIFI;

void MQQT_Empty_Bin(void){
  for(uint16_t i=0;i<MQQT_MSG_MAX_LENGTH;i++){
    mqqt_msg[i]=0;    
  }
}

void WIFI_Init(void){
  WIFI=&WIFI_type;
  WIFI->All.DevicesFound=0;
  WIFI->All.StrongestRSSI=0;
  WIFI->All.StrongestRSSIIndex=0;
  WIFI->Shortlisted.DevicesFound=0;
  WIFI->Shortlisted.StrongestRSSI=0;
  WIFI->Shortlisted.StrongestRSSIIndex=0;
  WIFI->Target.Found=0;
  WIFI->Target.SSID="";
  WIFI->Target.RSSI=0;
  WIFI->Target.AltSSID=0;
  WIFI->Target.Connected=0;
  WIFI->Target.ClientConnected=0;
}

void WIFI_Scan(void){
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WIFI->All.StrongestRSSI=-500;
  WIFI->All.DevicesFound=WiFi.scanNetworks();
  for(int i=0;i<WIFI->All.DevicesFound;i++){
    WIFI->All.SSID[i]=WiFi.SSID(i);
    WIFI->All.RSSI[i]=WiFi.RSSI(i);
    if(WIFI->All.RSSI[i]>WIFI->All.StrongestRSSI){
      WIFI->All.StrongestRSSI=WIFI->All.RSSI[i];
      WIFI->All.StrongestRSSIIndex=i;
    }
  }
}

void WIFI_Shortlist(){
  WIFI->Shortlisted.DevicesFound=0;
  for(int i=0;i<WIFI->All.DevicesFound;i++){
    if( (strstr(WIFI->All.SSID[i].c_str(), TARGET_FILTER_SSID))  ||  (strstr(WIFI->All.SSID[i].c_str(), TARGET_FILTER_SSID_ALT)) ){
      if(strstr(WIFI->All.SSID[i].c_str(), TARGET_FILTER_SSID_ALT)){
        WIFI->Target.AltSSID=1;
      }
      WIFI->Shortlisted.SSID[WIFI->Shortlisted.DevicesFound]=WIFI->All.SSID[i];
      WIFI->Shortlisted.RSSI[WIFI->Shortlisted.DevicesFound]=WIFI->All.RSSI[i];
      WIFI->Shortlisted.DevicesFound++;
    }
  }
}

void WIFI_Select_Target(void){
  WIFI->Target.Found=0;
  WIFI->Shortlisted.StrongestRSSI=-500;
  for(int i=0;i<WIFI->Shortlisted.DevicesFound;i++){
    if(WIFI->Shortlisted.RSSI[i]>WIFI->Shortlisted.StrongestRSSI){
      WIFI->Shortlisted.StrongestRSSI=WIFI->Shortlisted.RSSI[i];
      WIFI->Shortlisted.StrongestRSSIIndex=i;
    }
  }
  if(WIFI->Shortlisted.StrongestRSSI!=-500){
    WIFI->Target.RSSI=WIFI->Shortlisted.RSSI[WIFI->Shortlisted.StrongestRSSIIndex];
    WIFI->Target.SSID=WIFI->Shortlisted.SSID[WIFI->Shortlisted.StrongestRSSIIndex];
    WIFI->Target.Found=1;
  }
}

void WIFI_Connect_To_Target(void){
  int counter=0;
  WIFI->Target.Connected=0;
  if(WIFI->Target.Found==1){
    if(WIFI->Target.AltSSID){
      WiFi.begin(WIFI->Target.SSID.c_str(), TARGET_FILTER_ALT_PASS);
    }else{
      WiFi.begin(WIFI->Target.SSID.c_str(), TARGET_FILTER_PASS);
    }
    #ifdef DEBUG_ENABLE
    Serial.print("[");
    #endif
    while (WiFi.status() != WL_CONNECTED){
      delay(500);
      #ifdef DEBUG_ENABLE
      Serial.print(".");
      #endif
      counter++;
      if(counter>=TARGET_CONNECT_TIMEOUT){
        break;
      }
    }
    #ifdef DEBUG_ENABLE
    Serial.print("]\r\n");
    #endif
    if(counter<TARGET_CONNECT_TIMEOUT){
      #ifdef DEBUG_ENABLE
      Serial.print("Connected To [");
      Serial.print(WIFI->Target.SSID);
      Serial.print("]\r\n");
      Serial.print("Time Taken: ");
      Serial.print(counter/2);
      Serial.print("s\r\n\r\n");
      #endif
      WIFI->Target.Connected=1;
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(500);
    }else{
      WIFI->Target.Connected=0;
      digitalWrite(LED_BUILTIN, LOW);
      #ifdef DEBUG_ENABLE
      Serial.print("Could Not Connect To Target WiFi\r\n\r\n");
      #endif 
    }
  }
}

void WIFI_Connect_To_Client(void){
  while (!client.connected()){
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str()))
    {
      WIFI->Target.ClientConnected=1;
      #ifdef DEBUG_ENABLE
      Serial.print("Connected To Client\r\n");
      #endif
      
      digitalWrite(LED_BUILTIN, HIGH);

    }
    else
    { 
      digitalWrite(LED_BUILTIN, LOW);
      #ifdef DEBUG_ENABLE
      Serial.print("Could Not Connect To Client\r\n");
      #endif
      WIFI->Target.ClientConnected=0;
      if (WiFi.status() != WL_CONNECTED){
        #ifdef DEBUG_ENABLE
        Serial.print("WiFi Connection Lost\r\n");
        Serial.print("Retrying to Establish Connection With Target WiFi\r\n");
        #endif
        WIFI_Connect_To_Target();
      }
      #ifdef DEBUG_ENABLE
      Serial.print("Attempting again..\r\n");
      #endif
      delay(5000);
    }
  }
}

void Print_All_WiFi_Devices(void){
  Serial.print("ALL DEVICES:\r\n");
  for(int i=0;i<WIFI->All.DevicesFound;i++){
    Serial.print("[");
    Serial.print(WIFI->All.RSSI[i]);
    Serial.print("] ");
    Serial.print(WIFI->All.SSID[i]);
    Serial.print("\r\n");
  }
  Serial.print("\r\n");
}

void Print_Shortlisted_WiFi_Devices(void){
  Serial.print("SHORTLISTED DEVICES:\r\n");
  if(WIFI->Shortlisted.DevicesFound==0){
    Serial.print("No devices found");
    Serial.print("\r\n");
  }else{
    for(int i=0;i<WIFI->Shortlisted.DevicesFound;i++){
      Serial.print("[");
      Serial.print(WIFI->Shortlisted.RSSI[i]);
      Serial.print("] ");
      Serial.print(WIFI->Shortlisted.SSID[i]);
      Serial.print("\r\n");
    }
  }
  Serial.print("\r\n");
}

void Print_Target_WiFi_Device(void){
  Serial.print("TARGET DEVICE:\r\n");
  if(WIFI->Target.Found==0){
    Serial.print("No devices found");
    Serial.print("\r\n");
  }else{
    Serial.print("[");
    Serial.print(WIFI->Target.RSSI);
    Serial.print("] ");
    Serial.print(WIFI->Target.SSID);
    Serial.print("\r\n");    
  }
  Serial.print("\r\n");
}

void WIFI_Connect(void){
  if(!client.connected()){
    if(WiFi.status() != WL_CONNECTED){
      WIFI_Scan();
      #ifdef DEBUG_ENABLE
      Print_All_WiFi_Devices();
      #endif
      WIFI_Shortlist();
      #ifdef DEBUG_ENABLE
      Print_Shortlisted_WiFi_Devices();
      #endif
      WIFI_Select_Target();
      #ifdef DEBUG_ENABLE
      Print_Target_WiFi_Device();
      #endif
      WIFI_Connect_To_Target();
    }
    WIFI_Connect_To_Client();
  }
}