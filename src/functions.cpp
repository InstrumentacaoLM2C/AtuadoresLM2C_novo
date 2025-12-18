#include "../include/functions.h"
#include "../include/macros.h"
#include "../include/globals.h"
#include <AccelStepper.h>

/**
 * @brief Cria e inicializa dinamicamente um objeto AccelStepper.
 * @return Ponteiro para o motor criado ou nullptr em caso de falha.
 */
AccelStepper *CriarMotor(int stepPin, int dirPin, int enablePin, int velocidadeMaxima, int aceleracao, int velocidade)
{
    // Aloca dinamicamente o objeto usando o driver do tipo DRIVER (Step/Dir)
    AccelStepper *motor = new AccelStepper(AccelStepper::DRIVER, stepPin, dirPin);

    if (!motor)
    {
        return nullptr; // Falha na alocação de memória
    }

    // Aplica as configurações iniciais de pinagem e performance
    ConfigurarMotor(motor, enablePin, velocidadeMaxima, aceleracao, velocidade);

    return motor;
}

/**
 * @brief Configura pinos de hardware e parâmetros iniciais de movimento.
 */
void ConfigurarMotor(AccelStepper *motor, int enablePin, int velocidadeMaxima, int aceleracao, int velocidade)
{
    pinMode(enablePin, OUTPUT);
    digitalWrite(enablePin, LOW); // Garante motor inicialmente desabilitado

    motor->setMaxSpeed(velocidadeMaxima); // Limite de velocidade (passos/s)
    motor->setAcceleration(aceleracao);   // Limite de aceleração (passos/s²)
    motor->setSpeed(velocidade);          // Velocidade de regime inicial
}

/**
 * @brief Função para mover um motor de forma bloqueante.
 * @note NÃO ESTÁ SENDO UTILIZADA no fluxo principal.
 */
int moverMotor(AccelStepper *motor, long distancia)
{
    if (!motor)
    {
        return ERROR_MOTOR_NAO_SELECIONADO;
    }
    motor->move(distancia); // Define alvo relativo
    while (motor->distanceToGo() != 0)
    {
        motor->run(); // Executa o movimento até o fim (bloqueante)
    }
    return OK;
}

/**
 * @brief Move o motor utilizando rampas de aceleração.
 */
void moverAcelerado(AccelStepper *motor, long distancia, int velocidadeMaxima, int direcao)
{
    if (!motor) return;

    long posicaoInicial = motor->currentPosition();
    long posicaoFinal;

    motor->setAcceleration(1000);              
    motor->setMaxSpeed(abs(velocidadeMaxima)); 

    // Define a posição final com base no multiplicador de direção
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
        return; 
    }

    motor->moveTo(posicaoFinal);

    // Loop bloqueante até atingir a posição final
    while (motor->currentPosition() != posicaoFinal)
    {
        motor->run();
    }
}

/**
 * @brief Inicia o movimento uniforme (não bloqueante) para um motor específico.
 * @param numMotor Identificador do motor (1 ou 2) para atualizar flags globais.
 */
void moverUniforme(AccelStepper *motor, double distancia, int velocidade, char direcao, int numMotor)
{
    if (!motor) return;

    double posicaoInicial = motor->currentPosition();
    double posicaoDesejada;

    pararMotorSimultaneo = false;

    motor->setMaxSpeed(abs(velocidade)); 
    motor->enableOutputs(); // Ativa os drivers de potência

    // Lógica de direção baseada nos caracteres 'B' (Baixo) e 'C' (Cima)
    if (direcao == 'B')
    {
        posicaoDesejada = posicaoInicial - distancia; 
    }
    else if (direcao == 'C')
    {
        posicaoDesejada = posicaoInicial + distancia; 
    }
    else
    {
        return; 
    }

    motor->setAcceleration(5000); 
    motor->moveTo(posicaoDesejada); 

    // Ativa flags que serão processadas no loop() do main.cpp
    if (numMotor == 1) emMovimento1 = true;
    if (numMotor == 2) emMovimento2 = true;
}

/**
 * @brief Gerencia o movimento coordenado de dois motores simultaneamente.
 * @param tipoMotor Define se é o par principal (1) ou o par de falha (2).
 */
void moverSimultaneo(AccelStepper *motor1, AccelStepper *motor2, float distancia1, float distancia2, float velocidadeMaxima1, float velocidadeMaxima2, char direcao, int tipoMotor)
{
    if ((!motor1) || (!motor2)) return;

    double posicaoInicial1 = motor1->currentPosition();
    double posicaoInicial2 = motor2->currentPosition();
    double posicaoDesejada1, posicaoDesejada2;

    // Reseta flags de interrupção
    if (tipoMotor == 1) pararMotorSimultaneo = false;
    else if (tipoMotor == 2) pararMotorFalha = false;

    motor1->setMaxSpeed(abs(velocidadeMaxima1));
    motor2->setMaxSpeed(abs(velocidadeMaxima2));

    motor1->enableOutputs();
    motor2->enableOutputs();

    // Lógica de direção para o par Principal (Tipo 1)
    if (tipoMotor == 1)
    {
        if (direcao == 'B')
        {
            posicaoDesejada1 = posicaoInicial1 - distancia1;
            posicaoDesejada2 = posicaoInicial2 - distancia2;
        }
        else if (direcao == 'C')
        {
            posicaoDesejada1 = posicaoInicial1 + distancia1;
            posicaoDesejada2 = posicaoInicial2 + distancia2;
        }
        else return;
    } 
    // Lógica de direção para o par de Falha (Tipo 2)
    else if(tipoMotor == 2)
    {
        if (direcao == 'B')
        {
            posicaoDesejada1 = posicaoInicial1 - distancia1;
            posicaoDesejada2 = posicaoInicial2 + distancia2;
        }
        else if (direcao == 'C')
        {
            posicaoDesejada1 = posicaoInicial1 + distancia1;
            posicaoDesejada2 = posicaoInicial2 - distancia2;
        }
        else return;
    }

    motor1->setAcceleration(5000);
    motor2->setAcceleration(5000);

    motor1->moveTo(posicaoDesejada1);
    motor2->moveTo(posicaoDesejada2);

    // Sinaliza movimento para o loop principal
    if (tipoMotor == 1) emMovimentoSimultaneo = true;
    else if (tipoMotor == 2) emMovimentoFalha = true;
}

/**
 * @brief Função experimental para controle dos 4 motores ao mesmo tempo.
 * @note Atualmente apenas para testes e aplicações futuras.
 */
void moverUniversal(AccelStepper *motor1, AccelStepper *motor2, AccelStepper *motor3, AccelStepper *motor4,
                    float distancia1, float distancia2, float velocidade1, float velocidade2, char direcao1, char direcao2,
                    float distanciaUnidrecional, float velocidadeUnidirecional, char direcaoUnidirecional)
{
    if ((!motor1) || (!motor2) || (!motor3) || (!motor4)) return;

    double posicaoInicial1 = motor1->currentPosition();
    double posicaoInicial2 = motor2->currentPosition();
    double posicaoDesejada1, posicaoDesejada2;

    pararMotorSimultaneo = false;

    motor1->setMaxSpeed(abs(velocidade1));
    motor2->setMaxSpeed(abs(velocidade2));
    motor3->setMaxSpeed(abs(velocidadeUnidirecional));
    motor4->setMaxSpeed(abs(velocidadeUnidirecional));

    motor1->enableOutputs();
    motor2->enableOutputs();
    motor3->enableOutputs();
    motor4->enableOutputs();

    if (direcao1 == 'B')
    {
        posicaoDesejada1 = posicaoInicial1 - distancia1;
        posicaoDesejada2 = posicaoInicial2 - distancia2;
    }
    else if (direcao1 == 'C')
    {
        posicaoDesejada1 = posicaoInicial1 + distancia1;
        posicaoDesejada2 = posicaoInicial2 + distancia2;
    }
    else return;

    motor1->setAcceleration(5000);
    motor2->setAcceleration(5000);

    motor1->moveTo(posicaoDesejada1);
    motor2->moveTo(posicaoDesejada2);

    emMovimentoSimultaneo = true;
}

void calibracao() {} // Espaço reservado para rotina de calibração

/**
 * @brief Para o par de motores e redefine a posição zero.
 */
void paraMotorSimultaneo(AccelStepper *motor1, AccelStepper *motor2, int tipoMotor)
{
    if ((!motor1) || (!motor2)) return;

    if (tipoMotor == 1) pararMotorSimultaneo = true;
    else if (tipoMotor == 2) pararMotorFalha = true;

    motor1->setSpeed(0);
    motor2->setSpeed(0);
    motor1->setCurrentPosition(0);
    motor2->setCurrentPosition(0);
    motor1->disableOutputs();
    motor2->disableOutputs();
}

/**
 * @brief Interrompe imediatamente o Motor 1.
 */
void paraMotor1(AccelStepper *motor)
{
    if (!motor) return;
    pararMotor1 = true;
    motor->setSpeed(0);
    motor->setCurrentPosition(0);
    motor->disableOutputs();
}

/**
 * @brief Interrompe imediatamente o Motor 2.
 */
void paraMotor2(AccelStepper *motor)
{
    if (!motor) return;
    pararMotor2 = true;
    motor->setSpeed(0);
    motor->setCurrentPosition(0);
    motor->disableOutputs();
}

/**
 * @brief Aciona o pino de habilitação do driver.
 */
void habilitarMotor(AccelStepper *motor, int enablePin)
{
    if (motor) digitalWrite(enablePin, HIGH);
}

/**
 * @brief Desliga o pino de habilitação do driver.
 */
void desabilitarMotor(AccelStepper *motor, int enablePin)
{
    if (motor) digitalWrite(enablePin, LOW);
}

/**
 * @brief Monitora sensores indutivos para interromper movimento em caso de detecção.
 */
void sensorIndutivo(AccelStepper *motor, int numMotor)
{
    int valorLido1 = analogRead(SENSOR_INDUTIVO_MOTOR_1);
    int valorLido2 = analogRead(SENSOR_INDUTIVO_MOTOR_2);

    if (numMotor == 1)
    {
        if (valorLido1 > limiarSensor) emMovimento1 = false;
    }
    else
    {
        if (valorLido2 > limiarSensor) emMovimento2 = false;
    }
}

/**
 * @brief Monitora sensores indutivos durante movimentos de pares de motores.
 */
void sensorIndutivoSimultaneo(AccelStepper *motor1, AccelStepper *motor2, int tipoMotor)
{
    int valorLido1 = analogRead(SENSOR_INDUTIVO_MOTOR_1);
    int valorLido2 = analogRead(SENSOR_INDUTIVO_MOTOR_2);

    if (tipoMotor == 1)
    {
        if (valorLido1 > limiarSensor) emMovimentoSimultaneo = false;
    }
    else
    {
        if (valorLido2 > limiarSensor) emMovimentoFalha = false;
    }
}