/*Arquivo com as funções utilizadas para o trbalho pratico*/


/*Função ue verifica e retorna o valor da temperatura*/
int verificarTemperatura(){
    unsigned int temp;
    ADCON0bits.GO = 1;
    while(ADCON0bits.GO == 1);
    temp = (unsigned int) (ADRESH<<8 | ADRESL);
    temp= temp*10/8-150;
    return temp;
}

/*Imprime a temperatura na tela, se o parametro, 
para linha == 1, imprime na primeira linha
para linha == 2, imprime na segunda linha
*/
void imprimirTemp(int temp, int linha){
    char str[6];
    itoa(temp,str);   
    if(linha)lcd_cmd(L_L1 + 2);
    else lcd_cmd(L_L2 + 2);
    lcd_str("Temp: ");
    lcd_dat(str[2]);
    lcd_dat(str[3]);
    lcd_dat(',');
    lcd_dat(str[4]);
    lcd_dat('C');
    __delay_ms(500);
}
void armazenarSet(){
    dezena = eeprom_read(0x2A)  - 0x30;
    unidade = eeprom_read(0x0A) - 0x30;
    setPoint = dezena*10 + unidade;
}
/*Função para imprimir primeira tela*/
int telaInicio(){
    int flag = 0;
    armazenarSet();
    
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_dat(dezena + 0x30);
    lcd_dat(unidade + 0x30);
    __delay_ms(1000);
    
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1+1);
    lcd_str("Modo Local ...");
    __delay_ms(500);
    
    lcd_cmd(L_CLR);
    while(flag == 0){
    imprimirTemp(verificarTemperatura(), 1);
    lcd_cmd(L_L2+1);
    lcd_str("1-Start  2-Set");
    __delay_ms(10);
    if(PORTBbits.RB1 == 0) flag = 1;
    if(PORTBbits.RB2 == 0) flag = 2;
    }
    return flag;
}

void alterarSetPoint(){
    armazenarSet();
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_str("Valor Atualizado");
     lcd_cmd(L_L2+1);
    lcd_str("SetPoint: ");
    lcd_dat(dezena+0x30);
    lcd_dat(unidade+0x30);
    __delay_ms(1000);
}

void telaSetPoint(){
    int contD = 0;
    int contU = 0;
    int flag = 0;

    lcd_cmd(L_CLR);
    lcd_cmd(L_L1+1);
    lcd_str("1-Incr  2-Prox");
    __delay_ms(200);
    /*laço que implementa o contador*/
    while(flag == 0){
        lcd_cmd(L_L2+2);
        lcd_str("SetPoint: ");
        lcd_dat(contD + 0x30);
        lcd_dat(contU + 0x30);
        __delay_ms(10);
        if(PORTBbits.RB1 == 0) contU++;
        if(contU == 10){
            contU = 0;
            contD++;
            if(contD == 10) contD = 0;
        }
        if(PORTBbits.RB2 == 0) flag = 2;
    }
    flag = 0;
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1+1);
    lcd_str("1-Salvar");
    lcd_cmd(L_L2+1);
    lcd_str("2-Inicio");
    __delay_ms(500);
    while(flag == 0){
        if(RB1 == 0){
            eeprom_write(0x0A, (contU+0x30));
            eeprom_write(0x2A, (contD+0x30));
            alterarSetPoint();
            flag = 1;
        }
        if(RB2 == 0){
            flag = 1;
        }
    }
    
}

int telaGravarEEPROM(){
    int contD = 0;
    int contU = 0;
    int flag = 0;
    
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_str("Salvar SetPoint");
    lcd_cmd(L_L2);
    lcd_str("Para Iniciar");
    __delay_ms(1250);
    
    while(flag != 1){
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1+1);
    lcd_str("1-Incr  2-Prox");
    /*laço que implementa o contador*/
    while(flag != 1){
        lcd_cmd(L_L2+2);
        lcd_str("SetPoint: ");
        lcd_dat(contD + 0x30);
        lcd_dat(contU + 0x30);
        __delay_ms(10);
        if(PORTBbits.RB1 == 0) contU++;
        if(contU == 10){
            contU = 0;
            contD++;
            if(contD == 10) contD = 0;
        }
        if(PORTBbits.RB2 == 0) flag = 1;
    }
    flag = 0;
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1+1);
    lcd_str("1-Salvar");
    lcd_cmd(L_L2+1);
    lcd_str("2-Voltar");
    __delay_ms(500);
    while(flag == 0){
        if(RB1 == 0){
            alterarSetPoint();
            flag = 1;
        }
        if(RB2 == 0){
            flag = 2;
        }
    }
    if(flag == 2)
    flag = 0;
    }
    eeprom_write(0x0A, (contU+0x30));
    eeprom_write(0x2A, (contD+0x30));
    return 0;
}

int iniciarControlador(){
    int tempAtual = verificarTemperatura();
    int erro;
    int kp = 60;
    int ki = 10;
    long int num_dac;
    long int dutyCycle;

    erro = setPoint*10 - tempAtual;

    if(pi < 100000){
        pi += erro * ki;
    }
    else{
        pi = 100000;
    }

    num_dac = (int)((pi + kp*erro)/100);

    if(num_dac < 0){
        num_dac = 0;
    }
    else if(num_dac > 1023){
        num_dac = 1023;
    }


    if (verificarTemperatura() < setPoint*10-1) {
            RC1 = 0;
            __delay_ms(500);
            
            // This 10-bit value is represented by CCPR1L:CCP1CON<5:4>
            CCPR1L = (dutyCycle & 0x3FC) >> 2;  // Mask1 - CCPR1L: First 8 MSBs
            CCP1X = ((dutyCycle & 0x002) >> 1); // Mask2 - CCP1CON<5>
            CCP1Y = (dutyCycle & 0x001);        // Mask3 - CCP1CON<4>
            dutyCycle = num_dac;
        } else if(verificarTemperatura() > setPoint*10 +3){
            dutyCycle = 0;
            num_dac = 0;
            pi = 0;
            RC1 = 1;
        }
        if(verificarTemperatura() <= 278){
            RC1 = 0;
        }
    return dutyCycle;
}

int telaControlador(){

    char str[6];
    char strDC[6];
    int temp;
    long int dc;

    lcd_cmd(L_CLR);
    
    while(RB2){
        temp = verificarTemperatura();
        dc = iniciarControlador();

        itoa(temp,str);
        itoa(dc,strDC); 

        lcd_cmd(L_L1+1);
        lcd_str("Ligado  2-off");

        lcd_cmd(L_L2);
        lcd_str("T: ");
        lcd_dat(str[2]);
        lcd_dat(str[3]);
        lcd_dat(',');
        lcd_dat(str[4]);
        lcd_dat('C ');
        
        if(strDC[0] == 1){
            strDC[1] = 0;
        }
        lcd_str("DC:");
        lcd_dat(strDC[1]);
        lcd_dat(strDC[2]);
        lcd_dat(strDC[3]);
        lcd_dat('%');
        __delay_ms(500);
        if(RB2 == 0){
            RC1 = 0;
            CCPR1L = 0;
            return 0;
        }
    }
}
