#ifndef MACROS_H
#define MACROS_H

/**
 * @file macros.h
 * @brief Definições de hardware, mapeamento de pinos e protocolo de comunicação.
 * * Este arquivo centraliza todas as constantes do projeto, permitindo a fácil transição
 * entre diferentes setups de hardware e a manutenção do protocolo serial.
 */

/*
 * -------- CONFIGURAÇÃO DE REFERÊNCIA: CAIXA AZUL ----------
 * Mapeamento original para sistemas de 2 motores.
 * MOTOR 1: ENABLE 3 | PASSO 4 | DIR 2
 * MOTOR 2: ENABLE 10 | PUL 9  | DIR 8
 */

/*
 * -------- CONFIGURAÇÃO DE REFERÊNCIA: CAIXA PRETA (4 MOTORES) ----------
 * Mapeamento utilizado para sistemas expandidos de 4 motores.
 * MOTOR 1: PUL 3 | DIR 4 | ENABLE 2
 * MOTOR 2: PUL 6 | DIR 7 | ENABLE 5
 * MOTOR 3: PUL 9 | DIR 10 | ENABLE 8
 * MOTOR 4: PUL 12 | DIR 13 | ENABLE 11
 */

// --- Códigos de Status e Erros de Execução ---
#define OK 0                            // Operação executada com sucesso
#define ERROR_MOTOR_NAO_SELECIONADO 1   // Falha ao tentar mover sem motor definido

// --- Mapeamento de Pinos: Controle dos Motores de Passo ---
// Os pinos abaixo seguem a lógica: ENABLE (Ativação), PASSO/PUL (Pulsos), DIR (Direção)

// Controle do Motor 1 (Eixo Vertical)
#define PIN_ENABLE_1 3   // Pino de ativação do driver do Motor 1
#define PIN_PASSO_1 4    // Pino de sinal de passo (STEP) para o Motor 1
#define PIN_DIR_1 2      // Pino de definição de direção para o Motor 1

// Controle do Motor 2 (Eixo Horizontal)
#define PIN_ENABLE_2 6   // Pino de ativação do driver do Motor 2
#define PIN_PASSO_2 7    // Pino de sinal de passo (STEP) para o Motor 2
#define PIN_DIR_2 5      // Pino de definição de direção para o Motor 2

// Controle do Motor 3
#define PIN_ENABLE_3 9   // Pino de ativação do driver do Motor 3
#define PIN_PASSO_3 10   // Pino de sinal de passo (STEP) para o Motor 3
#define PIN_DIR_3 8      // Pino de definição de direção para o Motor 3

// Controle do Motor 4
#define PIN_ENABLE_4 12  // Pino de ativação do driver do Motor 4
#define PIN_PASSO_4 13   // Pino de sinal de passo (STEP) para o Motor 4
#define PIN_DIR_4 11     // Pino de definição de direção para o Motor 4

// --- Sensores de Segurança e Fim de Curso ---
#define SENSOR_INDUTIVO_MOTOR_1 A0 // Sensor para monitoramento do Motor 1
#define SENSOR_INDUTIVO_MOTOR_2 A1 // Sensor para monitoramento do Motor 2

// --- Protocolo Serial: Comandos de Operação (Single Character) ---
#define LIGAR_MOTOR 'A'             // Habilita a saída de energia para o motor
#define DESLIGAR_MOTOR 'a'          // Desabilita a saída (motor livre)
#define PARAR_MOTOR 'n'             // Interrupção imediata do movimento
#define MOVER_MOTORES_SIMULTANEOS 'W' // Comando complexo para Motores 1 e 2
#define MOVER_MOTORES_FALHA 'L'      // Comando complexo para Motores 3 e 4
#define MOVER_MOTOR_UNIVERSAL 'Z'    // Movimentação coordenada dos 4 motores

// --- Protocolo Serial: Seleção de Alvo (Máquina de Estados) ---
#define ALTERAR_PARA_MOTOR_1 'R'             // Seleciona o Motor 1 como foco
#define ALTERAR_PARA_MOTOR_2 'M'             // Seleciona o Motor 2 como foco
#define ALTERAR_PARA_MOTORES_SIMULTANEOS 'm' // Foco no par 1 e 2
#define ALTERAR_PARA_MOTORES_FALHA 'l'       // Foco no par 3 e 4

// --- Identificadores de Contexto de Motor ---
#define MOTOR_1 1
#define MOTOR_2 2
#define MOTORES_SIMULTANEOS 3
#define MOTORES_FALHA 4

// --- Parâmetros de Movimento e Direção ---
#define QTD_PULSOS 'P'         // Prefixo para definir distância de deslocamento
#define MOVER_MOTOR_BAIXO 1    // Sentido de rotação para baixo
#define MOVER_MOTOR_CIMA 0     // Sentido de rotação para cima
#define DEFINIR_VELOCIDADE 'V' // Prefixo para ajuste de velocidade

// --- Rotinas Especiais e Calibração ---
#define INICIAR_CALIBRACAO 'I'        // Inicia sequência de busca de zero
#define PARAR_CALIBRACAO 'N'          // Cancela rotina de calibração
#define ACELERAR_MOTOR 'G'            // Movimento com rampa de aceleração
#define ATIVAR_MOTOR_VEL_CTE 'H'      // Movimento uniforme
#define CONFIGURAR_ZERO_LASER 'J'     // Define referência zero via laser
#define SUBSIDENCIA 'K'               // Inicia ciclo de teste de subsidência
#define INSERIR_CONSTANTES_CALIBRACAO 'U' // Envio de fatores de correção

// --- Monitoramento de Sensores ---
#define ATIVAR_SENSOR_INDUTIVO 'S'    // Habilita parada por proximidade
#define DESATIVAR_SENSOR_INDUTIVO 's' // Ignora leituras dos sensores

// --- Mensagens de Status e Configuração ---
#define ENVIAR_CONFIG_COMPLETA 'T'    // Recebe pacote de dados (P;V;D;M)
#define MSG_MOTOR_MOVENDO_COM_ACELERACAO 'X' // Flag para modo acelerado
#define MSG_MOTOR_MOVENDO_SEM_ACELERACAO 'x' // Flag para modo uniforme

// --- Strings de Log/Depuração ---
#define DIRECAO_MOTOR_1_BAIXO "Mover motor 1 para baixo"
#define DIRECAO_MOTOR_2_BAIXO "Mover motor 2 para baixo"

#endif