#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <AccelStepper.h>

// Configurações iniciais para os Motores 
int ConfigurarMotor (AccelStepper* numeroMotor, int velocidadeMaximaMotor, int aceleracaoMotor, int velocidadeMotor); 

// Função para criar um motor
AccelStepper* CriarMotor(int stepPin, int dirPin, int enablePin, int velocidadeMaxima, int aceleracao, int velocidade);

void ConfigurarMotor(AccelStepper* motor, int enablePin, int velocidadeMaxima, int aceleracao, int velocidade);

// Função para mover um motor
int MoverMotor(AccelStepper* motor, long distancia);

// Função para desabilitar um motor
void DesabilitarMotor(AccelStepper* motor, int enablePin);

void VerificarSerial(AccelStepper* motor1, AccelStepper* motor2, AccelStepper* motor3, AccelStepper* motor4, int velocidadeMaxima, int velocidade, int aceleracaoMaxima);

void moverMotor();

void moverAcelerado(AccelStepper* motor, long distancia, int velocidadeMaxima, int direcao);

void moverUniforme(AccelStepper* motor, double distancia, int velocidadeMaxima, char direcao, int numMotor);

void moverSimultaneo(AccelStepper* motor1, AccelStepper* motor2, float distancia1, float distancia2, float velocidadeMaxima1, float velocidadeMaxima2, char direcao, int tipoMotor);

void moverUniversal(AccelStepper* motor1, AccelStepper* motor2, AccelStepper* motor3, AccelStepper* motor4, 
    float distancia1, float distancia2, float velocidade1, float velocidade2, char direcao1, char direcao2,
    float distanciaUnidrecional, float velocidadeUnidrecional, char direcaoUnidirecional);

void paraMotorSimultaneo(AccelStepper* motor1, AccelStepper* motor2, int tipoMotor);

void paraMotor1(AccelStepper* motor);

void paraMotor2(AccelStepper* motor);

void sensorIndutivo(AccelStepper* motor, int numMotor);

void sensorIndutivoSimultaneo(AccelStepper* motor1, AccelStepper* motor2, int tipoMotor);

void calibracao();

void subsidencia(AccelStepper* motor, int velocidadeMaxima, int aceleracao, long distancia);

void AtualizarMovimentoDosMotores(AccelStepper* motor1, AccelStepper* motor2);

#endif