#include <Arduino.h>
#include "functions.h"
#include "macros.h"
#include "globals.h"
#include <AccelStepper.h>

// Versão final do firmware para o sistema de atuadores do LM2C
int commit;

// Declaração dos ponteiros para os objetos dos 4 motores de passo suportados pelo hardware
AccelStepper* motor1 = nullptr; 
AccelStepper* motor2 = nullptr;
AccelStepper* motor3 = nullptr;
AccelStepper* motor4 = nullptr;

// Definição das variáveis de parâmetros globais de desempenho (velocidade e aceleração)
int velocidadeMaxima = 0, aceleracaoMaxima = 0, velocidade = 0;

void setup() {
  // Inicializa a comunicação serial a 9600 bps para interface com LabVIEW ou VS Code
  Serial.begin(9600);

  // Configura os pinos físicos de ENABLE dos drivers como saídas digitais
  pinMode(PIN_ENABLE_1, OUTPUT);
  pinMode(PIN_ENABLE_2, OUTPUT);
  pinMode(PIN_ENABLE_3, OUTPUT);
  pinMode(PIN_ENABLE_4, OUTPUT);

  // Garante que os motores iniciam desabilitados (pino em nível lógico baixo conforme o hardware)
  digitalWrite(PIN_ENABLE_1, LOW);
  digitalWrite(PIN_ENABLE_2, LOW);
  digitalWrite(PIN_ENABLE_3, LOW);
  digitalWrite(PIN_ENABLE_4, LOW);

  // Configuração dos limites iniciais de operação dos motores
  velocidadeMaxima = 8000; // Limite superior de passos por segundo
  aceleracaoMaxima = 200;  // Taxa de aceleração padrão
  velocidade = 1000;       // Velocidade de cruzeiro padrão

  // Criação e parametrização inicial de cada motor usando as constantes de hardware de macros.h
  motor1 = CriarMotor(PIN_PASSO_1, PIN_DIR_1, PIN_ENABLE_1, velocidadeMaxima, aceleracaoMaxima, velocidade);
  motor2 = CriarMotor(PIN_PASSO_2, PIN_DIR_2, PIN_ENABLE_2, velocidadeMaxima, aceleracaoMaxima, velocidade);
  motor3 = CriarMotor(PIN_PASSO_3, PIN_DIR_3, PIN_ENABLE_3, velocidadeMaxima, aceleracaoMaxima, velocidade);
  motor4 = CriarMotor(PIN_PASSO_4, PIN_DIR_4, PIN_ENABLE_4, velocidadeMaxima, aceleracaoMaxima, velocidade);
}

void loop() {
  // Escuta e processa continuamente os comandos vindos da interface serial
  VerificarSerial(motor1, motor2, motor3, motor4, velocidadeMaxima, aceleracaoMaxima, velocidade);
  
  // Controle de execução do movimento individual do Motor 1
  if (emMovimento1) {
    // Se ativado pela interface, monitora o sensor indutivo para parada de segurança
    if(usarSensor){
      sensorIndutivo(motor1, 1);
    }
    motor1->run(); // Processa um passo do motor se necessário (não bloqueante)
    if (motor1->distanceToGo() == 0) { // Verifica se atingiu a posição de destino
      emMovimento1 = false;
      motor1->disableOutputs(); // Desliga o driver para poupar energia e evitar aquecimento
      Serial.println('y');      // Envia confirmação de conclusão do Motor 1 para a interface
    }
  }

  // Controle de execução do movimento individual do Motor 2
  if (emMovimento2) {
    if(usarSensor){
      sensorIndutivo(motor2, 2);
    }
    motor2->run();
    if (motor2->distanceToGo() == 0) {
      emMovimento2 = false;
      motor2->disableOutputs();
      Serial.println('Y'); // Envia confirmação de conclusão do Motor 2 para a interface
    }
  }

  // Lógica para controle de movimento simultâneo coordenado entre os Motores 1 e 2
  if(emMovimentoSimultaneo) {
    if(usarSensor){
      sensorIndutivoSimultaneo(motor1, motor2, 1);
    }
    motor1->run();
    motor2->run();
    // Interrompe o estado de movimento se qualquer um dos dois motores atingir seu destino
    if (motor1->distanceToGo() == 0 || motor2->distanceToGo() == 0){
      emMovimentoSimultaneo = false;
      motor1->disableOutputs();
      motor2->disableOutputs();
      Serial.println('y'); // Feedback de conclusão para o modo simultâneo
    }
  }

  // Lógica para controle de movimento simultâneo coordenado entre os Motores de Falha (3 e 4)
  if(emMovimentoFalha) {
    if(usarSensor){
      sensorIndutivoSimultaneo(motor3, motor4, 2);
    }
    motor3->run();
    motor4->run();
    if (motor3->distanceToGo() == 0 || motor4->distanceToGo() == 0){
      emMovimentoFalha = false;
      motor3->disableOutputs();
      motor4->disableOutputs();
      Serial.println('Y'); // Feedback de conclusão para o modo de falha
    }
  }
}