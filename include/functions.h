#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <AccelStepper.h>

// Configurações iniciais para os Motores 
int ConfigurarMotor (AccelStepper* numeroMotor, int velocidadeMaximaMotor, int aceleracaoMotor, int velocidadeMotor); 

// Função para criar um motor
AccelStepper* CriarMotor(int stepPin, int dirPin, int enablePin, int velocidadeMaxima, int aceleracao, int velocidade);

// Função para mover um motor
int MoverMotor(AccelStepper* motor, long distancia);

// Função para desabilitar um motor
void DesabilitarMotor(AccelStepper* motor, int enablePin);

void VerificarSerial(AccelStepper* motor1, AccelStepper* motor2, int velocidadeMaxima, int velocidade, int aceleracaoMaxima);

void moverMotor();

void moverAcelerado(AccelStepper* motor, long distancia, int velocidadeMaxima, int direcao);

void moverUniforme(AccelStepper* motor, long distancia, int velocidadeMaxima, int direcao);

void moverSimultaneo(AccelStepper* motor1, AccelStepper* motor2, int distancia1, int distancia2, int velocidadeMaxima1, int velocidadeMaxima2, String direcao);

void paraMotorSimultaneo(AccelStepper* motor1, AccelStepper* motor2);

void paraMotor(AccelStepper* motor);

void calibracao();

void subsidencia(AccelStepper* motor, int velocidadeMaxima, int aceleracao, long distancia);

#endif