/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */

	#define ARM_MATH_CM4
	#include "arm_math.h"
	#include "codec.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
I2S_HandleTypeDef hi2s3;
DMA_HandleTypeDef hdma_spi3_tx;
DMA_HandleTypeDef hdma_i2s3_ext_rx;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim6;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2S3_Init(void);
static void MX_TIM6_Init(void);
static void MX_SPI1_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

	// Manejo de los callback												
	uint8_t toggle_buffer = 0;

	// Buffers de comunicacion I2S
	uint32_t buffer_rx[BUFFER_LENGTH*2] = {0};
	uint32_t buffer_tx[BUFFER_LENGTH*2] = {0};

	// Punteros auxiliares para realizar dsp												
	uint32_t *buffer_tx_aux;
	uint32_t *buffer_rx_aux;

	// Buffers dsp
	q31_t canal_L_q[BUFFER_LENGTH/4] = {0};
	float32_t canal_L[BUFFER_LENGTH/4] = {0};
	q31_t canal_R_q[BUFFER_LENGTH/4] = {0};
	float32_t canal_R[BUFFER_LENGTH/4] = {0};
	float32_t canal_L_filtrado[BUFFER_LENGTH/4] = {0};
	float32_t canal_R_filtrado[BUFFER_LENGTH/4] = {0};
	float32_t suma[BUFFER_LENGTH/4] = {0};
	float32_t resta[BUFFER_LENGTH/4] = {0};
	float32_t aux[BUFFER_LENGTH/4] = {0};
	float32_t mpx[BUFFER_LENGTH/4] = {0};
	q31_t mpx_q[BUFFER_LENGTH/4] = {0};
	float32_t lowpassfilter[FILTER_LENGTH] = {-0.002718, -0.001551, -0.000426, 0.002050, 0.005458, 0.008797, 0.010769, 0.010297, 0.007058, 0.001805, -0.003738, 
-0.007453, -0.007690, -0.004072, 0.002127, 0.008316, 0.011556, 0.009855, 0.003226, -0.006009, -0.013922, 
-0.016546, -0.011702, -0.000316, 0.013454, 0.023529, 0.024313, 0.013319, -0.007077, -0.029612, -0.044091, 
-0.040834, -0.014402, 0.033751, 0.094846, 0.154885, 0.198726, 0.214782, 0.198726, 0.154885, 0.094846, 
0.033751, -0.014402, -0.040834, -0.044091, -0.029612, -0.007077, 0.013319, 0.024313, 0.023529, 0.013454, 
-0.000316, -0.011702, -0.016546, -0.013922, -0.006009, 0.003226, 0.009855, 0.011556, 0.008316, 0.002127, 
-0.004072, -0.007690, -0.007453, -0.003738, 0.001805, 0.007058, 0.010297, 0.010769, 0.008797, 0.005458, 
0.002050, -0.000426, -0.001551, -0.002718};
	uint32_t piloto19k_idx = 0;
	float32_t piloto19k[PILOTO_19k_LENGTH] = {0.000000, 0.707107, 1.000000, 0.707107, 0.000000, -0.707107, -1.000000, -0.707107, -0.000000, 0.707107, 1.000000, 
0.707107, 0.000000, -0.707107, -1.000000, -0.707107, -0.000000, 0.707107, 1.000000, 0.707107, 0.000000, 
-0.707107, -1.000000, -0.707107, -0.000000, 0.707107, 1.000000, 0.707107, 0.000000, -0.707107, -1.000000, 
-0.707107, -0.000000, 0.707107, 1.000000, 0.707107, 0.000000, -0.707107, -1.000000, -0.707107, -0.000000, 
0.707107, 1.000000, 0.707107, -0.000000, -0.707107, -1.000000, -0.707107, -0.000000, 0.707107, 1.000000, 
0.707107, -0.000000, -0.707107, -1.000000, -0.707107, -0.000000, 0.707107, 1.000000, 0.707107, -0.000000, 
-0.707107, -1.000000, -0.707107, -0.000000, 0.707107, 1.000000, 0.707107, 0.000000, -0.707107, -1.000000, 
-0.707107, -0.000000, 0.707107, 1.000000, 0.707107, 0.000000, -0.707107, -1.000000, -0.707107, -0.000000, 
0.707107, 1.000000, 0.707107, -0.000000, -0.707107, -1.000000, -0.707107, 0.000000, 0.707107, 1.000000, 
0.707107, 0.000000, -0.707107, -1.000000, -0.707107, -0.000000, 0.707107, 1.000000, 0.707107, 0.000000, 
-0.707107, -1.000000, -0.707107, 0.000000, 0.707107, 1.000000, 0.707107, -0.000000, -0.707107, -1.000000, 
-0.707107, -0.000000, 0.707107, 1.000000, 0.707107, -0.000000, -0.707107, -1.000000, -0.707107, 0.000000, 
0.707107, 1.000000, 0.707107, -0.000000, -0.707107, -1.000000, -0.707107, -0.000000, 0.707107, 1.000000, 
0.707107, 0.000000, -0.707107, -1.000000, -0.707107, -0.000000, 0.707107, 1.000000, 0.707107, -0.000000, 
-0.707107, -1.000000, -0.707107, -0.000000, 0.707107, 1.000000, 0.707107, 0.000000, -0.707107, -1.000000, 
-0.707107, -0.000000, 0.707107, 1.000000, 0.707107, 0.000000, -0.707107, -1.000000, -0.707107, -0.000000, 
0.707107, 1.000000, 0.707107, 0.000000, -0.707107, -1.000000, -0.707107, 0.000000, 0.707107, 1.000000, 
0.707107, -0.000000, -0.707107, -1.000000, -0.707107, 0.000000, 0.707107, 1.000000, 0.707107, 0.000000, 
-0.707107, -1.000000, -0.707107, -0.000000, 0.707107, 1.000000, 0.707107, -0.000000, -0.707107, -1.000000, 
-0.707107, -0.000000, 0.707107, 1.000000, 0.707107, 0.000000, -0.707107, -1.000000, -0.707107, -0.000000, 
0.707107, 1.000000, 0.707107, -0.000000, -0.707107, -1.000000, -0.707107, 0.000000, 0.707107, 1.000000, 
0.707107, 0.000000, -0.707107, -1.000000, -0.707107, 0.000000, 0.707107, 1.000000, 0.707107, -0.000000, 
-0.707107, -1.000000, -0.707107, -0.000000, 0.707107, 1.000000, 0.707107, 0.000000, -0.707107, -1.000000, 
-0.707107, 0.000000, 0.707107, 1.000000, 0.707107, 0.000000, -0.707107, -1.000000, -0.707107, 0.000000, 
0.707107, 1.000000, 0.707107, 0.000000, -0.707107, -1.000000, -0.707107, 0.000000, 0.707107, 1.000000, 
0.707107, -0.000000, -0.707107, -1.000000, -0.707107, -0.000000, 0.707107, 1.000000, 0.707107, 0.000000, 
-0.707107, -1.000000, -0.707107, -0.000000, 0.707107, 1.000000, 0.707107, 0.000000, -0.707107, -1.000000, 
-0.707107, -0.000000, 0.707107, 1.000000, 0.707107, -0.000000, -0.707107, -1.000000, -0.707107, 0.000000, 
0.707107, 1.000000, 0.707107, -0.000000, -0.707107, -1.000000, -0.707107, -0.000000, 0.707107, 1.000000, 
0.707107, -0.000000, -0.707107, -1.000000, -0.707107, -0.000000, 0.707107, 1.000000, 0.707107, 0.000000, 
-0.707107, -1.000000, -0.707107, -0.000000, 0.707107, 1.000000, 0.707107, 0.000000, -0.707107, -1.000000, 
-0.707107, -0.000000, 0.707107, 1.000000, 0.707107, 0.000000, -0.707107, -1.000000, -0.707107, -0.000000, 
0.707107, 1.000000, 0.707107, 0.000000, -0.707107, -1.000000, -0.707107, -0.000000, 0.707107, 1.000000, 
0.707107, -0.000000, -0.707107, -1.000000, -0.707107, 0.000000, 0.707107, 1.000000, 0.707107, 0.000000, 
-0.707107, -1.000000, -0.707107, 0.000000, 0.707107, 1.000000, 0.707107, -0.000000, -0.707107, -1.000000, 
-0.707107, 0.000000, 0.707107, 1.000000, 0.707107, -0.000000, -0.707107, -1.000000, -0.707107, -0.000000, 
0.707107, 1.000000, 0.707107, 0.000000, -0.707107, -1.000000, -0.707107, -0.000000, 0.707107, 1.000000, 
0.707107, -0.000000, -0.707107, -1.000000, -0.707107};
	uint32_t piloto38k_idx = 0;
	float32_t piloto38k[PILOTO_38k_LENGTH] = {0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 1.000000, 0.000000, 
-1.000000, -0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 
1.000000, -0.000000, -1.000000, -0.000000, 1.000000, -0.000000, -1.000000, -0.000000, 1.000000, -0.000000, 
-1.000000, -0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 
1.000000, -0.000000, -1.000000, 0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 1.000000, 0.000000, 
-1.000000, 0.000000, 1.000000, -0.000000, -1.000000, -0.000000, 1.000000, -0.000000, -1.000000, 0.000000, 
1.000000, -0.000000, -1.000000, -0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 1.000000, -0.000000, 
-1.000000, -0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 
1.000000, 0.000000, -1.000000, 0.000000, 1.000000, -0.000000, -1.000000, 0.000000, 1.000000, 0.000000, 
-1.000000, -0.000000, 1.000000, -0.000000, -1.000000, -0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 
1.000000, -0.000000, -1.000000, 0.000000, 1.000000, 0.000000, -1.000000, 0.000000, 1.000000, -0.000000, 
-1.000000, -0.000000, 1.000000, 0.000000, -1.000000, 0.000000, 1.000000, 0.000000, -1.000000, 0.000000, 
1.000000, 0.000000, -1.000000, 0.000000, 1.000000, -0.000000, -1.000000, -0.000000, 1.000000, 0.000000, 
-1.000000, -0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 1.000000, -0.000000, -1.000000, 0.000000, 
1.000000, -0.000000, -1.000000, -0.000000, 1.000000, -0.000000, -1.000000, -0.000000, 1.000000, 0.000000, 
-1.000000, -0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 
1.000000, 0.000000, -1.000000, -0.000000, 1.000000, -0.000000, -1.000000, 0.000000, 1.000000, 0.000000, 
-1.000000, 0.000000, 1.000000, -0.000000, -1.000000, 0.000000, 1.000000, -0.000000, -1.000000, -0.000000, 
1.000000, 0.000000, -1.000000, -0.000000, 1.000000, -0.000000, -1.000000, 0.000000, 1.000000, 0.000000, 
-1.000000, -0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 
1.000000, -0.000000, -1.000000, 0.000000, 1.000000, -0.000000, -1.000000, 0.000000, 1.000000, -0.000000, 
-1.000000, -0.000000, 1.000000, 0.000000, -1.000000, 0.000000, 1.000000, -0.000000, -1.000000, 0.000000, 
1.000000, 0.000000, -1.000000, -0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 1.000000, 0.000000, 
-1.000000, 0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 1.000000, -0.000000, -1.000000, 0.000000, 
1.000000, -0.000000, -1.000000, -0.000000, 1.000000, -0.000000, -1.000000, 0.000000, 1.000000, -0.000000, 
-1.000000, 0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 
1.000000, -0.000000, -1.000000, -0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 1.000000, 0.000000, 
-1.000000, -0.000000, 1.000000, 0.000000, -1.000000, 0.000000, 1.000000, -0.000000, -1.000000, 0.000000, 
1.000000, -0.000000, -1.000000, 0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 1.000000, 0.000000, 
-1.000000, 0.000000, 1.000000, -0.000000, -1.000000, -0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 
1.000000, 0.000000, -1.000000, -0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 1.000000, 0.000000, 
-1.000000, -0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 
1.000000, 0.000000, -1.000000, -0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 1.000000, -0.000000, 
-1.000000, 0.000000, 1.000000, -0.000000, -1.000000, 0.000000, 1.000000, 0.000000, -1.000000, -0.000000, 
1.000000, 0.000000, -1.000000, 0.000000, 1.000000, -0.000000, -1.000000, 0.000000, 1.000000, -0.000000, 
-1.000000, 0.000000, 1.000000, -0.000000, -1.000000, 0.000000, 1.000000, -0.000000, -1.000000, -0.000000, 
1.000000, 0.000000, -1.000000, -0.000000, 1.000000, 0.000000, -1.000000};
	float32_t pState[PSTATE_LENGTH] = {0};
	
//	// Para 96KHz
//	float32_t piloto19k[BUFFER_LENGTH/4] = {0.000000, 0.946930, 0.608761, -0.555570, -0.965926, -0.065403, 0.923880, 0.659346, -0.500000, -0.980785, -0.130526, 
//0.896873, 0.707107, -0.442289, -0.991445, -0.195090, 0.866025, 0.751840, -0.382683, -0.997859, -0.258819, 
//0.831470, 0.793353, -0.321439, -1.000000, -0.321439, 0.793353, 0.831470, -0.258819, -0.997859, -0.382683, 
//0.751840, 0.866025, -0.195090, -0.991445, -0.442289, 0.707107, 0.896873, -0.130526, -0.980785, -0.500000, 
//0.659346, 0.923880, -0.065403, -0.965926, -0.555570, 0.608761, 0.946930, 0.000000, -0.946930, -0.608761, 
//0.555570, 0.965926, 0.065403, -0.923880, -0.659346, 0.500000, 0.980785, 0.130526, -0.896873, -0.707107, 
//0.442289, 0.991445, 0.195090, -0.866025, -0.751840, 0.382683, 0.997859, 0.258819, -0.831470, -0.793353, 
//0.321439, 1.000000, 0.321439, -0.793353, -0.831470, 0.258819, 0.997859, 0.382683, -0.751840, -0.866025, 
//0.195090, 0.991445, 0.442289, -0.707107, -0.896873, 0.130526, 0.980785, 0.500000, -0.659346, -0.923880, 
//0.065403, 0.965926, 0.555570, -0.608761, -0.946930, -0.000000, 0.946930, 0.608761, -0.555570, -0.965926, 
//-0.065403, 0.923880, 0.659346, -0.500000, -0.980785, -0.130526, 0.896873, 0.707107, -0.442289, -0.991445, 
//-0.195090, 0.866025, 0.751840, -0.382683, -0.997859, -0.258819, 0.831470, 0.793353, -0.321439, -1.000000, 
//-0.321439, 0.793353, 0.831470, -0.258819, -0.997859, -0.382683, 0.751840, 0.866025, -0.195090, -0.991445, 
//-0.442289, 0.707107, 0.896873, -0.130526, -0.980785, -0.500000, 0.659346, 0.923880, -0.065403, -0.965926, 
//-0.555570, 0.608761, 0.946930, 0.000000, -0.946930, -0.608761, 0.555570, 0.965926, 0.065403, -0.923880, 
//-0.659346, 0.500000, 0.980785, 0.130526, -0.896873, -0.707107, 0.442289, 0.991445, 0.195090, -0.866025, 
//-0.751840, 0.382683, 0.997859, 0.258819, -0.831470, -0.793353, 0.321439, 1.000000, 0.321439, -0.793353, 
//-0.831470, 0.258819, 0.997859, 0.382683, -0.751840, -0.866025, 0.195090, 0.991445, 0.442289, -0.707107, 
//-0.896873, 0.130526, 0.980785, 0.500000, -0.659346, -0.923880, 0.065403, 0.965926, 0.555570, -0.608761, 
//-0.946930, -0.000000, 0.946930, 0.608761, -0.555570, -0.965926, -0.065403, 0.923880, 0.659346, -0.500000, 
//-0.980785, -0.130526, 0.896873, 0.707107, -0.442289, -0.991445, -0.195090, 0.866025, 0.751840, -0.382683, 
//-0.997859, -0.258819, 0.831470, 0.793353, -0.321439, -1.000000, -0.321439, 0.793353, 0.831470, -0.258819, 
//-0.997859, -0.382683, 0.751840, 0.866025, -0.195090, -0.991445, -0.442289, 0.707107, 0.896873, -0.130526, 
//-0.980785, -0.500000, 0.659346, 0.923880, -0.065403, -0.965926, -0.555570, 0.608761, 0.946930, 0.000000, 
//-0.946930, -0.608761, 0.555570, 0.965926, 0.065403, -0.923880, -0.659346, 0.500000, 0.980785, 0.130526, 
//-0.896873, -0.707107, 0.442289, 0.991445, 0.195090, -0.866025, -0.751840, 0.382683, 0.997859, 0.258819, 
//-0.831470, -0.793353, 0.321439, 1.000000, 0.321439, -0.793353, -0.831470, 0.258819, 0.997859, 0.382683, 
//-0.751840, -0.866025, 0.195090, 0.991445, 0.442289, -0.707107, -0.896873, 0.130526, 0.980785, 0.500000, 
//-0.659346, -0.923880, 0.065403, 0.965926, 0.555570, -0.608761, -0.946930, -0.000000, 0.946930, 0.608761, 
//-0.555570, -0.965926, -0.065403, 0.923880, 0.659346, -0.500000, -0.980785, -0.130526, 0.896873, 0.707107, 
//-0.442289, -0.991445, -0.195090, 0.866025, 0.751840, -0.382683, -0.997859, -0.258819, 0.831470, 0.793353, 
//-0.321439, -1.000000, -0.321439, 0.793353, 0.831470, -0.258819, -0.997859, -0.382683, 0.751840, 0.866025, 
//-0.195090, -0.991445, -0.442289, 0.707107, 0.896873, -0.130526, -0.980785, -0.500000, 0.659346, 0.923880, 
//-0.065403, -0.965926, -0.555570, 0.608761, 0.946930, 0.000000, -0.946930, -0.608761, 0.555570, 0.965926, 
//0.065403, -0.923880, -0.659346, 0.500000, 0.980785, 0.130526, -0.896873, -0.707107, 0.442289, 0.991445, 
//0.195090, -0.866025, -0.751840, 0.382683, 0.997859, 0.258819, -0.831470, -0.793353, 0.321439, 1.000000, 
//0.321439, -0.793353, -0.831470, 0.258819, 0.997859, 0.382683, -0.751840, -0.866025, 0.195090, 0.991445, 
//0.442289, -0.707107, -0.896873, 0.130526, 0.980785, 0.500000, -0.659346, -0.923880, 0.065403, 0.965926, 
//0.555570, -0.608761, -0.946930, -0.000000, 0.946930, 0.608761, -0.555570, -0.965926, -0.065403, 0.923880, 
//0.659346, -0.500000, -0.980785, -0.130526, 0.896873, 0.707107, -0.442289, -0.991445, -0.195090, 0.866025, 
//0.751840, -0.382683, -0.997859, -0.258819, 0.831470, 0.793353, -0.321439, -1.000000, -0.321439, 0.793353, 
//0.831470, -0.258819, -0.997859, -0.382683, 0.751840, 0.866025, -0.195090, -0.991445, -0.442289, 0.707107, 
//0.896873, -0.130526, -0.980785, -0.500000, 0.659346, 0.923880, -0.065403, -0.965926, -0.555570, 0.608761, 
//0.946930, 0.000000, -0.946930, -0.608761, 0.555570, 0.965926, 0.065403, -0.923880, -0.659346, 0.500000, 
//0.980785, 0.130526, -0.896873, -0.707107, 0.442289, 0.991445, 0.195090, -0.866025, -0.751840, 0.382683, 
//0.997859, 0.258819, -0.831470, -0.793353, 0.321439, 1.000000, 0.321439, -0.793353, -0.831470, 0.258819, 
//0.997859, 0.382683, -0.751840, -0.866025, 0.195090, 0.991445, 0.442289, -0.707107, -0.896873, 0.130526, 
//0.980785, 0.500000, -0.659346, -0.923880, 0.065403, 0.965926, 0.555570, -0.608761, -0.946930, -0.000000, 
//0.946930, 0.608761, -0.555570, -0.965926, -0.065403, 0.923880, 0.659346, -0.500000, -0.980785, -0.130526, 
//0.896873, 0.707107, -0.442289, -0.991445, -0.195090, 0.866025, 0.751840, -0.382683, -0.997859, -0.258819, 
//0.831470, 0.793353, -0.321439, -1.000000, -0.321439, 0.793353, 0.831470, -0.258819, -0.997859, -0.382683, 
//0.000000};

	// Armado de los canales
	volatile uint8_t CANAL = CH_L_1;

	// Filtro digital
	arm_fir_instance_f32 lowpass = {0};


/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

	
	
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2S3_Init();
  MX_TIM6_Init();
  MX_SPI1_Init();

  /* USER CODE BEGIN 2 */

	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Pruebas arm.math
	////////////////////////////////////////////////////////////////////////////////////////////////////

//	arm_biquad_cascade_df2T_instance_f32 *S;
//	uint8_t numStages;
//	float32_t *pCoeffs;
//	float32_t *pState;
//	arm_biquad_cascade_df2T_init_f32 (S,numStages,pCoeffs,pState);
	
//	uint32_t blockSize=BUFFER_LENGTH;
//	float32_t pSrcA[BUFFER_LENGTH]={1};
//	float32_t pSrcB[BUFFER_LENGTH]={2};
//	float32_t pDst[BUFFER_LENGTH];
	

	buffer_tx_aux = buffer_tx;
	
	toggle_buffer = BUFFER_A;
	
	if( HAL_I2SEx_TransmitReceive_DMA(&hi2s3, (uint16_t *) buffer_tx, (uint16_t *) buffer_rx, BUFFER_LENGTH) != HAL_OK )
	{
			Error_Handler();
	}
	
	AK4621EF_init();

	HAL_GPIO_WritePin(TEST_PIN_GPIO_Port, TEST_PIN_Pin, GPIO_PIN_RESET);
	
	
	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		
		led_secuencia();
		dma_tx_rx();

  }
	
	//*********************************************************
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 11;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 212;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* I2S3 init function */
static void MX_I2S3_Init(void)
{

  hi2s3.Instance = SPI3;
  hi2s3.Init.Mode = I2S_MODE_MASTER_TX;
  hi2s3.Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s3.Init.DataFormat = I2S_DATAFORMAT_32B;
  hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  hi2s3.Init.AudioFreq = ((uint32_t)152000U);
  hi2s3.Init.CPOL = I2S_CPOL_LOW;
  hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
  hi2s3.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_ENABLE;
  if (HAL_I2S_Init(&hi2s3) != HAL_OK)
  {
    Error_Handler();
  }

}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }

}

/* TIM6 init function */
static void MX_TIM6_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 84;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 1;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
     PC3   ------> I2S2_SD
     PB10   ------> I2S2_CK
     PA9   ------> USB_OTG_FS_VBUS
     PA10   ------> USB_OTG_FS_ID
     PA11   ------> USB_OTG_FS_DM
     PA12   ------> USB_OTG_FS_DP
     PB6   ------> I2C1_SCL
     PB9   ------> I2C1_SDA
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(CS_I2C_SPI_GPIO_Port, CS_I2C_SPI_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin 
                          |CDTI_Pin|CCLK_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TEST_PIN_GPIO_Port, TEST_PIN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Audio_RST_GPIO_Port, Audio_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, CSN_Pin|PDN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : CS_I2C_SPI_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CS_I2C_SPI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PC3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BOOT1_Pin */
  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PB10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LD4_Pin LD3_Pin LD5_Pin LD6_Pin 
                           CDTI_Pin Audio_RST_Pin CCLK_Pin */
  GPIO_InitStruct.Pin = LD4_Pin|LD3_Pin|LD5_Pin|LD6_Pin 
                          |CDTI_Pin|Audio_RST_Pin|CCLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : VBUS_FS_Pin */
  GPIO_InitStruct.Pin = VBUS_FS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(VBUS_FS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : OTG_FS_ID_Pin OTG_FS_DM_Pin OTG_FS_DP_Pin */
  GPIO_InitStruct.Pin = OTG_FS_ID_Pin|OTG_FS_DM_Pin|OTG_FS_DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : TEST_PIN_Pin */
  GPIO_InitStruct.Pin = TEST_PIN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(TEST_PIN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OverCurrent_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : CSN_Pin PDN_Pin */
  GPIO_InitStruct.Pin = CSN_Pin|PDN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : Audio_SCL_Pin Audio_SDA_Pin */
  GPIO_InitStruct.Pin = Audio_SCL_Pin|Audio_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MEMS_INT2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
	uint32_t i = 0;
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
		if(i == 0)
			HAL_GPIO_WritePin(GPIOD, LD3_Pin|LD4_Pin|LD5_Pin|LD6_Pin, GPIO_PIN_SET);
		
		i++;
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
