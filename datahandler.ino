


void Data_Handler_Init(void){
  DataHandler.DataIndex=0;
}

void Data_Handler_Add_Field_Text(char *text){
  uint16_t i=0;
  while(text[i]!='\0'){
    mqqt_msg[DataHandler.DataIndex]=text[i];
    DataHandler.DataIndex++;
    i++;
  }
}

void Data_Handler_Add_Field_Value(int32_t val){
  if(val<0){
    mqqt_msg[DataHandler.DataIndex]='-';
    DataHandler.DataIndex++;
    val=-val;
  }
  if(val<10){
    mqqt_msg[DataHandler.DataIndex]=val+48;
    DataHandler.DataIndex++;
  }else{
    mqqt_msg[DataHandler.DataIndex]=(val/10)+48;
    DataHandler.DataIndex++;
    mqqt_msg[DataHandler.DataIndex]=(val%10)+48;
    DataHandler.DataIndex++;
  }
}