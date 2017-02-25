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
	arm_fir_init_f32( &lowpass, FILTER_LENGTH, lowpassfilter, pState, BUFFER_LENGTH/4 );
	
		
	// Piloto
//	for(int i = 0 ; i < BUFFER_LENGTH/4 ; i++)
//	{
//			piloto19k[i] = (piloto19k[i] >> 5);
//	}
			
	float32_t max_v;
	uint32_t max_i;
		
	arm_abs_f32(piloto19k, aux, PILOTO_19k_LENGTH);
	arm_max_f32(aux, PILOTO_19k_LENGTH, &max_v, &max_i);
	arm_scale_f32(piloto19k, 1/(2*max_v), piloto19k, PILOTO_19k_LENGTH);
		
	arm_abs_f32(piloto38k, aux, BUFFER_LENGTH/4);
	arm_max_f32(aux, BUFFER_LENGTH/4, &max_v, &max_i);
	arm_scale_f32(piloto38k, 1/(2*max_v), piloto38k, BUFFER_LENGTH/4);
}
//

q31_t auxiliar[4] = {0x11112222, 0x33334444, 0x55556666, 0x77778888};
q31_t aux1[1] = {0};
q31_t aux2[1] = {0x40000000};
q31_t aux3[1] = {0x40000000};

	int tam_restante = 512;

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
					canal_L_q[j] = ((buffer_rx_aux[i] << 16) & 0xFFFF0000);// en vez de 16 para evitar el desborde
					CANAL = CH_L_2;
				}
				else if( CANAL == CH_L_2 )
				{
					canal_L_q[j] |= (buffer_rx_aux[i] & 0x0000FFFF);
					canal_L_q[j] = canal_L_q[j] >> 2;
					CANAL = CH_R_1;
				}
				else if( CANAL == CH_R_1 )
				{
					canal_R_q[j] = ((buffer_rx_aux[i] << 16) & 0xFFFF0000);
					CANAL = CH_R_2;
				}
				else if( CANAL == CH_R_2 )
				{
					canal_R_q[j] |= (buffer_rx_aux[i] & 0x0000FFFF);
					canal_R_q[j] = canal_R_q[j] >> 2;
					CANAL = CH_L_1;
					j++;
				}
				
			#endif
				
		}		
			
		#if MPX
		
		arm_q31_to_float(canal_L_q, canal_L, BUFFER_LENGTH/4);
		arm_q31_to_float(canal_R_q, canal_R, BUFFER_LENGTH/4);
		
		float32_t max_v;
		uint32_t max_i;
		
		arm_abs_f32(canal_L, aux, BUFFER_LENGTH/4);
		arm_max_f32(aux, BUFFER_LENGTH/4, &max_v, &max_i);
		arm_scale_f32(canal_L, 1/(2*max_v), canal_L, BUFFER_LENGTH/4);

		arm_abs_f32(canal_R, aux, BUFFER_LENGTH/4);
		arm_max_f32(aux, BUFFER_LENGTH/4, &max_v, &max_i);
		arm_scale_f32(canal_R, 1/(2*max_v), canal_R, BUFFER_LENGTH/4);
			
	
////			// Filtro los dos canales de entrada
////			// Filtro FIR con fpaso 15KHz y fcorte 19KHz
////			arm_fir_f32( &lowpass, canal_L, canal_L_filtrado, BUFFER_LENGTH/4 );
////			arm_fir_f32( &lowpass, canal_R, canal_R_filtrado, BUFFER_LENGTH/4 );
		
			// Armo el vector con la suma			L+R
			arm_add_f32( (float32_t *) canal_L, (float32_t *) canal_R, suma, (BUFFER_LENGTH/4) );				// Necesita un desplazamiento hacia la derecha (+1)

			arm_abs_f32(suma, aux, BUFFER_LENGTH/4);
			arm_max_f32(aux, BUFFER_LENGTH/4, &max_v, &max_i);
			arm_scale_f32(suma, 1/(2*max_v), suma, BUFFER_LENGTH/4);

			// Armo el vector con la resta		L-R
			arm_sub_f32( (float32_t *) canal_L, (float32_t *) canal_R, resta, (BUFFER_LENGTH/4) );			// Necesita un desplazamiento hacia la derecha (+1)

			arm_abs_f32(resta, aux, BUFFER_LENGTH/4);
			arm_max_f32(aux, BUFFER_LENGTH/4, &max_v, &max_i);
			arm_scale_f32(resta, 1/(2*max_v), resta, BUFFER_LENGTH/4);
		
			// Desplazo en frecuencia				 (L-R)*piloto38KHz
			arm_mult_f32( resta, piloto38k, mpx, (BUFFER_LENGTH/4) );				// No necesita desplazamientos

			arm_abs_f32(mpx, aux, BUFFER_LENGTH/4);
			arm_max_f32(aux, BUFFER_LENGTH/4, &max_v, &max_i);
			arm_scale_f32(mpx, 1/(2*max_v), mpx, BUFFER_LENGTH/4);
		
			// Armo mpx con L+R y L-R
			arm_add_f32( mpx, suma, resta, (BUFFER_LENGTH/4) );							// Necesita un desplazamiento hacia la derecha (+2)

			arm_abs_f32(resta, aux, BUFFER_LENGTH/4);
			arm_max_f32(aux, BUFFER_LENGTH/4, &max_v, &max_i);
			arm_scale_f32(resta, 1/(2*max_v), resta, BUFFER_LENGTH/4);

			
			tam_restante = 512;
			arm_copy_f32(piloto19k + piloto19k_idx, aux, PILOTO_19k_LENGTH - piloto19k_idx);
			tam_restante = tam_restante - (PILOTO_19k_LENGTH - piloto19k_idx);
			if( tam_restante > PILOTO_19k_LENGTH )
			{
				arm_copy_f32(piloto19k, aux + PILOTO_19k_LENGTH - piloto19k_idx, PILOTO_19k_LENGTH);
				tam_restante -= PILOTO_19k_LENGTH;
				piloto19k_idx -= PILOTO_19k_LENGTH;
			}	
			arm_copy_f32(piloto19k, aux + PILOTO_19k_LENGTH - piloto19k_idx, tam_restante);
			piloto19k_idx = tam_restante;

			// Agrego el piloto
			arm_add_f32( resta, aux, mpx, (BUFFER_LENGTH/4) );				// Necesita un desplazamiento hacia la derecha (+3)

			#if ONLY_LPF
			
				// Escalamos el vector
				arm_abs_f32(canal_R, suma, BUFFER_LENGTH/4);
				arm_max_f32(suma, BUFFER_LENGTH/4, &max_v, &max_i);
				arm_scale_f32(canal_R, 1/(2*max_v), canal_R, BUFFER_LENGTH/4);
				
				arm_float_to_q31(canal_R, canal_R_q, BUFFER_LENGTH/4);
			
				arm_abs_f32(canal_L, suma, BUFFER_LENGTH/4);
				arm_max_f32(suma, BUFFER_LENGTH/4, &max_v, &max_i);
				arm_scale_f32(canal_L, 1/(2*max_v), canal_L, BUFFER_LENGTH/4);
				
				arm_float_to_q31(canal_L, canal_L_q, BUFFER_LENGTH/4);
			
				// Rearmo el vector de salida
				for(i = 0, j=0 ; i < BUFFER_LENGTH ; i+=4, j++)
				{
					buffer_tx_aux[i] = ((canal_L_q[j] >> 16) & 0x0000FFFF);
					buffer_tx_aux[i+2] = ((canal_R_q[j] >> 16) & 0x0000FFFF);

					buffer_tx_aux[i+1] = ((canal_L_q[j] << 0) & 0x0000FFFF);			
					buffer_tx_aux[i+3] = ((canal_R_q[j] << 0) & 0x0000FFFF);			
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

				// Escalamos el vector
				arm_abs_f32(mpx, aux, BUFFER_LENGTH/4);
				arm_max_f32(aux, BUFFER_LENGTH/4, &max_v, &max_i);
				arm_scale_f32(mpx, 1/(2*max_v), mpx, BUFFER_LENGTH/4);
				
				arm_float_to_q31(mpx, mpx_q, BUFFER_LENGTH/4);
				
				// Rearmamos el vector a ser transmitido según el protocolo I2S
				for(i = 0, j = 0 ; i < BUFFER_LENGTH ; i+=4, j++)
				{
						buffer_tx_aux[i] = ((mpx_q[j] >> 16) & 0x0000FFFF);
						buffer_tx_aux[i+2] = buffer_tx_aux[i];

						buffer_tx_aux[i+1] = ((mpx_q[j] << 0) & 0x0000FF00);
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



