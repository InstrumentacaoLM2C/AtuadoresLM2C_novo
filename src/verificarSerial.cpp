#include "../include/functions.h"
#include "../include/macros.h"
#include "../include/globals.h"

// Função principal de processamento da porta Serial
void VerificarSerial(AccelStepper *motor1, AccelStepper *motor2, AccelStepper *motor3, AccelStepper *motor4, int velocidadeMaxima, int velocidade, int aceleracaoMaxima)
{
  // Definição de variáveis locais para armazenamento de dados recebidos e processados
  String posicao_calculadaStr1, posicao_calculadaStr2;

  int aceleracao1, aceleracao2, posicao_calculada1, posicao_calculada2, parar_calibracao,
      constanteCalibracao1, constanteCalibracao2, motorParou1;

  float qtdPulsosMotor1, qtdPulsosMotor2, qtdPulsosMotores1, qtdPulsosMotores2, velocidadeMaxima1, velocidadeMaxima2, pulsosMotor1,
      pulsosMotor2, pulsoUnidirecional, velMotor1, velMotor2, velUnidirecional;

  float receivedPulsesDistance1, receivedPulsesDistance2,
      receivedDelay1, receivedDelay2, zero_laser;

  // Variáveis estáticas e de estado de direção
  static int motor = 1;
  int direcao1 = 1;
  int direcao2 = 1;
  int direcaoMotor = 1;

  // Verifica se existem bytes pendentes na leitura serial
  if (Serial.available())
  {
    // Lê a string enviada pela interface (ex: LabVIEW) até o caractere terminador '#'
    String data = Serial.readStringUntil('#');
    char digitoUm = data.charAt(0); // Identifica o comando principal pelo primeiro caractere

    switch (digitoUm)
    { 
    case LIGAR_MOTOR: // Caso 'A': Ativa o driver do motor selecionado (pino ENABLE)
    { 
      if (motor == MOTOR_1)
      {
        digitalWrite(PIN_ENABLE_1, HIGH);
      }
      else if (motor == MOTOR_2)
      {
        digitalWrite(PIN_ENABLE_2, HIGH);
      }
      else if (motor == MOTORES_SIMULTANEOS)
      {
        digitalWrite(PIN_ENABLE_1, HIGH);
        digitalWrite(PIN_ENABLE_2, HIGH);
      }
      else if (motor == MOTORES_FALHA)
      {
        digitalWrite(PIN_ENABLE_3, HIGH);
        digitalWrite(PIN_ENABLE_4, HIGH);
      }
      break;
    }

    case DESLIGAR_MOTOR: // Caso 'a': Desativa o driver do motor selecionado (pino ENABLE)
      if (motor == MOTOR_1)
      {
        digitalWrite(PIN_ENABLE_1, LOW);
      }
      else if (motor == MOTOR_2)
      {
        digitalWrite(PIN_ENABLE_2, LOW);
      }
      else if (motor == MOTORES_SIMULTANEOS)
      {
        digitalWrite(PIN_ENABLE_1, LOW);
        digitalWrite(PIN_ENABLE_2, LOW);
      }
      else if (motor == MOTORES_FALHA)
      {
        digitalWrite(PIN_ENABLE_3, LOW);
        digitalWrite(PIN_ENABLE_4, LOW);
      }
      break;

    case MOVER_MOTOR_CIMA: // Caso para definir a direção de subida
      if (motor == MOTOR_1)
      {
        direcao1 = 1;
        direcaoMotor = 1;
      }
      else if (motor == MOTOR_2)
      {
        direcao2 = 1;
        direcaoMotor = -1;
      }
      else if (motor == MOTORES_SIMULTANEOS)
      {
        direcao1 = 1;
        direcao2 = -1;
      }
      break;

    case MOVER_MOTOR_BAIXO: // Caso para definir a direção de descida
      if (motor == MOTOR_1)
      {
        direcao1 = 1;
        direcaoMotor = 1;
      }
      else if (motor == MOTOR_2)
      {
        direcao2 = 1;
        direcaoMotor = -1;
      }
      else if (motor == MOTORES_SIMULTANEOS)
      {
        direcao1 = 1;
        direcao2 = 1;
      }
      break;

    case QTD_PULSOS: // Caso 'P': Recebe o valor numérico para o deslocamento
    { 
      String info_qtd_pulsos = data.substring(1);
      if (motor == '1')
      {
        qtdPulsosMotor1 = info_qtd_pulsos.toFloat(); 
      }
      else if (motor == '2')
      {
        qtdPulsosMotor2 = info_qtd_pulsos.toFloat(); 
      }
      break;
    }

    case ACELERAR_MOTOR: // Caso 'G': Inicia movimento com rampa de aceleração
      if (motor == '1')
      {
        moverAcelerado(motor1, qtdPulsosMotor1, velocidadeMaxima, direcaoMotor);
      }
      else if (motor == '2')
      {
        moverAcelerado(motor2, qtdPulsosMotor2, velocidadeMaxima, direcaoMotor);
      }
      break;

    case MSG_MOTOR_MOVENDO_COM_ACELERACAO: // Sinaliza flag de aceleração ativa
      if (motor == '1') aceleracao1 = 1;
      else if (motor == '2') aceleracao2 = 1;
      break;

    case MSG_MOTOR_MOVENDO_SEM_ACELERACAO: // Sinaliza flag de aceleração inativa
      if (motor == '1') aceleracao1 = 0;
      else if (motor == '2') aceleracao2 = 0;
      break;

    case ATIVAR_MOTOR_VEL_CTE: // Caso 'H': Inicia movimento com velocidade constante
      if (motor == '1')
      {
        moverUniforme(motor1, qtdPulsosMotor1, velocidadeMaxima, direcaoMotor, 1);
      }
      else if (motor == '2')
      {
        moverUniforme(motor2, qtdPulsosMotor2, velocidadeMaxima, direcaoMotor, 2);
      }
      break;

    case DEFINIR_VELOCIDADE: // Caso 'V': Ajusta o valor da velocidade em passos/segundo
    { 
      String x = data.substring(1);
      float y = x.toFloat();
      if (200 < y < 8000)
      {
        if (motor == '1') receivedDelay1 = y;
        else if (motor == '2') receivedDelay2 = y;
      }
      break;
    }

    case PARAR_MOTOR: // Caso 'n': Interrompe o motor imediatamente
      if (motor == 1)
      {
        paraMotor1(motor1);
        pararMotor1 = true;
      }
      else if (motor == 2)
      {
        paraMotor2(motor2);
        pararMotor2 = true;
      }
      else if (motor == 3)
      {
        paraMotorSimultaneo(motor1, motor2, 1);
      }
      else if (motor == 4)
      {
        paraMotorSimultaneo(motor3, motor4, 2);
      }
      break;

    case PARAR_CALIBRACAO: // Interrompe processo de calibração
      parar_calibracao = 1;
      break;

    case POSICAO_MOTOR_1: // Recebe posição calculada para o motor 1
    { 
      delayMicroseconds(1000);
      String x = data.substring(1);
      posicao_calculada1 = x.toFloat();
      delayMicroseconds(1000);
      posicao_calculadaStr1 = String(posicao_calculada1);
      break;
    }

    case POSICAO_MOTOR_2: // Recebe posição calculada para o motor 2
    { 
      delayMicroseconds(1000);
      String x = data.substring(1);
      posicao_calculada2 = x.toFloat();
      delayMicroseconds(1000);
      posicao_calculadaStr2 = String(posicao_calculada2);
      break;
    }

    case CONFIGURAR_ZERO_LASER: // Ajusta o ponto zero do sensor laser
    { 
      String x = data.substring(1);
      zero_laser = x.toFloat();
      break;
    }

    case INICIAR_CALIBRACAO: // Inicia rotina de calibração
      calibracao();
      break;

    case INSERIR_CONSTANTES_CALIBRACAO: // Recebe constantes para cálculos de precisão
    { 
      String x = data.substring(1);
      if (motor == 1) constanteCalibracao1 = x.toFloat();
      else if (motor == 2) constanteCalibracao2 = x.toFloat();
      else if (motor == 3)
      {
        constanteCalibracao1 = x.toFloat();
        constanteCalibracao2 = x.toFloat();
      }
      Serial.print('w'); 
      Serial.println(x);
      break;
    }

    case ATIVAR_SENSOR_INDUTIVO: // Ativa monitoramento de fim de curso via sensor
      usarSensor = true;
      break;

    case DESATIVAR_SENSOR_INDUTIVO: // Desativa monitoramento de fim de curso via sensor
      usarSensor = false;
      break;

    case ALTERAR_PARA_MOTOR_2: // Seleciona Motor 2 para controle
      motor = 2;
      break;

    case ALTERAR_PARA_MOTOR_1: // Seleciona Motor 1 para controle
      motor = 1;
      break;

    case SUBSIDENCIA: // Espaço reservado para função de subsidência
      break;

    case ALTERAR_PARA_MOTORES_SIMULTANEOS: // Seleciona modo de operação Motores 1 & 2
    {
      motor = MOTORES_SIMULTANEOS;
      break;
    }

    case ALTERAR_PARA_MOTORES_FALHA: // Seleciona modo de operação Motores 3 & 4
    {
      motor = MOTORES_FALHA;
      break;
    }

    case ENVIAR_CONFIG_COMPLETA: // Caso 'T': Recebe string complexa formatada com ';'
    { 
      String x = data.substring(1);
      int firstSeparatorIndex = x.indexOf(';');
      int secondSeparatorIndex = x.indexOf(';', firstSeparatorIndex + 1);
      int thirdSeparatorIndex = x.indexOf(';', secondSeparatorIndex + 1);

      String pulso = x.substring(0, firstSeparatorIndex);
      String velocidade = x.substring(firstSeparatorIndex + 1, secondSeparatorIndex);
      String direcaoMotor = x.substring(secondSeparatorIndex + 1, thirdSeparatorIndex);
      String mover = x.substring(thirdSeparatorIndex + 1);

      char direcao = direcaoMotor[0];

      if (motor == 1)
      {
        qtdPulsosMotor1 = pulso.toFloat();
        velocidadeMaxima = velocidade.toFloat();
        if (mover == "H") moverUniforme(motor1, qtdPulsosMotor1, velocidadeMaxima, direcao, 1);
      }
      else if (motor == 2)
      {
        qtdPulsosMotor2 = pulso.toFloat();
        velocidadeMaxima = velocidade.toFloat();
        if (mover == "H") moverUniforme(motor2, qtdPulsosMotor2, velocidadeMaxima, direcao, 2);
      }
      break;
    }

    case MOVER_MOTORES_SIMULTANEOS: // Comando 'W': Movimenta motores 1 e 2 simultaneamente com parâmetros individuais
    {
      String x = data.substring(1);
      int firstSeparatorIndex = x.indexOf(';');
      int secondSeparatorIndex = x.indexOf(';', firstSeparatorIndex + 1);
      int thirdSeparatorIndex = x.indexOf(';', secondSeparatorIndex + 1);
      int fourthSeparatorIndex = x.indexOf(';', thirdSeparatorIndex + 1);
      int fifthSeparatorIndex = x.indexOf(';', fourthSeparatorIndex + 1);

      String pulso1 = x.substring(0, firstSeparatorIndex);
      String velocidade1 = x.substring(firstSeparatorIndex + 1, secondSeparatorIndex);
      String pulso2 = x.substring(secondSeparatorIndex + 1, thirdSeparatorIndex);
      String velocidade2 = x.substring(thirdSeparatorIndex + 1, fourthSeparatorIndex);
      String direcao = x.substring(fourthSeparatorIndex + 1, fifthSeparatorIndex);
      String mover = x.substring(fifthSeparatorIndex + 1);

      digitalWrite(PIN_ENABLE_1, HIGH);
      digitalWrite(PIN_ENABLE_2, HIGH);

      qtdPulsosMotores1 = pulso1.toFloat();
      velocidadeMaxima1 = velocidade1.toFloat();
      qtdPulsosMotores2 = pulso2.toFloat();
      velocidadeMaxima2 = velocidade2.toFloat();
      char direcaoChar = direcao[0];

      if (mover.equals("H"))
      {
        moverSimultaneo(motor1, motor2, qtdPulsosMotores1, qtdPulsosMotores2, velocidadeMaxima1, velocidadeMaxima2, direcaoChar, 1);
      }
      break;
    }

    case MOVER_MOTORES_FALHA: // Comando 'L': Movimenta motores 3 e 4 simultaneamente
    {
      String x = data.substring(1);
      int firstSeparatorIndex = x.indexOf(';');
      int secondSeparatorIndex = x.indexOf(';', firstSeparatorIndex + 1);
      int thirdSeparatorIndex = x.indexOf(';', secondSeparatorIndex + 1);
      int fourthSeparatorIndex = x.indexOf(';', thirdSeparatorIndex + 1);
      int fifthSeparatorIndex = x.indexOf(';', fourthSeparatorIndex + 1);

      String pulso1 = x.substring(0, firstSeparatorIndex);
      String velocidade1 = x.substring(firstSeparatorIndex + 1, secondSeparatorIndex);
      String pulso2 = x.substring(secondSeparatorIndex + 1, thirdSeparatorIndex);
      String velocidade2 = x.substring(thirdSeparatorIndex + 1, fourthSeparatorIndex);
      String direcao = x.substring(fourthSeparatorIndex + 1, fifthSeparatorIndex);
      String mover = x.substring(fifthSeparatorIndex + 1);

      digitalWrite(PIN_ENABLE_3, HIGH);
      digitalWrite(PIN_ENABLE_4, HIGH);

      qtdPulsosMotores1 = pulso1.toFloat();
      velocidadeMaxima1 = velocidade1.toFloat();
      qtdPulsosMotores2 = pulso2.toFloat();
      velocidadeMaxima2 = velocidade2.toFloat();
      char direcaoChar = direcao[0];

      if (mover.equals("H"))
      {
        moverSimultaneo(motor3, motor4, qtdPulsosMotores1, qtdPulsosMotores2, velocidadeMaxima1, velocidadeMaxima2, direcaoChar, 2);
      }
      break;
    }

    case MOVER_MOTOR_UNIVERSAL: // Comando 'Z': Controle coordenado dos 4 motores ao mesmo tempo
    {
      String x = data.substring(1);
      int firstSeparatorIndex = x.indexOf(';');
      int secondSeparatorIndex = x.indexOf(';', firstSeparatorIndex + 1);
      int thirdSeparatorIndex = x.indexOf(';', secondSeparatorIndex + 1);
      int fourthSeparatorIndex = x.indexOf(';', thirdSeparatorIndex + 1);
      int fifthSeparatorIndex = x.indexOf(';', fourthSeparatorIndex + 1);
      int sixthSeparatorIndex = x.indexOf(';', fifthSeparatorIndex + 1);
      int seventhSeparatorIndex = x.indexOf(';', sixthSeparatorIndex + 1);
      int eigthSeparatorIndex = x.indexOf(';', seventhSeparatorIndex + 1);
      int ninethSeparatorIndex = x.indexOf(';', seventhSeparatorIndex + 1);

      String pulsoBidirecional1 = x.substring(0, firstSeparatorIndex);
      String velocidadeBidirecional1 = x.substring(firstSeparatorIndex + 1, secondSeparatorIndex);
      String pulsoBidirecional2 = x.substring(secondSeparatorIndex + 1, thirdSeparatorIndex);
      String velocidadeBidirecional2 = x.substring(thirdSeparatorIndex + 1, fourthSeparatorIndex);
      String direcaoVertical = x.substring(fourthSeparatorIndex + 1, fifthSeparatorIndex);
      String direcaoHorizontal = x.substring(fifthSeparatorIndex + 1, sixthSeparatorIndex);
      String pulsosUnidirecional = x.substring(sixthSeparatorIndex + 1, seventhSeparatorIndex);
      String velocidadeUnidirecional = x.substring(seventhSeparatorIndex + 1, eigthSeparatorIndex);
      String direcaoUnidirecional = x.substring(eigthSeparatorIndex + 1, ninethSeparatorIndex);

      digitalWrite(PIN_ENABLE_1, HIGH);
      digitalWrite(PIN_ENABLE_2, HIGH);
      digitalWrite(PIN_ENABLE_3, HIGH);
      digitalWrite(PIN_ENABLE_4, HIGH);

      pulsosMotor1 = pulsoBidirecional1.toFloat();
      velMotor1 = velocidadeBidirecional1.toFloat();
      pulsosMotor2 = pulsoBidirecional2.toFloat();
      velMotor2 = velocidadeBidirecional2.toFloat();
      pulsoUnidirecional = pulsosUnidirecional.toFloat();
      velUnidirecional = velocidadeUnidirecional.toFloat();
      char direcaoMotor1 = direcaoVertical[0];
      char direcaoMotor2 = direcaoHorizontal[0];
      char direcaoMotorUni = direcaoUnidirecional[0];

      moverUniversal(motor1, motor2, motor3, motor4, pulsosMotor1, pulsosMotor2, velMotor1, velMotor2, direcaoMotor1, direcaoMotor2,
                     pulsoUnidirecional, velUnidirecional, direcaoMotorUni);
      break;
    }
    }
  }
}