
#define  SOFT_I2C_SCL               12U
#define  SOFT_I2C_SDA               14U

void Soft_I2C_GPIO_Init(void){
  pinMode(SOFT_I2C_SCL, OUTPUT);
  digitalWrite(SOFT_I2C_SCL, HIGH);
  pinMode(SOFT_I2C_SDA, INPUT_PULLUP);
}

void Soft_I2C_Timer_Config(void){
    
}

void Soft_I2C_Init(void){
    Soft_I2C_GPIO_Init();
}


void Soft_I2C_SCL_Logic_Low(void){
  digitalWrite(SOFT_I2C_SCL, LOW);
}

void Soft_I2C_SCL_Logic_High(void){
  digitalWrite(SOFT_I2C_SCL, HIGH);
}

void Soft_I2C_SDA_Input(void){
  pinMode(SOFT_I2C_SDA, INPUT_PULLUP);
}

void Soft_I2C_SDA_Output(void){
  pinMode(SOFT_I2C_SDA, OUTPUT);
}

void Soft_I2C_SDA_Logic_Low(void){
  digitalWrite(SOFT_I2C_SDA, LOW);
}

void Soft_I2C_SDA_Logic_High(void){
  digitalWrite(SOFT_I2C_SDA, HIGH);
}


uint8_t Soft_I2C_Read_SDA_State(void){
  if(digitalRead(SOFT_I2C_SDA)){
    return 1;
  }else{
    return 0;
  }
}

void Soft_I2C_Half_Bit_Delay(void){
    delayMicroseconds(10);
}

void Soft_I2C_Bit_Delay(void){
    delayMicroseconds(20);
}

void Soft_I2C_Start(void){
  Soft_I2C_Half_Bit_Delay();
  Soft_I2C_SDA_Logic_Low();
  Soft_I2C_Half_Bit_Delay();
  Soft_I2C_SCL_Logic_Low();
  Soft_I2C_Half_Bit_Delay();
}

void Soft_I2C_Restart(void){
  Soft_I2C_Half_Bit_Delay();
  Soft_I2C_SDA_Logic_High();
  Soft_I2C_Half_Bit_Delay();
  Soft_I2C_SCL_Logic_High();
  Soft_I2C_Half_Bit_Delay();
  Soft_I2C_SDA_Logic_Low();
  Soft_I2C_Half_Bit_Delay();
  Soft_I2C_SCL_Logic_Low();
  Soft_I2C_Half_Bit_Delay();
}

void Soft_I2C_Stop(void){
  Soft_I2C_SCL_Logic_Low();
  Soft_I2C_SDA_Logic_Low();
  Soft_I2C_Half_Bit_Delay();
  Soft_I2C_SCL_Logic_High();
  Soft_I2C_Bit_Delay();
  Soft_I2C_SDA_Logic_High();
  Soft_I2C_Half_Bit_Delay();
}

void Soft_I2C_Transmit_ACK(void){
  Soft_I2C_SDA_Logic_Low();
  Soft_I2C_Half_Bit_Delay();
  Soft_I2C_SCL_Logic_High();
  Soft_I2C_Half_Bit_Delay();
  Soft_I2C_SCL_Logic_Low();
  Soft_I2C_Half_Bit_Delay();
}

void Soft_I2C_Transmit_NACK(void){
  Soft_I2C_SDA_Logic_High();
  Soft_I2C_Half_Bit_Delay();
  Soft_I2C_SCL_Logic_High();
  Soft_I2C_Half_Bit_Delay();
  Soft_I2C_SCL_Logic_Low();
  Soft_I2C_Half_Bit_Delay();
}

uint8_t Soft_I2C_Transmit_Byte(uint8_t data){
  uint8_t ack=0;
  for(uint8_t i=0;i<8;i++){
    Soft_I2C_Half_Bit_Delay();
    if(data & 0x80){
      Soft_I2C_SDA_Logic_High();
    }else{
      Soft_I2C_SDA_Logic_Low();
    }
    data<<=1;
    Soft_I2C_Half_Bit_Delay();
    Soft_I2C_SCL_Logic_High();
    Soft_I2C_Half_Bit_Delay();
    Soft_I2C_SCL_Logic_Low();
  }
  Soft_I2C_SDA_Input();
  Soft_I2C_Bit_Delay();
  Soft_I2C_SCL_Logic_High();
  ack=Soft_I2C_Read_SDA_State();
  Soft_I2C_Half_Bit_Delay();
  Soft_I2C_SCL_Logic_Low();
  Soft_I2C_Half_Bit_Delay();
  Soft_I2C_SDA_Output();
  if(ack==0){
    ack=1;
  }else{
    ack=0;
  }
  return ack;
}

uint8_t Soft_I2C_Read_Byte_NACK(void){
  uint8_t data=0;
  Soft_I2C_SDA_Input();
  for(uint8_t i=0;i<8;i++){
      data<<=1;
      Soft_I2C_Half_Bit_Delay();
      Soft_I2C_SCL_Logic_High();
      data|=Soft_I2C_Read_SDA_State();
      Soft_I2C_Half_Bit_Delay();
      Soft_I2C_SCL_Logic_Low();
      Soft_I2C_Half_Bit_Delay();
  }
  Soft_I2C_SDA_Output();
  Soft_I2C_Transmit_NACK();
  return data;
}

uint8_t Soft_I2C_Read_Byte_ACK(void){
  uint8_t data=0;
  Soft_I2C_SDA_Input();
  for(uint8_t i=0;i<8;i++){
      data<<=1;
      Soft_I2C_Half_Bit_Delay();
      Soft_I2C_SCL_Logic_High();
      data|=Soft_I2C_Read_SDA_State();
      Soft_I2C_Half_Bit_Delay();
      Soft_I2C_SCL_Logic_Low();
      Soft_I2C_Half_Bit_Delay();
  }
  Soft_I2C_SDA_Output();
  Soft_I2C_Transmit_ACK();
  return data;
}

uint8_t Soft_I2C_Transmit_Address_With_RW(uint8_t address_rw){
  uint8_t sts=0;
  Soft_I2C_Start();
  sts=Soft_I2C_Transmit_Byte(address_rw);
  Soft_I2C_Stop();
  return sts;
}

uint8_t Soft_I2C_Write_Register(uint8_t address, uint8_t reg_address, uint8_t *data, uint8_t len){
  uint8_t sts=0;
  address&=~(1<<0);
  Soft_I2C_Start();
  sts=Soft_I2C_Transmit_Byte(address);
  sts&=Soft_I2C_Transmit_Byte(reg_address);
  for(uint8_t i=0;i<len;i++){
    sts&=Soft_I2C_Transmit_Byte(data[i]);
  }
  Soft_I2C_Stop();
  return sts;
}

void Soft_I2C_Read_Register(uint8_t address, uint8_t reg_address, uint8_t *data, uint8_t len){
  uint8_t sts=0;
  // Soft_I2C_Start();
  // address&=~(1<<0);
  // Soft_I2C_Transmit_Byte(address);
  // Soft_I2C_Transmit_Byte(reg_address);
  // Soft_I2C_Stop();
  Soft_I2C_Start();
  address|=(1<<0);
  Soft_I2C_Transmit_Byte(address);
  for(uint8_t i=0;i<len;i++){
    if(i==(len-1)){
      data[i]=Soft_I2C_Read_Byte_NACK();
    }else{
      data[i]=Soft_I2C_Read_Byte_ACK();
    }
  }
  Soft_I2C_Stop();
}

void HDC1080_Start_Conversion(void){
  uint8_t data[2];
  data[0]=0x30;
  data[1]=0x00;
  Soft_I2C_Write_Register(0x80, 0x02, data, 2);
  Soft_I2C_Write_Register(0x80, 0x00, data, 0);
  delay(20);
}

void HDC1080_Read_Raw_Temp_Himidity(uint8_t *data){
  Soft_I2C_Read_Register(0x80, 0x00, data, 4);
}

void HDC1080_Read_Temperature_Humidity(int32_t *temperature, int32_t *humidity){
    uint8_t data[4];
  HDC1080_Start_Conversion();
  HDC1080_Read_Raw_Temp_Himidity(data);
  uint16_t temp_raw=data[0];
  temp_raw<<=8;
  temp_raw|=data[1];
  uint16_t humidity_raw=data[2];
  humidity_raw<<=8;
  humidity_raw|=data[3];

  int32_t temp=temp_raw;
  temp*=165;
  temp/=65536;
  temp-=40;

  int32_t hum=humidity_raw;
  hum*=100;
  hum/=65536;

  *temperature=temp;
  *humidity=hum;
}
