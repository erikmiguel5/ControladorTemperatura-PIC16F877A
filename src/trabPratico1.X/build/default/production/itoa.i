# 1 "itoa.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:/Program Files/Microchip/MPLABX/v5.50/packs/Microchip/PIC16Fxxx_DFP/1.2.33/xc8\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "itoa.c" 2
# 19 "itoa.c"
void itoa(unsigned int val, char* str )
{
  str[0]=(val/10000)+0x30;
  str[1]=((val%10000)/1000)+0x30;
  str[2]=((val%1000)/100)+0x30;
  str[3]=((val%100)/10)+0x30;
  str[4]=(val%10)+0x30;
  str[5]=0;
}
