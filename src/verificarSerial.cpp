#include "../include/functions.h"
#include "../include/macros.h"
#include "../include/globals.h"

//nmhkjh

void VerificarSerial(AccelStepper* motor1, AccelStepper* motor2, int velocidadeMaxima, int velocidade, int aceleracaoMaxima){
  

    //Definição de variáveis
    String posicao_calculadaStr1, posicao_calculadaStr2;

    int  aceleracao1, aceleracao2,  posicao_calculada1, posicao_calculada2, parar_calibracao,
    constanteCalibracao1, constanteCalibracao2, motorParou1;

    float qtdPulsosMotor1, qtdPulsosMotor2, qtdPulsosMotores1, qtdPulsosMotores2, velocidadeMaxima1, velocidadeMaxima2;

    float receivedPulsesDistance1, receivedPulsesDistance2,
    receivedDelay1, receivedDelay2, zero_laser;

    static int motor = 1;
    int direcao1 = 1;
    int direcao2 = 1;
    int direcaoMotor = 1;

    if(Serial.available()){
        String data = Serial.readStringUntil('#');
        char digitoUm = data.charAt(0);
    
        switch(digitoUm){     //Verifica as condições com base no primeiro caractere
    
          case LIGAR_MOTOR:{         //Se o primeiro caractere for A, liga o motor
            if(motor == MOTOR_1){
              digitalWrite(PIN_ENABLE_1, 1);
              //Serial.println("/Motor 1 ligado!");
            }
            else if(motor == MOTOR_2){
              digitalWrite(PIN_ENABLE_2, 1);
              //Serial.println("/Motor 2 ligado!");
            }else if(motor == MOTORES_SIMULTANEOS){
              digitalWrite(PIN_ENABLE_1, 1);
              digitalWrite(PIN_ENABLE_2, 1);
            }
          break;
          }
          case ALTERAR_PARA_MOTORES_SIMULTANEOS:{
            motor = MOTORES_SIMULTANEOS;
            //Serial.println("Motores simultaneos sendo operados");
          break;
          }

          case DESLIGAR_MOTOR:         //Se o primeiro caractere for a, desliga o motor
            if(motor == MOTOR_1){
              digitalWrite(PIN_ENABLE_1, 0);
              //Serial.println("/Motor 1 desligado!");
            }
            else if(motor == MOTOR_2){
              digitalWrite(PIN_ENABLE_2, 0);
              //Serial.println("/Motor 2 desligado!");
            } else if(motor == MOTORES_SIMULTANEOS){
              digitalWrite(PIN_ENABLE_1, 0);
              digitalWrite(PIN_ENABLE_2, 0);
            }
          break;
          
          case MOVER_MOTOR_CIMA:         //Se o primeiro caractere é C, move o motor para cima
    
            if(motor == MOTOR_1){
              //Serial.println("c"); // Printa a mensagem no aplicativo do vs code:: Direcão: Para cima
              direcao1 = 1;
              direcaoMotor = 1;
            }
            else if(motor == MOTOR_2){
              //Serial.println("C"); // Printa a mensagem no aplicativo do vs code:: Direcão: Para cima
              direcao2 = 1;
              direcaoMotor = -1;
            }else if(motor == MOTORES_SIMULTANEOS) {
              direcao1 = 1;
              direcao2 = -1;
            }
          break;
    
          case MOVER_MOTOR_BAIXO:        //Se o primeiro caractere é B, move o motor para baixo
    
            if(motor == MOTOR_1){
              //Serial.println(DIRECAO_MOTOR_1_BAIXO); // Printa a mensagem no aplicativo do vs code:: Direcão: Para baixo
              direcao1 = 1;
              direcaoMotor = 1;
            }
            else if(motor == MOTOR_2){
              //Serial.println(DIRECAO_MOTOR_2_BAIXO); // Printa a mensagem no aplicativo do vs code:: Direcão: Para baixo
              direcao2 = 1;
              direcaoMotor = -1;
            }else if(motor == MOTORES_SIMULTANEOS){
              direcao1 = 1;
              direcao2 = 1;
            }
    
          break;
    
          case QTD_PULSOS: {         //Se o primeiro caractere é P, envia a quantidade de pulsos
            // a cada pulso se movimenta 0,6 mm -> conta: (1,8 \ (3 * 4)) * 4mm do passo
            String info_qtd_pulsos = data.substring(1);
            if(motor == '1'){
                qtdPulsosMotor1 = info_qtd_pulsos.toFloat(); //value for the steps
              //Serial.print("/Pulsos motor 1: ");
              //Serial.println(receivedPulsesDistance1);
            }
            else if(motor == '2'){
                qtdPulsosMotor2 = info_qtd_pulsos.toFloat(); //value for the steps
             //Serial.print("/Pulsos motor 2: ");
              //Serial.println(receivedPulsesDistance2);
    
            }
          break;
          }
          case ACELERAR_MOTOR:     //First character is an G = motor accelerates
            if(motor == '1'){
              //Serial.println("a"); // Printa a mensagem no aplicativo do vs code:: O motor 1 está se movendo com aceleração!
              moverAcelerado(motor1, qtdPulsosMotor1, velocidadeMaxima, direcaoMotor);
            }
            else if(motor == '2'){
              //Serial.println("A"); // Printa a mensagem no aplicativo do vs code: O motor 2 está se movendo com aceleração!
              moverAcelerado(motor2, qtdPulsosMotor2, velocidadeMaxima, direcaoMotor);
            }
    
          
          break;
    
          case MSG_MOTOR_MOVENDO_COM_ACELERACAO:
            if(motor == '1'){
              //Serial.println("a"); // Printa a mensagem no aplicativo do vs code:: O motor 1 está se movendo com aceleração!
              aceleracao1 = 1;
            }
            else if(motor == '2'){
              //Serial.println("Entrei na função do motor 2");
              //Serial.println("A"); // Printa a mensagem no aplicativo do vs code: O motor 2 está se movendo com aceleração!
              aceleracao2 = 1;
            }
          break;
    
          case MSG_MOTOR_MOVENDO_SEM_ACELERACAO:
            if(motor == '1'){
              //Serial.println("a"); // Printa a mensagem no aplicativo do vs code:: O motor 1 está se movendo sem aceleração!
              aceleracao1 = 0;
            }
            else if(motor == '2'){
              //Serial.println("A"); // Printa a mensagem no aplicativo do vs code: O motor 2 está se movendo sem aceleração!
              aceleracao2 = 0;
            }
          break;
    
    
          case ATIVAR_MOTOR_VEL_CTE :     //First character is an H = motor spins with constant speed
            if(motor == '1'){
              //Serial.println("/O motor 1 se move com velocidade constante!");
              moverUniforme(motor1, qtdPulsosMotor1, velocidadeMaxima, direcaoMotor);
              
            }
            else if(motor == '2'){
              //Serial.println("/O motor 2 se move com velocidade constante!");
              moverUniforme(motor2, qtdPulsosMotor2, velocidadeMaxima, direcaoMotor);
    
            }
          break;
          
          case DEFINIR_VELOCIDADE: {        //First character is an V = set velocity
            String x = data.substring(1);
            float y = x.toFloat();
            if(200<y<8000){
              if(motor == '1'){  
                receivedDelay1 = y;
                //Serial.println("/Velocidade do motor 1: " + x + " Pulsos por segundo");
    
    
              }
              else if(motor == '2'){
                //Serial.println("/Velocidade do motor 2: " + x + " Pulsos por segundo");
    
                receivedDelay2 = y;
              }
            }
            else{
             // Serial.println("Q"); //Printa a mensagem no aplicativo do vs code: "Valor de velocidade inválido! Insira um valor entre 200 e 8000 pulsos/segundo
            }
    
          break;
          }
          case PARAR_MOTOR: //para o motor, #define PARAR_MOTOR 'n' no arquivo macros.h
          
            if(motor == 1){
              //paraMotor(motor1);
              digitalWrite(PIN_ENABLE_1, 0);
              pararMotor = true;
            }
            else if(motor == 2){
              //paraMotor(motor2);
              digitalWrite(PIN_ENABLE_2, 0);
              pararMotor = true;
            } else if(motor == 3){
              //paraMotorSimultaneo(motor1, motor2);
              paraMotorSimultaneo(motor1, motor2);
              pararMotorSimultaneo = true;
            }

          break;
          
          case PARAR_CALIBRACAO : //para calibração
            parar_calibracao = 1;
    
          break;
    
          case POSICAO_MOTOR_1 :{ //  Insere um valor para a posição calculada do primeiro motor. 
            delayMicroseconds(1000); 
            String x = data.substring(1);
            posicao_calculada1 = x.toFloat();
            delayMicroseconds(1000); 
            posicao_calculadaStr1  = String(posicao_calculada1); 
          break;
          }
          case POSICAO_MOTOR_2: { //  Insere um valor para a posição calculada do segundo motor. 
            delayMicroseconds(1000); 
            String x = data.substring(1);
            posicao_calculada2 = x.toFloat();
            delayMicroseconds(1000); 
            posicao_calculadaStr2  = String(posicao_calculada2);
            //Serial.println('P'+ posicao_calculadaStr2);
          break;
          }
          case CONFIGURAR_ZERO_LASER: { //sets intercept of the laser
            String  x = data.substring(1);
            zero_laser = x.toFloat();
          break;
          }
          
          
          case INICIAR_CALIBRACAO: // Inicia o processo de calibração
            calibracao();
    
          break;
    
          case INSERIR_CONSTANTES_CALIBRACAO: { //Insere constante de calibração dos motores
            String x = data.substring(1);
            if(motor == 1){
              constanteCalibracao1 = x.toFloat();
            }
            else if(motor == 2){
              constanteCalibracao2 = x.toFloat();
            } else if(motor ==3){
              constanteCalibracao1 = x.toFloat();
              constanteCalibracao2 = x.toFloat();
            }
    
            Serial.print('w'); //Printa a constante de calibração no app do VSCode 
            Serial.println(x);
          break;
          }
    
    
          case ATIVAR_SENSOR_INDUTIVO: // ativa a funcionalidade do sensor indutivo novamente
            motorParou1 = 0;
            Serial.print("\nSensor indutivo ativado"); //Printa a constante de calibração no app do VSCode 
          break;
    
          case ALTERAR_PARA_MOTOR_2: //Função para mudar qual motor está sendo utilizado.
    
            motor = 2;
            //Serial.println('u'); //Segundo motor sendo operado!
          break;
    
          case ALTERAR_PARA_MOTOR_1: //Função para mudar qual motor está sendo utilizado.
            motor = 1;
            //Serial.println('U');//Primeiro motor sendo operado!
          break;
    
          case SUBSIDENCIA:
            //subsidencia(); // Função que movimenta o motor para frente e para trás (2 voltas completas) ativando o mecanismo de subsidência
          break;
    
          case ENVIAR_CONFIG_COMPLETA: { // recebe todas as informações do motor de uma vez e aciona o motor
            String x = data.substring(1);
    
            //código para separar as strings 
            int firstSeparatorIndex = x.indexOf(';');
            int secondSeparatorIndex = x.indexOf(';', firstSeparatorIndex + 1);
            int thirdSeparatorIndex = x.indexOf(';', secondSeparatorIndex + 1);
    
            // Extract substrings based on the positions of the separators
            String pulso = x.substring(0, firstSeparatorIndex); // "primeiro numero"
            String velocidade = x.substring(firstSeparatorIndex + 1, secondSeparatorIndex); // "segundo numero"
            String direcao = x.substring(secondSeparatorIndex + 1, thirdSeparatorIndex); // "caracter B ou C"
            String mover = x.substring(thirdSeparatorIndex + 1); // "caracter H ou x"
    
            //Exemplo T2000;200;B;H#        
            if(motor == 1){
              //liga motor
              digitalWrite(PIN_ENABLE_1, 1);
              //Serial.println("/Motor 1 ligado!");
              //recebe pulsos
              qtdPulsosMotor1 = pulso.toFloat(); //value for the steps
              //Serial.print("/Pulsos motor 1: ");
             // Serial.println(receivedPulsesDistance1);
              //recebe velocidade
              velocidadeMaxima = velocidade.toFloat();
              //Serial.println("/Velocidade do motor 1: " + velocidade + " Pulsos por segundo");
              if(direcao == "B"){
                //Serial.println("b"); // Printa a mensagem no aplicativo do vs code:: Direcão: Para baixo
                direcao1 = 1;
                direcaoMotor = 1;
              }
              else if(direcao == "C"){
                //Serial.println("c"); // Printa a mensagem no aplicativo do vs code:: Direcão: Para cima
                direcao1 = -1;
                direcaoMotor = -1;
              }
              if(mover == "H"){
                moverUniforme(motor1, qtdPulsosMotor1, velocidadeMaxima, direcaoMotor);
              }
    
              
            }
            else if(motor == 2){
              //liga motor
              digitalWrite(PIN_ENABLE_2, 1);
              //Serial.println("/Motor 2 ligado!");
              //recebe pulsos
              qtdPulsosMotor2 = pulso.toFloat(); //value for the steps
              //Serial.print("/Pulsos motor 2: ");
              //Serial.println(receivedPulsesDistance2);
              //recebe velocidade
              //Serial.println("/Velocidade do motor 2: " + x + " Pulsos por segundo");
              velocidadeMaxima = velocidade.toFloat();
              if(direcao == "B"){
                //Serial.println("B"); // Printa a mensagem no aplicativo do vs code:: Direcão: Para baixo
                direcao2 = 1;
                direcaoMotor = 1;
              }
              else if(direcao == "C"){
                  //Serial.println("C"); // Printa a mensagem no aplicativo do vs code:: Direcão: Para cima
                direcao2 = -1;
                direcaoMotor = -1;
              }
              if(mover == "H"){
                moverUniforme(motor2, qtdPulsosMotor2, velocidadeMaxima, direcaoMotor);
              }
              
            }
          break;
          }

          case MOVER_MOTORES_SIMULTANEOS: {
            String x = data.substring(1);
    
            //código para separar as strings 
            int firstSeparatorIndex = x.indexOf(';');
            int secondSeparatorIndex = x.indexOf(';', firstSeparatorIndex + 1);
            int thirdSeparatorIndex = x.indexOf(';', secondSeparatorIndex + 1);
            int fourthSeparatorIndex = x.indexOf(';', thirdSeparatorIndex + 1);
            int fifthSeparatorIndex = x.indexOf(';', fourthSeparatorIndex + 1);
    
            // Extract substrings based on the positions of the separators
            String pulso1 = x.substring(0, firstSeparatorIndex); // "primeiro numero"
            String velocidade1 = x.substring(firstSeparatorIndex + 1, secondSeparatorIndex); // "segundo numero"
            String pulso2 = x.substring(secondSeparatorIndex + 1, thirdSeparatorIndex); // "terceiro numero"
            String velocidade2 = x.substring(thirdSeparatorIndex + 1, fourthSeparatorIndex); // "quarto numero"
            String direcao = x.substring(fourthSeparatorIndex + 1, fifthSeparatorIndex); // "caracter B ou C"
            String mover = x.substring(fifthSeparatorIndex + 1); // "caracter H ou x"

            Serial.print("Direção do motor: (case)" + direcao);

            digitalWrite(PIN_ENABLE_1, 1);
            digitalWrite(PIN_ENABLE_2, 1);

            qtdPulsosMotores1 = pulso1.toFloat();
            velocidadeMaxima1 = velocidade1.toFloat();
            qtdPulsosMotores2 = pulso2.toFloat();
            velocidadeMaxima2 = velocidade2.toFloat();
            
            /*if(direcao == "B"){
              //Serial.println("B"); // Printa a mensagem no aplicativo do vs code:: Direcão: Para baixo
              direcao1 = -1;
              direcao2 = 1;
            }
            else if(direcao == "C"){
                //Serial.println("C"); // Printa a mensagem no aplicativo do vs code:: Direcão: Para cima
              direcao1 = 1;
              direcao2 = -1;
            }*/

            if(mover.equals("H")){
              moverSimultaneo(motor1, motor2, qtdPulsosMotores1, qtdPulsosMotores2, velocidadeMaxima1, velocidadeMaxima2, direcao);
            }


            break;
          }
        }
      }
}