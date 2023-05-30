
typedef struct data_handler_t{
  uint16_t DataIndex;

}data_handler_t;

data_handler_t DataHandler;

void Data_Handler_Init(void);
void Data_Handler_Add_Field_Text(char *text);
void Data_Handler_Add_Field_Value(int32_t val);
