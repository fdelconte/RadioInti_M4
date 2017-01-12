#include "stm32f4xx.h"                  // Device header
#include "RTE_Components.h"             // Component selection


//===========================DEFINES=========================//
// Codec AK4621EF reg
#define AK4621EF_PDC	0
#define AK4621EF_RC		1
#define AK4621EF_CFC	2
#define AK4621EF_DVC	3
#define AK4621EF_R0		4
#define AK4621EF_R1		5
#define AK4621EF_LDC	6
#define AK4621EF_RDC	7
#define AK4621EF_LEDC	8
#define AK4621EF_REDC 9

//===========================VARIABLES=========================//
extern TIM_HandleTypeDef htim6;


//===========================Prototipos=========================//

void Error_Handler ( void );

void led_secuencia ( void );

void codec_send_data ( uint8_t address, uint8_t data );

void dma_tx_rx ( void );

void init_AK4621EF ( void );
