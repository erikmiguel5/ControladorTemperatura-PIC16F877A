
/* ########################################################################

   PICsim - PIC simulator http://sourceforge.net/projects/picsim/

   ########################################################################

   Copyright (c) : 2015  Luis Claudio Gamb�a Lopes

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   For e-mail suggestions :  lcgamboa@yahoo.com
   ######################################################################## */

#include <xc.h>
#include "adc.h"


void adc_init(void)
{
  ADCON1=0x02;
  ADCON0=0x41; 

}

unsigned int adc_amostra(unsigned char canal)
{
     switch(canal)
    {
      case 0:
        ADCON0=0x00;
        break;
      case 1:
        ADCON0=0x09;
        break;
      case 2:
        ADCON0=0x11;
        break;
    }   

   return ((((unsigned int)ADRESH)<<2)|(ADRESL>>6));
}
