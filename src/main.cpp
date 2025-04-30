#include <Arduino.h>
#include "functions.h"
#include "macros.h"
#include "globals.h"
#include <AccelStepper.h>

//Declaração de variáveis para motores
AccelStepper* motor1 = nullptr; 
AccelStepper* motor2 = nullptr;


//Declaração de variáveis para paramêtros do motor
int velocidadeMaxima = 0, aceleracaoMaxima = 0, velocidade = 0;

void setup() {

  Serial.begin(115200);

  // Configura os pinos de ENABLE como saída
  pinMode(PIN_ENABLE_1, OUTPUT);
  pinMode(PIN_ENABLE_2, OUTPUT);

  // Desabilita os motores inicialmente (ENABLE em HIGH)
  //digitalWrite(PIN_ENABLE_1, LOW);
  //digitalWrite(PIN_ENABLE_2, LOW);

  //Configuração de velocidade e aceleração
  velocidadeMaxima = 8000;
  aceleracaoMaxima = 200;
  velocidade = 1000;

   // Configurações iniciais para o Motor 1
  motor1 = CriarMotor(PIN_PASSO_1, PIN_DIR_1, PIN_ENABLE_1, velocidadeMaxima, aceleracaoMaxima, velocidade);
  motor2 = CriarMotor(PIN_PASSO_2, PIN_DIR_2, PIN_ENABLE_2, velocidadeMaxima, aceleracaoMaxima, velocidade);
}

void loop() {

  //Chamada de funções
  VerificarSerial(motor1, motor2, velocidadeMaxima, aceleracaoMaxima, velocidade);
  
  if (emMovimento1) {
    motor1->run();
    if (motor1->distanceToGo() == 0) {
      emMovimento1 = false;
      motor1->disableOutputs();
      Serial.println("y");
    }
  }

  if (emMovimento2) {
    motor2->run();
    if (motor2->distanceToGo() == 0) {
      emMovimento2 = false;
      motor2->disableOutputs();
      Serial.println("Y");
    }
  }

  if(emMovimentoSimultaneo) {
    motor1->run();
    motor2->run();
    if (motor1->distanceToGo() == 0 || motor2->distanceToGo() ==0){
      emMovimentoSimultaneo = false;
      motor1->disableOutputs();
      motor2->disableOutputs();
      Serial.println("y");
    }
  }
}