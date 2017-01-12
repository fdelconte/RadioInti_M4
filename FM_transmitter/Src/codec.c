// Header:
// File Name: 
// Author:
// Date:


#include "codec.h"


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


#define CHIP_ADDRESS_0	1
#define CHIP_ADDRESS_1	0
#define READ_WRITE			1

volatile uint8_t bit_counter = 0;

void codec_send_data ( uint8_t address, uint8_t data )
{
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);					// Bajo la linea de CSN
	HAL_GPIO_WritePin(CCLK_GPIO_Port, CCLK_Pin, GPIO_PIN_RESET);				// Bajo la linea de CCLK
	
	HAL_TIM_Base_Start_IT(&htim6);																			// Start Timer
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, CHIP_ADDRESS_0);							// BIT_0
	while(bit_counter != 1) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, CHIP_ADDRESS_1);							// BIT_1
	while(bit_counter != 2) {}

	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, READ_WRITE);									// BIT_2
	while(bit_counter != 3) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, ( (address >> 4) & 0x01) );			// BIT_3
	while(bit_counter != 4) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, ( (address >> 3) & 0x1) );			// BIT_4
	while(bit_counter != 5) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, ( (address >> 2) & 0x1) );			// BIT_5
	while(bit_counter != 6) {}
		
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, ( (address >> 1) & 0x1) );			// BIT_6
	while(bit_counter != 7) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, ( (address >> 0) & 0x1) );			// BIT_7
	while(bit_counter != 8) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, ( (data >> 7) & 0x1) );				// BIT_8
	while(bit_counter != 9) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, ( (data >> 6) & 0x1) );				// BIT_9
	while(bit_counter != 10) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, ( (data >> 5) & 0x1) );				// BIT_10
	while(bit_counter != 11) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, ( (data >> 4) & 0x1) );				// BIT_11
	while(bit_counter != 12) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, ( (data >> 3) & 0x1) );				// BIT_12
	while(bit_counter != 13) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, ( (data >> 2) & 0x1) );				// BIT_13
	while(bit_counter != 14) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, ( (data >> 1) & 0x1) );				// BIT_14
	while(bit_counter != 15) {}
	
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, ( (data >> 7) & 0x1) );				// BIT_15
	while(bit_counter != 16) {}

	HAL_TIM_Base_Stop_IT(&htim6);																			// Stop Timer

	bit_counter = 0;
	HAL_GPIO_WritePin(CDTI_GPIO_Port, CDTI_Pin, GPIO_PIN_SET);				// Subo la linea de CDTI
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);					// Subo la linea de CSN	
	HAL_GPIO_WritePin(CCLK_GPIO_Port, CCLK_Pin, GPIO_PIN_SET);				// Subo la linea de CCLK
}


extern DMA_HandleTypeDef hdma_spi3_tx;
extern DMA_HandleTypeDef hdma_i2s3_ext_rx;
extern I2S_HandleTypeDef hi2s3;
extern TIM_HandleTypeDef htim6;

extern uint16_t buffer_tx_A[4];
extern uint16_t buffer_tx_B[4];
	
extern uint16_t buffer_rx_A[8];
extern uint16_t buffer_rx_B[8];


void dma_tx_rx ( void )
{
	
	static uint8_t i = 0;
	static uint8_t j = 0;

	// Se termina TX por DMA
	if( hdma_spi3_tx.State == HAL_DMA_STATE_READY )
	{
		if( i == 0 )
		{
			if( HAL_I2SEx_TransmitReceive_DMA(&hi2s3, buffer_tx_A, buffer_rx_A, 4) != HAL_OK )
				Error_Handler();
			i = 1;
		}
		else if ( i == 1 )
		{
			if( HAL_I2SEx_TransmitReceive_DMA(&hi2s3, buffer_tx_B, buffer_rx_B, 4) != HAL_OK )
				Error_Handler();
			i = 0;
		}
	}
	
	// Se termina RX por DMA
	if( hdma_i2s3_ext_rx.State == HAL_DMA_STATE_READY )
	{
		if( j == 0 )
		{
			buffer_rx_A[0] = 0x0000;	buffer_rx_A[1] = 0x0000;
			buffer_rx_A[2] = 0x0000;	buffer_rx_A[3] = 0x0000;	
			if( HAL_I2SEx_TransmitReceive_DMA(&hi2s3, buffer_tx_A, buffer_rx_A, 4) != HAL_OK )
				Error_Handler();
			j = 1;
		}
		else if ( j == 1 )
		{
			buffer_rx_B[0] = 0x0000;	buffer_rx_B[1] = 0x0000;
			buffer_rx_B[2] = 0x0000;	buffer_rx_B[3] = 0x0000;	
			if( HAL_I2SEx_TransmitReceive_DMA(&hi2s3, buffer_tx_B, buffer_rx_B, 4) != HAL_OK )
				Error_Handler();
			j = 0;
		}
	}
}




void init_AK4621EF ( void )
{
	codec_send_data( AK4621EF_PDC , 0x07 );
	codec_send_data( AK4621EF_RC , 0x03 );
	codec_send_data( AK4621EF_CFC , 0x60 );
	codec_send_data( AK4621EF_DVC , 0x62 );
	codec_send_data( AK4621EF_R0 , 0x00 );
	codec_send_data( AK4621EF_R1 , 0x00 );
	codec_send_data( AK4621EF_LDC , 0xFF );
	codec_send_data( AK4621EF_RDC , 0xFF );
	codec_send_data( AK4621EF_LEDC , 0x0F );
	codec_send_data( AK4621EF_REDC , 0x0F );
}


void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
	
	HAL_I2SEx_TransmitReceive_DMA(&hi2s3, (uint16_t *)buffer_tx_A, (uint16_t *)buffer_rx_A, 4);
//	static uint32_t i=0;
//	
//	buffer_tx_A[0] = i+=0xFFFF;
}

void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{	
	
	HAL_I2SEx_TransmitReceive_DMA(&hi2s3, (uint16_t *)buffer_tx_A, (uint16_t *)buffer_rx_A, 4);
//	if( HAL_I2SEx_TransmitReceive_DMA(&hi2s3, (uint16_t *)buffer_tx_A, (uint16_t *)buffer_rx_A, 4) != HAL_OK )
//	{
//			Error_Handler();
//	}
}

