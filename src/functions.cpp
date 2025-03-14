#include "../include/functions.h"
#include "../include/macros.h"
#include "../include/globals.h"
#include <AccelStepper.h>


AccelStepper* CriarMotor(int stepPin, int dirPin, int enablePin, int velocidadeMaxima, int aceleracao, int velocidade) {
    // Aloca dinamicamente um objeto AccelStepper
    AccelStepper* motor = new AccelStepper(AccelStepper::DRIVER, stepPin, dirPin);
    
    if (!motor) {
        Serial.println("Erro: Falha ao alocar memória para o motor!");
        return nullptr; // Retorna nullptr em caso de falha na alocação
    }

    // Configura os pinos do motor
    ConfigurarMotor(motor, enablePin, velocidadeMaxima, aceleracao, velocidade);

    return motor; // Retorna o ponteiro para o motor criado
}

// Configura os parâmetros do motor e habilita o driver.

void ConfigurarMotor(AccelStepper* motor, int enablePin, int velocidadeMaxima, int aceleracao, int velocidade) {
    pinMode(enablePin, OUTPUT);
    digitalWrite(enablePin, LOW); // Habilita o motor

    motor->setMaxSpeed(velocidadeMaxima); // Velocidade máxima em passos/s
    motor->setAcceleration(aceleracao);   // Aceleração em passos/s²
    motor->setSpeed(velocidade);          // Velocidade inicial em passos/s
}

// Função para mover um motor (NÃO ESTÁ SENDO UTILIZADA)
int moverMotor(AccelStepper* motor, long distancia) {
    if (!motor) { // Verifica se o ponteiro do motor é válido
        return ERROR_MOTOR_NAO_SELECIONADO;
    }
    motor->move(distancia); // Define a posição alvo
    while (motor->distanceToGo() != 0) { // Executa o movimento
        motor->run();
    }
    return OK;
}

// Função para mover o motor com aceleraçao
void moverAcelerado(AccelStepper* motor, long distancia, int velocidadeMaxima, int direcao) {
    if (!motor) { // Verifica se o ponteiro do motor é válido
        return; // Retorna sem fazer nada se o ponteiro for inválido
    }

    long posicaoInicial = motor->currentPosition();
    long posicaoFinal;

    motor->setAcceleration(1000); // Define a aceleração em passos por segundo ao quadrado
    motor->setMaxSpeed(abs(velocidadeMaxima)); // Define a velocidade máxima em passos por segundo
    
    if (direcao == 1) {
        motor->setSpeed(abs(velocidadeMaxima));
        posicaoFinal = posicaoInicial + distancia;
    } else if (direcao == -1) {
        motor->setSpeed(-abs(velocidadeMaxima));
        posicaoFinal = posicaoInicial - distancia;
    } else {
        return; // Direção inválida, sai da função
    }

    motor->moveTo(posicaoFinal);

    while (motor->currentPosition() != posicaoFinal) {
        motor->run();
    }
}



// Função para mover o motor de forma acelerada
void moverUniforme(AccelStepper* motor, long distancia, int velocidade, int direcao) {
    if (!motor) return; // Verifica se o ponteiro do motor é válido

    motor->setMaxSpeed(abs(velocidade)); // Define a velocidade máxima sem aceleração

    // Define a direção do motor
    if (direcao == 1) {
        motor->setSpeed(abs(velocidade));  // Sentido horário
    } else if (direcao == -1) {
        motor->setSpeed(-abs(velocidade)); // Sentido anti-horário
    } else {
        return; // Direção inválida, sai da função
    }

    long posicaoInicial = motor->currentPosition(); // Guarda a posição inicial
    long posicaoFinal = posicaoInicial + (direcao == 1 ? distancia : -distancia);

    pararMotor = false;

    // Move o motor enquanto não atingir a posição final
    while (motor->currentPosition() != posicaoFinal) {
        if (pararMotor) break; // Para o motor se necessário
        motor->runSpeed(); // Mantém a velocidade fixa
    }

    // Parada abrupta: Zera a velocidade e a posição imediatamente
    motor->setSpeed(0);
    motor->setCurrentPosition(0);
    motor->disableOutputs(); // Desativa as saídas para cortar a energia
    
}


void moverSimultaneo(AccelStepper* motor1, AccelStepper* motor2, int distancia1, int distancia2, int velocidadeMaxima1, int velocidadeMaxima2, String direcao) {
    // Verifica se os motores são válidos
    if ((!motor1) || (!motor2)) {
        return;
    }

    long posicaoInicial1 = motor1->currentPosition();
    long posicaoInicial2 = motor2->currentPosition();
    long posicaoDesejada1, posicaoDesejada2;

    pararMotorSimultaneo = false;

    motor1->setMaxSpeed(abs(velocidadeMaxima1));  // Define a velocidade máxima
    motor2->setMaxSpeed(abs(velocidadeMaxima2));  // Define a velocidade máxima

    motor1->enableOutputs();  // Ativa os motores
    motor2->enableOutputs();  // Ativa os motores

    if (direcao.equals("C")) {  // Se direção for "C"
        posicaoDesejada1 = posicaoInicial1 - distancia1;  // Motor 1 avança
        posicaoDesejada2 = posicaoInicial2 - distancia2;  // Motor 2 recua
    } else if (direcao.equals("B")) {  // Se direção for "B"
        posicaoDesejada1 = posicaoInicial1 + distancia1;  // Motor 1 recua
        posicaoDesejada2 = posicaoInicial2 + distancia2;  // Motor 2 avança
    } else {
        return; // Se direção inválida, sai da função
    }

    motor1->setAcceleration(5000);  // Define aceleração
    motor2->setAcceleration(5000);  // Define aceleração

    motor1->moveTo(posicaoDesejada1);  // Move motor 1 para a posição desejada
    motor2->moveTo(posicaoDesejada2);  // Move motor 2 para a posição desejada

    Serial.println("Movendo motores...");

    // Loop para mover os motores até que ambos atinjam suas posições desejadas
    while ((motor1->distanceToGo() != 0 || motor2->distanceToGo() != 0)) {
        if (pararMotorSimultaneo) {
            Serial.println("Parando motores...");
            break;
        }

        // Verifica se há comandos de parada
        if (Serial.available()) {
            char comando = Serial.read();
            if (comando == 'n') {
                pararMotorSimultaneo = true;
                Serial.println("Comando de parada recebido!");
                break;
            }
        }

        motor1->run();  // Executa o movimento do motor 1
        motor2->run();  // Executa o movimento do motor 2
    }

    // Para os motores ao terminar o movimento
    motor1->stop();
    motor2->stop();

    // Zera as acelerações e as velocidades
    motor1->setAcceleration(0);
    motor2->setAcceleration(0);
    motor1->setSpeed(0);
    motor2->setSpeed(0);

    // Reseta a posição dos motores
    motor1->setCurrentPosition(0);
    motor2->setCurrentPosition(0);

    // Desativa os motores após o movimento
    motor1->disableOutputs();
    motor2->disableOutputs();

    Serial.println("y");
}

void calibracao(){}

void paraMotorSimultaneo(AccelStepper* motor1, AccelStepper* motor2) {
    if ((!motor1) || (!motor2)) return;

    pararMotorSimultaneo = true;

    // Para o motor instantaneamente definindo velocidade zero
    motor1->setSpeed(0);
    motor2->setSpeed(0);

    motor1->setCurrentPosition(0);
    motor2->setCurrentPosition(0);

    motor1->disableOutputs(); 
    motor2->disableOutputs();


}

void paraMotor(AccelStepper* motor){
    if (!motor) return;

    pararMotor = true;

    motor->setSpeed(0); // Para o motor imediatamente
    motor->setCurrentPosition(0); // Redefine a posição atual do motor para
    motor->disableOutputs(); // Desabilita as saídas do motor (desliga a energia)
    
}

void subsidencia(AccelStepper* motor, int velocidadeMaxima, int aceleracao, long distancia){
    digitalWrite(PIN_ENABLE_1, HIGH);
    motor->setMaxSpeed(velocidadeMaxima);
    motor->setAcceleration(aceleracao);
    motor->move(distancia);
    
    while(motor->distanceToGo() != 0){
        motor->run();
    }

    delay(50);

    motor->move(-distancia);
    while(motor->distanceToGo() != 0){
        motor->run();
    }

    delay(50);
    digitalWrite(PIN_ENABLE_1, LOW);
}

void habilitarMotor(AccelStepper* motor, int enablePin){
    if(motor){
        digitalWrite(enablePin, HIGH); // Habilita o motor
    }
}

// Função para desabilitar um motor
void desabilitarMotor(AccelStepper* motor, int enablePin) {
    if (motor) {
        digitalWrite(enablePin, LOW); // Desabilita o motor
    }
}

