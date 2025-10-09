#include "../include/functions.h"
#include "../include/macros.h"
#include "../include/globals.h"
#include <AccelStepper.h>

AccelStepper *CriarMotor(int stepPin, int dirPin, int enablePin, int velocidadeMaxima, int aceleracao, int velocidade)
{
    // Aloca dinamicamente um objeto AccelStepper
    AccelStepper *motor = new AccelStepper(AccelStepper::DRIVER, stepPin, dirPin);

    if (!motor)
    {

        return nullptr; // Retorna nullptr em caso de falha na alocação
    }

    // Configura os pinos do motor
    ConfigurarMotor(motor, enablePin, velocidadeMaxima, aceleracao, velocidade);

    return motor; // Retorna o ponteiro para o motor criado
}

// Configura os parâmetros do motor e habilita o driver.

void ConfigurarMotor(AccelStepper *motor, int enablePin, int velocidadeMaxima, int aceleracao, int velocidade)
{
    pinMode(enablePin, OUTPUT);
    digitalWrite(enablePin, LOW);

    motor->setMaxSpeed(velocidadeMaxima); // Velocidade máxima em passos/s
    motor->setAcceleration(aceleracao);   // Aceleração em passos/s²
    motor->setSpeed(velocidade);          // Velocidade inicial em passos/s
}

// Função para mover um motor (NÃO ESTÁ SENDO UTILIZADA)
int moverMotor(AccelStepper *motor, long distancia)
{
    if (!motor)
    { // Verifica se o ponteiro do motor é válido
        return ERROR_MOTOR_NAO_SELECIONADO;
    }
    motor->move(distancia); // Define a posição alvo
    while (motor->distanceToGo() != 0)
    { // Executa o movimento
        motor->run();
    }
    return OK;
}

// Função para mover o motor com aceleraçao
void moverAcelerado(AccelStepper *motor, long distancia, int velocidadeMaxima, int direcao)
{
    if (!motor)
    {           // Verifica se o ponteiro do motor é válido
        return; // Retorna sem fazer nada se o ponteiro for inválido
    }

    long posicaoInicial = motor->currentPosition();
    long posicaoFinal;

    motor->setAcceleration(1000);              // Define a aceleração em passos por segundo ao quadrado
    motor->setMaxSpeed(abs(velocidadeMaxima)); // Define a velocidade máxima em passos por segundo

    if (direcao == 1)
    {
        motor->setSpeed(abs(velocidadeMaxima));
        posicaoFinal = posicaoInicial + distancia;
    }
    else if (direcao == -1)
    {
        motor->setSpeed(-abs(velocidadeMaxima));
        posicaoFinal = posicaoInicial - distancia;
    }
    else
    {
        return; // Direção inválida, sai da função
    }

    motor->moveTo(posicaoFinal);

    while (motor->currentPosition() != posicaoFinal)
    {
        motor->run();
    }
}

void moverUniforme(AccelStepper *motor, double distancia, int velocidade, char direcao, int numMotor)
{
    if (!motor)
        return;

    String resposta;

    double posicaoInicial = motor->currentPosition();
    double posicaoDesejada;

    pararMotorSimultaneo = false;

    motor->setMaxSpeed(abs(velocidade)); // Define a velocidade máxima

    motor->enableOutputs(); // Ativa os motores

    if (direcao == 'B')
    { // Se direção for 1
        resposta = "horario";
        posicaoDesejada = posicaoInicial - distancia; // Motor 1
    }
    else if (direcao == 'C')
    { // Se direção for 0
        resposta = "antihorario";
        posicaoDesejada = posicaoInicial + distancia; // Motor 1
    }
    else
    {
        return; // Se direção inválida, sai da função
    }

    motor->setAcceleration(5000); // Define aceleração

    motor->moveTo(posicaoDesejada); // Move motor 1 para a posição desejada

    if (numMotor == 1)
        emMovimento1 = true;
    if (numMotor == 2)
        emMovimento2 = true;
}

void moverSimultaneo(AccelStepper *motor1, AccelStepper *motor2, float distancia1, float distancia2, float velocidadeMaxima1, float velocidadeMaxima2, char direcao, int tipoMotor)
{
    // Verifica se os motores são válidos
    String resposta;

    if ((!motor1) || (!motor2))
    {
        return;
    }

    double posicaoInicial1 = motor1->currentPosition();
    double posicaoInicial2 = motor2->currentPosition();
    double posicaoDesejada1, posicaoDesejada2;

    if (tipoMotor == 1)
    {
        pararMotorSimultaneo = false;
    }
    else if (tipoMotor == 2)
    {
        pararMotorFalha = false;
    }

    motor1->setMaxSpeed(abs(velocidadeMaxima1)); // Define a velocidade máxima
    motor2->setMaxSpeed(abs(velocidadeMaxima2)); // Define a velocidade máxima

    motor1->enableOutputs(); // Ativa os motores
    motor2->enableOutputs(); // Ativa os motores

    if (direcao == 'B')
    { // Se direção for 1
        resposta = "horario";
        posicaoDesejada1 = posicaoInicial1 - distancia1; // Motor 1
        posicaoDesejada2 = posicaoInicial2 - distancia2; // Motor 2
    }
    else if (direcao == 'C')
    { // Se direção for 0
        resposta = "antihorario";
        posicaoDesejada1 = posicaoInicial1 + distancia1; // Motor 1
        posicaoDesejada2 = posicaoInicial2 + distancia2; // Motor 2
    }
    else
    {
        return; // Se direção inválida, sai da função
    }

    motor1->setAcceleration(5000); // Define aceleração
    motor2->setAcceleration(5000); // Define aceleração

    motor1->moveTo(posicaoDesejada1); // Move motor 1 para a posição desejada
    motor2->moveTo(posicaoDesejada2); // Move motor 2 para a posição desejada

    if (tipoMotor == 1)
    {
        emMovimentoSimultaneo = true;
    }
    else if (tipoMotor == 2)
    {
        emMovimentoFalha = true;
    }
}

void moverUniversal(AccelStepper *motor1, AccelStepper *motor2, AccelStepper *motor3, AccelStepper *motor4,
                    float distancia1, float distancia2, float velocidade1, float velocidade2, char direcao1, char direcao2,
                    float distanciaUnidrecional, float velocidadeUnidirecional, char direcaoUnidirecional)
{

    if ((!motor1) || (!motor2) || (!motor3) || (!motor4))
    {
        return;
    }

    double posicaoInicial1 = motor1->currentPosition();
    double posicaoInicial2 = motor2->currentPosition();
    double posicaoInicial3 = motor3->currentPosition();
    double posicaoInicial4 = motor4->currentPosition();
    double posicaoDesejada1, posicaoDesejada2, posicaoDesejada3, posicaoDesejada4;

    pararMotorSimultaneo = false;

    motor1->setMaxSpeed(abs(velocidade1)); // Define a velocidade máxima
    motor2->setMaxSpeed(abs(velocidade2)); // Define a velocidade máxima
    motor3->setMaxSpeed(abs(velocidadeUnidirecional));
    motor4->setMaxSpeed(abs(velocidadeUnidirecional));

    motor1->enableOutputs(); // Ativa os motores
    motor2->enableOutputs(); // Ativa os motores
    motor3->enableOutputs();
    motor4->enableOutputs();

    if (direcao1 == 'B')
    { // Se direção for 1

        posicaoDesejada1 = posicaoInicial1 - distancia1; // Motor 1
        posicaoDesejada2 = posicaoInicial2 - distancia2; // Motor 2
    }
    else if (direcao1 == 'C')
    { // Se direção for 0

        posicaoDesejada1 = posicaoInicial1 + distancia1; // Motor 1
        posicaoDesejada2 = posicaoInicial2 + distancia2; // Motor 2
    }
    else
    {
        return; // Se direção inválida, sai da função
    }

    motor1->setAcceleration(5000); // Define aceleração
    motor2->setAcceleration(5000); // Define aceleração

    motor1->moveTo(posicaoDesejada1); // Move motor 1 para a posição desejada
    motor2->moveTo(posicaoDesejada2); // Move motor 2 para a posição desejada

    emMovimentoSimultaneo = true;
}

void calibracao() {}

void paraMotorSimultaneo(AccelStepper *motor1, AccelStepper *motor2, int tipoMotor)
{
    if ((!motor1) || (!motor2))
        return;

    if (tipoMotor == 1)
    {
        pararMotorSimultaneo = true;
    }
    else if (tipoMotor == 2)
    {
        pararMotorFalha = true;
    }

    // Para o motor instantaneamente definindo velocidade zero
    motor1->setSpeed(0);
    motor2->setSpeed(0);

    motor1->setCurrentPosition(0);
    motor2->setCurrentPosition(0);

    motor1->disableOutputs();
    motor2->disableOutputs();
}

void paraMotor1(AccelStepper *motor)
{
    if (!motor)
        return;

    pararMotor1 = true;

    motor->setSpeed(0);           // Para o motor imediatamente
    motor->setCurrentPosition(0); // Redefine a posição atual do motor para
    motor->disableOutputs();      // Desabilita as saídas do motor (desliga a energia)
}

void paraMotor2(AccelStepper *motor)
{
    if (!motor)
        return;

    pararMotor2 = true;

    motor->setSpeed(0);           // Para o motor imediatamente
    motor->setCurrentPosition(0); // Redefine a posição atual do motor para
    motor->disableOutputs();      // Desabilita as saídas do motor (desliga a energia)
}

void habilitarMotor(AccelStepper *motor, int enablePin)
{
    if (motor)
    {
        digitalWrite(enablePin, HIGH); // Habilita o motor
    }
}

// Função para desabilitar um motor
void desabilitarMotor(AccelStepper *motor, int enablePin)
{
    if (motor)
    {
        digitalWrite(enablePin, LOW); // Desabilita o motor
    }
}


void sensorIndutivo(AccelStepper* motor, int numMotor) {
    int valorLido1 = analogRead(SENSOR_INDUTIVO_MOTOR_1);
    int valorLido2 = analogRead(SENSOR_INDUTIVO_MOTOR_2);

    if(numMotor == 1){
        if(valorLido1 > limiarSensor){
            emMovimento1 = false;
        }
    } else {
        if(valorLido2 > limiarSensor){
            emMovimento2 = false;
        }
    }
}

void sensorIndutivoSimultaneo(AccelStepper* motor1, AccelStepper* motor2, int tipoMotor) {

    int valorLido1 = analogRead(SENSOR_INDUTIVO_MOTOR_1);
    int valorLido2 = analogRead(SENSOR_INDUTIVO_MOTOR_2);

    if(tipoMotor == 1){
        if(valorLido1 > limiarSensor){
            emMovimentoSimultaneo = false;
        }
    } else {
        if(valorLido2 > limiarSensor){
            emMovimentoFalha = false;
        }
    }
}