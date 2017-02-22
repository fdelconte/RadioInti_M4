// Header:
// File Name: 
// Author:
// Date:


//===========================INCLUDES=========================//

#include "stm32f4xx.h"                  // Device header
#include "RTE_Components.h"             // Component selection
#include "stm32f4xx_hal.h"
#define ARM_MATH_CM4
#include "arm_math.h"

//===========================CONFIG=========================//

#define LOOPBACK 				0
#define	MPX							!LOOPBACK

//===========================DEFINES=========================//

// Codec AK4621EF reg
#define AK4621EF_PDC		0
#define AK4621EF_RC			1
#define AK4621EF_CFC		2
#define AK4621EF_DVC		3
#define AK4621EF_R0			4
#define AK4621EF_R1			5
#define AK4621EF_LDC		6
#define AK4621EF_RDC		7
#define AK4621EF_LEDC		8
#define AK4621EF_REDC 	9

// Codec AK4621EF command
#define CHIP_ADDRESS_0	1
#define CHIP_ADDRESS_1	0
#define READ_WRITE			1

// Double buffer
#define	BUFFER_A				0
#define	BUFFER_B				1	

// Init transsmision
#define BUFFER_LENGTH 	4

// Separacion de canales
#define CH_L_1					1
#define CH_L_2					2
#define CH_R_1					3
#define CH_R_2					4

//===========================VARIABLES=========================//

// Handlers de los perifericos utilizados
extern TIM_HandleTypeDef htim6;

extern DMA_HandleTypeDef hdma_spi3_tx;
extern DMA_HandleTypeDef hdma_i2s3_ext_rx;

extern I2S_HandleTypeDef hi2s3;

extern SPI_HandleTypeDef hspi1;

// Buffers de comunicacion I2S
extern uint32_t buffer_tx[BUFFER_LENGTH*2];
extern uint32_t buffer_rx[BUFFER_LENGTH*2];

// Buffers auxiliares para realizar dsp
extern uint32_t *buffer_tx_aux;
extern uint32_t *buffer_rx_aux;

// Manejo de los callback
extern uint8_t toggle_buffer;

// Canales
extern q31_t canal_L[BUFFER_LENGTH];
extern q31_t canal_R[BUFFER_LENGTH];
extern q31_t suma[BUFFER_LENGTH/4];
extern q31_t resta[BUFFER_LENGTH/4];

// Armado de los canales
extern volatile uint8_t CANAL;

//===========================Prototipos=========================//

// Manejador de errores, prende todos los leds
void Error_Handler ( void );

// Indica que todo esta OK
void led_secuencia ( void );

// AK4621EF inicializacion
void AK4621EF_init ( void );

// AK4621EF comunicacion SPI half-duplex
void AK4621EF_send_data ( uint8_t address, uint8_t data );

// Controla la comunicacion full duplex con el codec
void dma_tx_rx ( void );


