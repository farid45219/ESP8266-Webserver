


void    Soft_I2C_GPIO_Init(void);
void    Soft_I2C_Timer_Config(void);
void    Soft_I2C_Init(void);
void    Soft_I2C_SCL_Logic_Low(void);
void    Soft_I2C_SCL_Logic_High(void);
void    Soft_I2C_SDA_Logic_Low(void);
void    Soft_I2C_SDA_Logic_High(void);
void    Soft_I2C_SDA_Input(void);
void    Soft_I2C_SDA_Output(void);
uint8_t Soft_I2C_Read_SDA_State(void);
void    Soft_I2C_Half_Bit_Delay(void);
void    Soft_I2C_Bit_Delay(void);
void    Soft_I2C_Start(void);
void    Soft_I2C_Restart(void);
void    Soft_I2C_Stop(void);
void    Soft_I2C_Transmit_ACK(void);
void    Soft_I2C_Transmit_NACK(void);
uint8_t Soft_I2C_Transmit_Byte(uint8_t data);
uint8_t Soft_I2C_Read_Byte_NACK(void);
uint8_t Soft_I2C_Read_Byte_ACK(void);
uint8_t Soft_I2C_Transmit_Address_With_RW(uint8_t address_rw);
uint8_t Soft_I2C_Write_Register(uint8_t address, uint8_t reg_address, uint8_t *data, uint8_t len);
void    Soft_I2C_Read_Register(uint8_t address, uint8_t reg_address, uint8_t *data, uint8_t len);
void    HDC1080_Start_Conversion(void);
void    HDC1080_Read_Raw_Temp_Himidity(uint8_t *data);
void    HDC1080_Read_Temperature_Humidity(int32_t *temperature, int32_t *humidity);