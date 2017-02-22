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

	if (led_count == 30000)
	{
		HAL_GPIO_WritePin(GPIOD, LD4_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, LD6_Pin, GPIO_PIN_RESET);
		led_count++;
	}
	else if (led_count == 60000)
	{
		HAL_GPIO_WritePin(GPIOD, LD3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, LD4_Pin, GPIO_PIN_RESET);
		led_count++;
	}
	else if (led_count == 90000)
	{
		HAL_GPIO_WritePin(GPIOD, LD5_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, LD3_Pin, GPIO_PIN_RESET);			
		led_count++;
	}
	else if (led_count == 120000)
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
	uint32_t demora = 0;
	uint16_t trama = 0;

	HAL_GPIO_WritePin(PDN_GPIO_Port, PDN_Pin, GPIO_PIN_SET);
	
	trama = 0xA007;
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);					// Bajo la linea de CSN
	if( HAL_SPI_Transmit(&hspi1, (uint8_t *) &trama, 1, 50) != HAL_OK )										// Transmito
			Error_Handler();
	while(hspi1.State != HAL_SPI_STATE_READY ) {};
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);						// Subo la linea de CSN	
	
	for(demora = 0; demora < 1000; demora++){};
		
	trama = 0xA103;
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);					// Bajo la linea de CSN
	if( HAL_SPI_Transmit(&hspi1, (uint8_t *) &trama, 1, 50) != HAL_OK )										// Transmito
			Error_Handler();
	while(hspi1.State != HAL_SPI_STATE_READY ) {};
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);						// Subo la linea de CSN	

	for(demora = 0; demora < 1000; demora++){};
			
	trama = 0xA26A;
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);					// Bajo la linea de CSN
	if( HAL_SPI_Transmit(&hspi1, (uint8_t *) &trama, 1, 50) != HAL_OK )										// Transmito
			Error_Handler();
	while(hspi1.State != HAL_SPI_STATE_READY ) {};
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);						// Subo la linea de CSN	

	for(demora = 0; demora < 1000; demora++){};
			
	trama = 0xA361;
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);					// Bajo la linea de CSN
	if( HAL_SPI_Transmit(&hspi1, (uint8_t *) &trama, 1, 50) != HAL_OK )										// Transmito
			Error_Handler();
	while(hspi1.State != HAL_SPI_STATE_READY ) {};
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);						// Subo la linea de CSN	
		
	for(demora = 0; demora < 1000; demora++){};	
		
	trama = 0xA400;
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);					// Bajo la linea de CSN
	if( HAL_SPI_Transmit(&hspi1, (uint8_t *) &trama, 1, 50) != HAL_OK )										// Transmito
			Error_Handler();
	while(hspi1.State != HAL_SPI_STATE_READY ) {};
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);						// Subo la linea de CSN	

	for(demora = 0; demora < 1000; demora++){};
		
	trama = 0xA500;
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);					// Bajo la linea de CSN
	if( HAL_SPI_Transmit(&hspi1, (uint8_t *) &trama, 1, 50) != HAL_OK )										// Transmito
			Error_Handler();
	while(hspi1.State != HAL_SPI_STATE_READY ) {};
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);						// Subo la linea de CSN	
	
	for(demora = 0; demora < 1000; demora++){};
		
	trama = 0xA6FF;
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);					// Bajo la linea de CSN
	if( HAL_SPI_Transmit(&hspi1, (uint8_t *) &trama, 1, 50) != HAL_OK )										// Transmito
			Error_Handler();
	while(hspi1.State != HAL_SPI_STATE_READY ) {};
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);						// Subo la linea de CSN	

	for(demora = 0; demora < 1000; demora++){};		
		
	trama = 0xA7FF;
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);					// Bajo la linea de CSN
	if( HAL_SPI_Transmit(&hspi1, (uint8_t *) &trama, 1, 50) != HAL_OK )										// Transmito
			Error_Handler();
	while(hspi1.State != HAL_SPI_STATE_READY ) {};
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);						// Subo la linea de CSN	

	for(demora = 0; demora < 1000; demora++){};
		
	trama = 0xA80F;
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);					// Bajo la linea de CSN
	if( HAL_SPI_Transmit(&hspi1, (uint8_t *) &trama, 1, 50) != HAL_OK )										// Transmito
			Error_Handler();
	while(hspi1.State != HAL_SPI_STATE_READY ) {};
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);						// Subo la linea de CSN	

	for(demora = 0; demora < 1000; demora++){};		
		
	trama = 0xA90F;
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);					// Bajo la linea de CSN
	if( HAL_SPI_Transmit(&hspi1, (uint8_t *) &trama, 1, 50) != HAL_OK )										// Transmito
			Error_Handler();
	while(hspi1.State != HAL_SPI_STATE_READY ) {};
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);						// Subo la linea de CSN	
		
	// Inicializo el filtro digital
	arm_fir_init_q31( &lowpass, FILTER_LENGTH, lowpassfilter, pState, BUFFER_LENGTH/4 );
}
//

q31_t auxiliar[4] = {0x11112222, 0x33334444, 0x55556666, 0x77778888};
q31_t aux1[1] = {0};
q31_t aux2[1] = {0x40000000};
q31_t aux3[1] = {0x40000000};

// Manejo de buffers en la comunicación I2S
void dma_tx_rx ( void )
{
	uint32_t i = 0, j = 0;
	
	if ( transmission_ready == HAL_OK )
	{
		transmission_ready = HAL_BUSY;
		
		for(i = 0 ; i < BUFFER_LENGTH ; i++)
		{
			
			#if LOOPBACK
		
				buffer_tx_aux[i] = buffer_rx_aux[i];
			
			#else
				
				// Separo la señal de entrada en dos vectores, uno para suma y otro para la resta
				if( i == 0 || CANAL == CH_L_1 )
				{
					canal_L[j] = ((buffer_rx_aux[i] << 16) & 0xFFFF0000);// en vez de 16 para evitar el desborde
					CANAL = CH_L_2;
				}
				else if( CANAL == CH_L_2 )
				{
					canal_L[j] |= (buffer_rx_aux[i] & 0x0000FFFF);
					canal_L[j] = canal_L[j] >> 8;
					CANAL = CH_R_1;
				}
				else if( CANAL == CH_R_1 )
				{
					canal_R[j] = ((buffer_rx_aux[i] << 16) & 0xFFFF0000);
					CANAL = CH_R_2;
				}
				else if( CANAL == CH_R_2 )
				{
					canal_R[j] |= (buffer_rx_aux[i] & 0x0000FFFF);
					canal_R[j] = canal_R[j] >> 8;
					CANAL = CH_L_1;
					j++;
				}
				
			#endif
				
		}		
			
		#if MPX
	
			// Filtro los dos canales de entrada
			// Filtro FIR con fpaso 15KHz y fcorte 19KHz
			arm_fir_q31( &lowpass, canal_L, canal_L_filtrado, BUFFER_LENGTH/4 );
			arm_fir_q31( &lowpass, canal_R, canal_R_filtrado, BUFFER_LENGTH/4 );
		
//			// Armo el vector con la suma			L+R
//			arm_add_q31( (q31_t *) canal_L, (q31_t *) canal_R, (q31_t *) suma, (BUFFER_LENGTH/4) );				// Necesita un desplazamiento hacia la derecha (+1)

//			// Armo el vector con la resta		L-R
//			arm_sub_q31( (q31_t *) canal_L_filtrado, (q31_t *) canal_R_filtrado, (q31_t *) resta, (BUFFER_LENGTH/4) );			// Necesita un desplazamiento hacia la derecha (+1)
//		
//			// Desplazo en frecuencia				 (L-R)*piloto38KHz
//			arm_mult_q31( (q31_t *) resta, (q31_t *) piloto38k, (q31_t *) mpx, (BUFFER_LENGTH/4) );				// No necesita desplazamientos
//		
//			// Armo mpx con L+R y L-R
//			arm_add_q31( (q31_t *) mpx, (q31_t *) suma, (q31_t *) resta, (BUFFER_LENGTH/4) );							// Necesita un desplazamiento hacia la derecha (+2)

//			// Agrego el piloto
//			arm_add_q31( (q31_t *) resta, (q31_t *) piloto19k, (q31_t *) mpx, (BUFFER_LENGTH/4) );				// Necesita un desplazamiento hacia la derecha (+3)

			#if ONLY_LPF
				// Rearmo el vector de salida
				for(i = 0, j=0 ; i < BUFFER_LENGTH ; i+=4, j++)
				{
					buffer_tx_aux[i] = ((canal_L_filtrado[j] >> 16) & 0x0000FFFF);
					buffer_tx_aux[i+2] = ((canal_R_filtrado[j] >> 16) & 0x0000FFFF);

					buffer_tx_aux[i+1] = ((canal_L_filtrado[j] << 16) & 0x0000FFFF);			
					buffer_tx_aux[i+3] = ((canal_R_filtrado[j] << 16) & 0x0000FFFF);			
				}
			#else
				/*	Desplazamientos
				*		Tener en cuenta que estamos trabajando con aritmética de punto fijo, por lo tanto al realizar una suma o resta, su resultado
				*		tendrá una bit de más, por lo tanto debe desplazarse teniendo en cuenta este hecho.
				*		Tenemos 3 sumas:
				*			1- L+R y L-R (que multiplicado por el piloto mantiene la longitud del vector)
				*			2- La suma de los vectores descriptos anteriormente
				*			3- La suma del piloto de 19KHz para completar la señal MPX
				*/

				// Rearmamos el vector a ser transmitido según el protocolo I2S
				for(i = 0, j = 0 ; i < BUFFER_LENGTH ; i+=4, j++)
				{
//					suma[j] = suma[j] << 1;
					buffer_tx_aux[i] = ((suma[j] >> 9) & 0x0000FFFF);
					buffer_tx_aux[i+2] = buffer_tx_aux[i];

					buffer_tx_aux[i+1] = ((suma[j] << 7) & 0x0000FF00);
					buffer_tx_aux[i+3] = buffer_tx_aux[i+1];
				}
			#endif
		
		#endif
	
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

	buffer_tx_aux = buffer_tx;
	buffer_rx_aux = buffer_rx;
		
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

	buffer_tx_aux = &(buffer_tx[BUFFER_LENGTH]);
	buffer_rx_aux = &(buffer_rx[BUFFER_LENGTH]);
	
	transmission_ready = HAL_OK;
}
//



