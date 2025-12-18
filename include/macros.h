#ifndef MACROS_H
#define MACROS_H

/*
--------CAIXA AZUL----------

**MOTOR 1**
ENABLE 3
PASSO 4
DIR 2


**MOTOR 2**
ENABLE 10
PUL 9
DIR 8
*/


/*
--------CAIXA PRETA (4 MOTORES)----------

**MOTOR 1**
PUL 3
DIR 4
ENABLE 2

**MOTOR 2**
PUL 6
DIR 7
ENABLE 5

**MOTOR 3**
PUL 9
DIR 10
ENABLE 8

**MOTOR 4**
PUL 12
DIR 13
ENABLE 11

*/

// Define Erros de Exeecução
#define OK 0 // Executado com sucesso
#define ERROR_MOTOR_NAO_SELECIONADO 1

// Defina os pinos de controle do primeiro motor
// Motor 1 = Vertical
#define PIN_ENABLE_1 3   // Pino de ENABLE 11 do Motor 1 2
#define PIN_PASSO_1 4  // Pino de passo (STEP) 8 do Motor 1 4
#define PIN_DIR_1 2    // Pino de direção (DIR) 9 do Motor 1 3

// Defina os pinos de controle do segundo motor
// Motor 2 = Horizontal
#define PIN_ENABLE_2 6 // Pino de ENABLE 13 do Motor 2 4 4.54 0
#define PIN_PASSO_2 7  // Pino de passo (STEP) 11 do Motor 2 0 4.53  4.22
#define PIN_DIR_2 5    // Pino de direção (DIR) 12 do Motor 2 0 4.54 4.36

// Defina os pinos de controle do segundo motor
// Motor 3
#define PIN_ENABLE_3 9 // Pino de ENABLE 4 do Motor 2 4 4.54 0 8
#define PIN_PASSO_3 10 // Pino de passo (STEP)2 do Motor 2 0 4.53  4.22 10
#define PIN_DIR_3 8   // Pino de direção (DIR) 3 do Motor 2 0 4.54 4.36 9

// Defina os pinos de controle do segundo motor
// Motor 4
#define PIN_ENABLE_4 12 // Pino de ENABLE 7 do Motor 2 4 4.54 0
#define PIN_PASSO_4 13 // Pino de passo (STEP) 5 do Motor 2 0 4.53  4.22
#define PIN_DIR_4 11    // Pino de direção (DIR) 6 do Motor 2 0 4.54 4.36

// Defina os pinos dos sensores indutivos
#define SENSOR_INDUTIVO_MOTOR_1 A0
#define SENSOR_INDUTIVO_MOTOR_2 A1

#define LIGAR_MOTOR 'A'
#define DESLIGAR_MOTOR 'a'
#define PARAR_MOTOR 'n'
#define MOVER_MOTORES_SIMULTANEOS 'W'
#define MOVER_MOTORES_FALHA 'L'
#define MOVER_MOTOR_UNIVERSAL 'Z'
#define ALTERAR_PARA_MOTOR_1 'R'
#define ALTERAR_PARA_MOTOR_2 'M'
#define ALTERAR_PARA_MOTORES_SIMULTANEOS 'm'
#define ALTERAR_PARA_MOTORES_FALHA 'l'

#define MOTOR_1 1
#define MOTOR_2 2
#define MOTORES_SIMULTANEOS 3
#define MOTORES_FALHA 4

#define QTD_PULSOS 'P'

#define MOVER_MOTOR_BAIXO 1
#define MOVER_MOTOR_CIMA 0

#define INICIAR_CALIBRACAO 'I'
#define ACELERAR_MOTOR 'G'
#define ATIVAR_MOTOR_VEL_CTE 'H'
#define CONFIGURAR_ZERO_LASER 'J'
#define SUBSIDENCIA 'K'
#define PARAR_CALIBRACAO 'N'
#define POSICAO_MOTOR_1 'O'
#define POSICAO_MOTOR_2 'o'
#define ATIVAR_SENSOR_INDUTIVO 'S'
#define DESATIVAR_SENSOR_INDUTIVO 's'
#define ENVIAR_CONFIG_COMPLETA 'T'
#define INSERIR_CONSTANTES_CALIBRACAO 'U'
#define DEFINIR_VELOCIDADE 'V'
#define MSG_MOTOR_MOVENDO_COM_ACELERACAO 'X'
#define MSG_MOTOR_MOVENDO_SEM_ACELERACAO 'x'
#define DIRECAO_MOTOR_1_BAIXO "Mover motor 1 para baixo"
#define DIRECAO_MOTOR_2_BAIXO "Mover motor 2 para baixo"

#endif