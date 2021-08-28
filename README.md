# PIC16F877A
Controlador de temperatura utilizando o PIC16F877A e a placa MacLab2

O presente trabalho foi realizado para a disciplina de microprocessadores e microcontroladores,
para sua implementação foi utilizado o mplab e o picsimlab, como ambientes de desenvolvimento e teste.

Para dúvidas e sugestões segue o meu email: miguelerik5@gmail.com


Especificações:

Controle de Temperatura

Objetivo: Controlar a temperatura utilizando o conjunto cooler/resistor de potência. Utilizar a 
placa McLab2.
Ajustes:
1 – Ao ligar o micro, exibir uma tela perguntando se o usuário deseja operar em modo local ou 
modo remoto.
2 – Se a opção for modo local, exibir uma tela mostrando a temperatura atual e duas opções 
para o usuário: iniciar controlador ou ajustar setpoint. 
3 – Se o usuário optar por ajustar setpoint, exibir uma tela de ajuste. Ao final, perguntar se o 
usuário deseja gravar o novo setpoint na eeprom. Se sim, salvar o novo valor no lugar do 
anterior. Se não, voltar a tela 1.
4 – Se o usuário selecionar iniciar controlador, ajustar a temperatura para o setpoint desejado.
Utilizar um controlador pi ou pid.
5 – Mostrar no display um indicador de controlador ligado e uma tecla com opção para 
desligar o controlador. Mostrar também a temperatura monitorada e o duty cycle utilizado 
pelo modulador PWM.
6 – Se a opção escolhida for modo remoto, repetir os passos de 2 a 5 utilizando a comunicação 
serial como porta de entrada e saída de dados. O display deverá exibir a temperatura medida e 
indicar operação em modo remoto. Também deverá exibir uma tecla que poderá ser 
pressionada para retornar o sistema de controle para modo local.
7 – Se usuário optar por retomar ao modo local, voltar para a etapa 1

