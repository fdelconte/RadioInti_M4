// Header:
// File Name: 
// Author:
// Date:


//===========================INCLUDES=========================//

#include "codec.h"

//===========================VARIABLES=========================//

// Codec init counter
volatile uint8_t bit_counter = 0;

// Enable DSP
volatile uint8_t transmission_ready = HAL_BUSY;

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
	
}
//


// Inicializacion del codec AK4621EF
void AK4621EF_init ( void )
{
	uint16_t trama = 0;

	HAL_GPIO_WritePin(PDN_GPIO_Port, PDN_Pin, GPIO_PIN_SET);
	
	trama = 0xA007;
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);					// Bajo la linea de CSN
	if( HAL_SPI_Transmit(&hspi1, (uint8_t *) &trama, 1, 50) != HAL_OK )										// Transmito
			Error_Handler();
	while(hspi1.State != HAL_SPI_STATE_READY ) {};
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);						// Subo la linea de CSN	

	trama = 0xA103;
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);					// Bajo la linea de CSN
	if( HAL_SPI_Transmit(&hspi1, (uint8_t *) &trama, 1, 50) != HAL_OK )										// Transmito
			Error_Handler();
	while(hspi1.State != HAL_SPI_STATE_READY ) {};
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);						// Subo la linea de CSN	

	trama = 0xA260;
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);					// Bajo la linea de CSN
	if( HAL_SPI_Transmit(&hspi1, (uint8_t *) &trama, 1, 50) != HAL_OK )										// Transmito
			Error_Handler();
	while(hspi1.State != HAL_SPI_STATE_READY ) {};
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);						// Subo la linea de CSN	

	trama = 0xA362;
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);					// Bajo la linea de CSN
	if( HAL_SPI_Transmit(&hspi1, (uint8_t *) &trama, 1, 50) != HAL_OK )										// Transmito
			Error_Handler();
	while(hspi1.State != HAL_SPI_STATE_READY ) {};
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);						// Subo la linea de CSN	
		
	trama = 0xA400;
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);					// Bajo la linea de CSN
	if( HAL_SPI_Transmit(&hspi1, (uint8_t *) &trama, 1, 50) != HAL_OK )										// Transmito
			Error_Handler();
	while(hspi1.State != HAL_SPI_STATE_READY ) {};
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);						// Subo la linea de CSN	

	trama = 0xA500;
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);					// Bajo la linea de CSN
	if( HAL_SPI_Transmit(&hspi1, (uint8_t *) &trama, 1, 50) != HAL_OK )										// Transmito
			Error_Handler();
	while(hspi1.State != HAL_SPI_STATE_READY ) {};
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);						// Subo la linea de CSN	
		
	trama = 0xA6FF;
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);					// Bajo la linea de CSN
	if( HAL_SPI_Transmit(&hspi1, (uint8_t *) &trama, 1, 50) != HAL_OK )										// Transmito
			Error_Handler();
	while(hspi1.State != HAL_SPI_STATE_READY ) {};
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);						// Subo la linea de CSN	
		
	trama = 0xA7FF;
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);					// Bajo la linea de CSN
	if( HAL_SPI_Transmit(&hspi1, (uint8_t *) &trama, 1, 50) != HAL_OK )										// Transmito
			Error_Handler();
	while(hspi1.State != HAL_SPI_STATE_READY ) {};
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);						// Subo la linea de CSN	

	trama = 0xA80F;
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);					// Bajo la linea de CSN
	if( HAL_SPI_Transmit(&hspi1, (uint8_t *) &trama, 1, 50) != HAL_OK )										// Transmito
			Error_Handler();
	while(hspi1.State != HAL_SPI_STATE_READY ) {};
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);						// Subo la linea de CSN	

	trama = 0xA90F;
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);					// Bajo la linea de CSN
	if( HAL_SPI_Transmit(&hspi1, (uint8_t *) &trama, 1, 50) != HAL_OK )										// Transmito
			Error_Handler();
	while(hspi1.State != HAL_SPI_STATE_READY ) {};
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);						// Subo la linea de CSN	
}
//


// Manejo de buffers en la comunicación I2S
void dma_tx_rx ( void )
{
	if ( transmission_ready == HAL_OK )
	{
		transmission_ready = HAL_BUSY;
		
		if( toggle_buffer == BUFFER_A )
		{
			buffer_tx_aux[0] = buffer_rx_aux[0] + 1;
			buffer_tx_aux[1] = buffer_rx_aux[1] + 1;
			buffer_tx_aux[2] = buffer_rx_aux[2] + 1;
			buffer_tx_aux[3] = buffer_rx_aux[3] + 1;
		}
		else
		{
			buffer_tx_aux[0] = buffer_rx_aux[0] + 1;
			buffer_tx_aux[1] = buffer_rx_aux[1] + 1;
			buffer_tx_aux[2] = buffer_rx_aux[2] + 1;
			buffer_tx_aux[3] = buffer_rx_aux[3] + 1;
		}

		toggle_buffer = !toggle_buffer;
		
		HAL_GPIO_WritePin(TEST_PIN_GPIO_Port, TEST_PIN_Pin, GPIO_PIN_RESET);
	}
}
//


// Half complete callback TX
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
}
//


// Half complete callback RX
void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
	HAL_GPIO_WritePin(TEST_PIN_GPIO_Port, TEST_PIN_Pin, GPIO_PIN_SET);

	if(toggle_buffer == BUFFER_A)
	{
		buffer_tx_aux = buffer_tx;
		buffer_rx_aux = buffer_rx;
	}
	else
	{
		buffer_tx_aux = &(buffer_tx[BUFFER_LENGTH/2]);
		buffer_rx_aux = &(buffer_rx[BUFFER_LENGTH/2]);
	}
		
	transmission_ready = HAL_OK;
}
//


// Complete callback TX
void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
}
//


// Complete callback RX
void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
	HAL_GPIO_WritePin(TEST_PIN_GPIO_Port, TEST_PIN_Pin, GPIO_PIN_SET);

	if(toggle_buffer == BUFFER_A)
	{
		buffer_tx_aux = buffer_tx;
	}
	else
	{
		buffer_tx_aux = &(buffer_tx[4]);
	}
	
	transmission_ready = HAL_OK;
}
//



