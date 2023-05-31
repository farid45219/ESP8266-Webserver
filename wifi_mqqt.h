
#define  MQQT_SERVER_IP_ADDR     "192.168.119.10"
#define  MQQT_SERVER_PORT        1883
#define  MQQT_PUBLICATION_TOPIC  "ref/dsp/UDR/003"

#define  TARGET_FILTER_SSID      "RPM IoT Lab"
#define  TARGET_FILTER_SSID_ALT  "Iot"
#define  TARGET_FILTER_PASS      "password@iiT"
#define  TARGET_FILTER_ALT_PASS  "Password@#2023"
#define  TARGET_CONNECT_TIMEOUT  30U
#define  MQQT_MSG_MAX_LENGTH     1500UL
#define  DEBUG_ENABLE            1U

char     mqqt_msg[MQQT_MSG_MAX_LENGTH];

void     MQQT_Empty_Bin(void);
void     WIFI_Init(void);
void     WIFI_Scan(void);
void     WIFI_Shortlist();
void     WIFI_Select_Target(void);
void     WIFI_Connect_To_Target(void);
void     WIFI_Connect_To_Client(void);
void     Print_All_WiFi_Devices(void);
void     Print_Target_WiFi_Device(void);
void     WIFI_Connect(void);
