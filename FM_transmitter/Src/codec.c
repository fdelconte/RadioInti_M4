// Header:
// File Name: 
// Author:
// Date:


//===========================INCLUDES=========================//

#include "codec.h"

//===========================VARIABLES=========================//

// Para el init del codec
volatile uint8_t bit_counter = 0;



//===========================FUNCIONES=========================//

// Para indicar que todo anda bien
void led_secuencia (void)
{
	static uint32_t led_count = 0;

	if (led_count == 300000)
	{
		HAL_GPIO_WritePin(GPIOD, LD4_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, LD6_Pin, GPIO_PIN_RESET);
		led_count++;
	}
	else if (led_count == 600000)
	{
		HAL_GPIO_WritePin(GPIOD, LD3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, LD4_Pin, GPIO_PIN_RESET);
		led_count++;
	}
	else if (led_count == 900000)
	{
		HAL_GPIO_WritePin(GPIOD, LD5_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, LD3_Pin, GPIO_PIN_RESET);			
		led_count++;
	}
	else if (led_count == 1200000)
	{
		HAL_GPIO_WritePin(GPIOD, LD6_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, LD5_Pin, GPIO_PIN_RESET);			
		led_count = 0;
	}
	else
		led_count++;
}
//


// Comunicacion SPI half-duplex para configurar al codec AK4621EF
void AK4621EF_send_data ( uint8_t address, uint8_t data )
{
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);					// Bajo la linea de CSN
	HAL_GPIO_WritePin(CCLK_GPIO_Port, CCLK_Pin, GPIO_PIN_RESET);				// Bajo la linea de CCLK
	
	HAL_TIM_Base_Start_IT(&htim6);																			// Start Timer
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, (GPIO_PinState) CHIP_ADDRESS_0);							// BIT_0
	while(bit_counter != 1) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, (GPIO_PinState) CHIP_ADDRESS_1);							// BIT_1
	while(bit_counter != 2) {}

	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, (GPIO_PinState) READ_WRITE);									// BIT_2
	while(bit_counter != 3) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, (GPIO_PinState) ( (address >> 4) & 0x01) );			// BIT_3
	while(bit_counter != 4) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, (GPIO_PinState) ( (address >> 3) & 0x1) );			// BIT_4
	while(bit_counter != 5) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, (GPIO_PinState) ( (address >> 2) & 0x1) );			// BIT_5
	while(bit_counter != 6) {}
		
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, (GPIO_PinState) ( (address >> 1) & 0x1) );			// BIT_6
	while(bit_counter != 7) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, (GPIO_PinState) ( (address >> 0) & 0x1) );			// BIT_7
	while(bit_counter != 8) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, (GPIO_PinState) ( (data >> 7) & 0x1) );				// BIT_8
	while(bit_counter != 9) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, (GPIO_PinState) ( (data >> 6) & 0x1) );				// BIT_9
	while(bit_counter != 10) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, (GPIO_PinState) ( (data >> 5) & 0x1) );				// BIT_10
	while(bit_counter != 11) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, (GPIO_PinState) ( (data >> 4) & 0x1) );				// BIT_11
	while(bit_counter != 12) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, (GPIO_PinState) ( (data >> 3) & 0x1) );				// BIT_12
	while(bit_counter != 13) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, (GPIO_PinState) ( (data >> 2) & 0x1) );				// BIT_13
	while(bit_counter != 14) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, (GPIO_PinState) ( (data >> 1) & 0x1) );				// BIT_14
	while(bit_counter != 15) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, (GPIO_PinState) ( (data >> 7) & 0x1) );				// BIT_15
	while(bit_counter != 16) {}

	HAL_TIM_Base_Stop_IT(&htim6);																			// Stop Timer

	bit_counter = 0;
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, GPIO_PIN_SET);				// Subo la linea de CDTI
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);					// Subo la linea de CSN	
	HAL_GPIO_WritePin(CCLK_GPIO_Port, CCLK_Pin, GPIO_PIN_SET);				// Subo la linea de CCLK
}
//


// Inicializacion del codec AK4621EF
void AK4621EF_init ( void )
{
	AK4621EF_send_data( AK4621EF_PDC , 0x07 );
	AK4621EF_send_data( AK4621EF_RC , 0x03 );
	AK4621EF_send_data( AK4621EF_CFC , 0x60 );
	AK4621EF_send_data( AK4621EF_DVC , 0x62 );
	AK4621EF_send_data( AK4621EF_R0 , 0x00 );
	AK4621EF_send_data( AK4621EF_R1 , 0x00 );
	AK4621EF_send_data( AK4621EF_LDC , 0xFF );
	AK4621EF_send_data( AK4621EF_RDC , 0xFF );
	AK4621EF_send_data( AK4621EF_LEDC , 0x0F );
	AK4621EF_send_data( AK4621EF_REDC , 0x0F );
}
//








// Manejo de buffers en la comunicación I2S
void dma_tx_rx ( void )
{
}

void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
}

void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
	if(transmission_ready == 0)
	{
		buffer_tx_aux = buffer_tx_A;
	}
	else
	{
		buffer_tx_aux = &(buffer_tx_A[4]);
	}
	transmission_ready = !transmission_ready;
	
	buffer_tx_aux[0] -= 2;
	buffer_tx_aux[1] -= 2;
	buffer_tx_aux[2] -= 2;
	buffer_tx_aux[3] -= 2;
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
}

void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
	if(transmission_ready == 0)
	{
		buffer_tx_aux = buffer_tx_A;
	}
	else
	{
		buffer_tx_aux = &(buffer_tx_A[4]);
	}
	transmission_ready = !transmission_ready;
	
	buffer_tx_aux[0] -= 2;
	buffer_tx_aux[1] -= 2;
	buffer_tx_aux[2] -= 2;
	buffer_tx_aux[3] -= 2;
}
