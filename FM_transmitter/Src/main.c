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
	float32_t *canal_L = {0};
	q31_t canal_R_q[BUFFER_LENGTH/4] = {0};
	float32_t *canal_R = {0};
	float32_t canal_L_filtrado[BUFFER_LENGTH/4] = {0};
	float32_t canal_R_filtrado[BUFFER_LENGTH/4] = {0};
	float32_t suma[BUFFER_LENGTH/4] = {0};
	float32_t resta[BUFFER_LENGTH/4] = {0};
	float32_t mpx[BUFFER_LENGTH/4] = {0};
	q31_t mpx_q[BUFFER_LENGTH/4] = {0};
	float32_t lowpassfilter[FILTER_LENGTH] = {-0.002466, -0.001076, -0.000476, 0.000844, 0.002797, 0.005077, 0.007198, 0.008594, 0.008772, 0.007472, 0.004787, 
0.001195, -0.002507, -0.005386, -0.006635, -0.005802, -0.002991, 0.001118, 0.005386, 0.008508, 0.009376, 
0.007441, 0.002945, -0.003053, -0.008893, -0.012746, -0.013140, -0.009459, -0.002249, 0.006787, 0.015125, 
0.020056, 0.019454, 0.012500, 0.000130, -0.014926, -0.028585, -0.036264, -0.033962, -0.019321, 0.007614, 
0.044131, 0.085199, 0.124373, 0.155066, 0.171915, 0.171915, 0.155066, 0.124373, 0.085199, 0.044131, 
0.007614, -0.019321, -0.033962, -0.036264, -0.028585, -0.014926, 0.000130, 0.012500, 0.019454, 0.020056, 
0.015125, 0.006787, -0.002249, -0.009459, -0.013140, -0.012746, -0.008893, -0.003053, 0.002945, 0.007441, 
0.009376, 0.008508, 0.005386, 0.001118, -0.002991, -0.005802, -0.006635, -0.005386, -0.002507, 0.001195, 
0.004787, 0.007472, 0.008772, 0.008594, 0.007198, 0.005077, 0.002797, 0.000844, -0.000476, -0.001076, 
-0.002466};
	float32_t piloto19k[BUFFER_LENGTH/4] = {0.000000, 0.594542, 0.956100, 0.942991, 0.560350, -0.041876, -0.627691, -0.967533, -0.928226, -0.525175, 0.083678, 
0.659739, 0.977268, 0.911834, 0.489078, -0.125333, -0.690630, -0.985289, -0.893841, -0.452123, 0.166769, 
0.720309, 0.991581, 0.874281, 0.414376, -0.207912, -0.748724, -0.996134, -0.853187, -0.375901, 0.248690, 
0.775826, 0.998939, 0.830596, 0.336767, -0.289032, -0.801567, -0.999991, -0.806548, -0.297042, 0.328867, 
0.825902, 0.999289, 0.781085, 0.256795, -0.368125, -0.848787, -0.996835, -0.754251, -0.216099, 0.406737, 
0.870184, 0.992631, 0.726095, 0.175023, -0.444635, -0.890054, -0.986686, -0.696664, -0.133640, 0.481754, 
0.908362, 0.979010, 0.666012, 0.092023, -0.518027, -0.925077, -0.969616, -0.634191, -0.050244, 0.553392, 
0.940169, 0.958522, 0.601257, 0.008377, -0.587785, -0.953612, -0.945746, -0.567269, 0.033504, 0.621148, 
0.965382, 0.931310, 0.532285, -0.075327, -0.653421, -0.975458, -0.915241, -0.496368, 0.117017, 0.684547, 
0.983823, 0.897566, 0.459580, -0.158503, -0.714473, -0.990461, -0.878317, -0.421985, 0.199710, 0.743145, 
0.995363, 0.857527, 0.383651, -0.240567, -0.770513, -0.998518, -0.835232, -0.344643, 0.281002, 0.796530, 
0.999921, 0.811472, 0.305031, -0.320944, -0.821149, -0.999570, -0.786288, -0.264883, 0.360322, 0.844328, 
0.997466, 0.759725, 0.224271, -0.399069, -0.866025, -0.993611, -0.731830, -0.183265, 0.437116, 0.886204, 
0.988014, 0.702650, 0.141938, -0.474396, -0.904827, -0.980683, -0.672238, -0.100362, 0.510843, 0.921863, 
0.971632, 0.640646, 0.058609, -0.546394, -0.937282, -0.960876, -0.607930, -0.016754, 0.580987, 0.951057, 
0.948434, 0.574148, -0.025130, -0.614561, -0.963163, -0.934329, -0.539359, 0.066970, 0.647056, 0.973579, 
0.918584, 0.503623, -0.108693, -0.678416, -0.982287, -0.901228, -0.467004, 0.150226, 0.708586, 0.989272, 
0.882291, 0.429566, -0.191494, -0.737513, -0.994522, -0.861806, -0.391374, 0.232427, 0.765146, 0.998027, 
0.839809, 0.352495, -0.272952, -0.791437, -0.999781, -0.816339, -0.312998, 0.312998, 0.816339, 0.999781, 
0.791437, 0.272952, -0.352495, -0.839809, -0.998027, -0.765146, -0.232427, 0.391374, 0.861806, 0.994522, 
0.737513, 0.191494, -0.429566, -0.882291, -0.989272, -0.708586, -0.150226, 0.467004, 0.901228, 0.982287, 
0.678416, 0.108693, -0.503623, -0.918584, -0.973579, -0.647056, -0.066970, 0.539359, 0.934329, 0.963163, 
0.614561, 0.025130, -0.574148, -0.948434, -0.951057, -0.580987, 0.016754, 0.607930, 0.960876, 0.937282, 
0.546394, -0.058609, -0.640646, -0.971632, -0.921863, -0.510843, 0.100362, 0.672238, 0.980683, 0.904827, 
0.474396, -0.141938, -0.702650, -0.988014, -0.886204, -0.437116, 0.183265, 0.731830, 0.993611, 0.866025, 
0.399069, -0.224271, -0.759725, -0.997466, -0.844328, -0.360322, 0.264883, 0.786288, 0.999570, 0.821149, 
0.320944, -0.305031, -0.811472, -0.999921, -0.796530, -0.281002, 0.344643, 0.835232, 0.998518, 0.770513, 
0.240567, -0.383651, -0.857527, -0.995363, -0.743145, -0.199710, 0.421985, 0.878317, 0.990461, 0.714473, 
0.158503, -0.459580, -0.897566, -0.983823, -0.684547, -0.117017, 0.496368, 0.915241, 0.975458, 0.653421, 
0.075327, -0.532285, -0.931310, -0.965382, -0.621148, -0.033504, 0.567269, 0.945746, 0.953612, 0.587785, 
-0.008377, -0.601257, -0.958522, -0.940169, -0.553392, 0.050244, 0.634191, 0.969616, 0.925077, 0.518027, 
-0.092023, -0.666012, -0.979010, -0.908362, -0.481754, 0.133640, 0.696664, 0.986686, 0.890054, 0.444635, 
-0.175023, -0.726095, -0.992631, -0.870184, -0.406737, 0.216099, 0.754251, 0.996835, 0.848787, 0.368125, 
-0.256795, -0.781085, -0.999289, -0.825902, -0.328867, 0.297042, 0.806548, 0.999991, 0.801567, 0.289032, 
-0.336767, -0.830596, -0.998939, -0.775826, -0.248690, 0.375901, 0.853187, 0.996134, 0.748724, 0.207912, 
-0.414376, -0.874281, -0.991581, -0.720309, -0.166769, 0.452123, 0.893841, 0.985289, 0.690630, 0.125333, 
-0.489078, -0.911834, -0.977268, -0.659739, -0.083678, 0.525175, 0.928226, 0.967533, 0.627691, 0.041876, 
-0.560350, -0.942991, -0.956100, -0.594542, -0.000000, 0.594542, 0.956100, 0.942991, 0.560350, -0.041876, 
-0.627691, -0.967533, -0.928226, -0.525175, 0.083678, 0.659739, 0.977268, 0.911834, 0.489078, -0.125333, 
-0.690630, -0.985289, -0.893841, -0.452123, 0.166769, 0.720309, 0.991581, 0.874281, 0.414376, -0.207912, 
-0.748724, -0.996134, -0.853187, -0.375901, 0.248690, 0.775826, 0.998939, 0.830596, 0.336767, -0.289032, 
-0.801567, -0.999991, -0.806548, -0.297042, 0.328867, 0.825902, 0.999289, 0.781085, 0.256795, -0.368125, 
-0.848787, -0.996835, -0.754251, -0.216099, 0.406737, 0.870184, 0.992631, 0.726095, 0.175023, -0.444635, 
-0.890054, -0.986686, -0.696664, -0.133640, 0.481754, 0.908362, 0.979010, 0.666012, 0.092023, -0.518027, 
-0.925077, -0.969616, -0.634191, -0.050244, 0.553392, 0.940169, 0.958522, 0.601257, 0.008377, -0.587785, 
-0.953612, -0.945746, -0.567269, 0.033504, 0.621148, 0.965382, 0.931310, 0.532285, -0.075327, -0.653421, 
-0.975458, -0.915241, -0.496368, 0.117017, 0.684547, 0.983823, 0.897566, 0.459580, -0.158503, -0.714473, 
-0.990461, -0.878317, -0.421985, 0.199710, 0.743145, 0.995363, 0.857527, 0.383651, -0.240567, -0.770513, 
-0.998518, -0.835232, -0.344643, 0.281002, 0.796530, 0.999921, 0.811472, 0.305031, -0.320944, -0.821149, 
-0.999570, -0.786288, -0.264883, 0.360322, 0.844328, 0.997466, 0.759725, 0.224271, -0.399069, -0.866025, 
-0.993611, -0.731830, -0.183265, 0.437116, 0.886204, 0.988014, 0.702650, 0.141938, 0.000000, 0.000000, 
0.000000};
	float32_t piloto38k[BUFFER_LENGTH/4] = {0.000000, 0.956100, 0.560350, -0.627691, -0.928226, 0.083678, 0.977268, 0.489078, -0.690630, -0.893841, 0.166769, 
0.991581, 0.414376, -0.748724, -0.853187, 0.248690, 0.998939, 0.336767, -0.801567, -0.806548, 0.328867, 
0.999289, 0.256795, -0.848787, -0.754251, 0.406737, 0.992631, 0.175023, -0.890054, -0.696664, 0.481754, 
0.979010, 0.092023, -0.925077, -0.634191, 0.553392, 0.958522, 0.008377, -0.953612, -0.567269, 0.621148, 
0.931310, -0.075327, -0.975458, -0.496368, 0.684547, 0.897566, -0.158503, -0.990461, -0.421985, 0.743145, 
0.857527, -0.240567, -0.998518, -0.344643, 0.796530, 0.811472, -0.320944, -0.999570, -0.264883, 0.844328, 
0.759725, -0.399069, -0.993611, -0.183265, 0.886204, 0.702650, -0.474396, -0.980683, -0.100362, 0.921863, 
0.640646, -0.546394, -0.960876, -0.016754, 0.951057, 0.574148, -0.614561, -0.934329, 0.066970, 0.973579, 
0.503623, -0.678416, -0.901228, 0.150226, 0.989272, 0.429566, -0.737513, -0.861806, 0.232427, 0.998027, 
0.352495, -0.791437, -0.816339, 0.312998, 0.999781, 0.272952, -0.839809, -0.765146, 0.391374, 0.994522, 
0.191494, -0.882291, -0.708586, 0.467004, 0.982287, 0.108693, -0.918584, -0.647056, 0.539359, 0.963163, 
0.025130, -0.948434, -0.580987, 0.607930, 0.937282, -0.058609, -0.971632, -0.510843, 0.672238, 0.904827, 
-0.141938, -0.988014, -0.437116, 0.731830, 0.866025, -0.224271, -0.997466, -0.360322, 0.786288, 0.821149, 
-0.305031, -0.999921, -0.281002, 0.835232, 0.770513, -0.383651, -0.995363, -0.199710, 0.878317, 0.714473, 
-0.459580, -0.983823, -0.117017, 0.915241, 0.653421, -0.532285, -0.965382, -0.033504, 0.945746, 0.587785, 
-0.601257, -0.940169, 0.050244, 0.969616, 0.518027, -0.666012, -0.908362, 0.133640, 0.986686, 0.444635, 
-0.726095, -0.870184, 0.216099, 0.996835, 0.368125, -0.781085, -0.825902, 0.297042, 0.999991, 0.289032, 
-0.830596, -0.775826, 0.375901, 0.996134, 0.207912, -0.874281, -0.720309, 0.452123, 0.985289, 0.125333, 
-0.911834, -0.659739, 0.525175, 0.967533, 0.041876, -0.942991, -0.594542, 0.594542, 0.942991, -0.041876, 
-0.967533, -0.525175, 0.659739, 0.911834, -0.125333, -0.985289, -0.452123, 0.720309, 0.874281, -0.207912, 
-0.996134, -0.375901, 0.775826, 0.830596, -0.289032, -0.999991, -0.297042, 0.825902, 0.781085, -0.368125, 
-0.996835, -0.216099, 0.870184, 0.726095, -0.444635, -0.986686, -0.133640, 0.908362, 0.666012, -0.518027, 
-0.969616, -0.050244, 0.940169, 0.601257, -0.587785, -0.945746, 0.033504, 0.965382, 0.532285, -0.653421, 
-0.915241, 0.117017, 0.983823, 0.459580, -0.714473, -0.878317, 0.199710, 0.995363, 0.383651, -0.770513, 
-0.835232, 0.281002, 0.999921, 0.305031, -0.821149, -0.786288, 0.360322, 0.997466, 0.224271, -0.866025, 
-0.731830, 0.437116, 0.988014, 0.141938, -0.904827, -0.672238, 0.510843, 0.971632, 0.058609, -0.937282, 
-0.607930, 0.580987, 0.948434, -0.025130, -0.963163, -0.539359, 0.647056, 0.918584, -0.108693, -0.982287, 
-0.467004, 0.708586, 0.882291, -0.191494, -0.994522, -0.391374, 0.765146, 0.839809, -0.272952, -0.999781, 
-0.312998, 0.816339, 0.791437, -0.352495, -0.998027, -0.232427, 0.861806, 0.737513, -0.429566, -0.989272, 
-0.150226, 0.901228, 0.678416, -0.503623, -0.973579, -0.066970, 0.934329, 0.614561, -0.574148, -0.951057, 
0.016754, 0.960876, 0.546394, -0.640646, -0.921863, 0.100362, 0.980683, 0.474396, -0.702650, -0.886204, 
0.183265, 0.993611, 0.399069, -0.759725, -0.844328, 0.264883, 0.999570, 0.320944, -0.811472, -0.796530, 
0.344643, 0.998518, 0.240567, -0.857527, -0.743145, 0.421985, 0.990461, 0.158503, -0.897566, -0.684547, 
0.496368, 0.975458, 0.075327, -0.931310, -0.621148, 0.567269, 0.953612, -0.008377, -0.958522, -0.553392, 
0.634191, 0.925077, -0.092023, -0.979010, -0.481754, 0.696664, 0.890054, -0.175023, -0.992631, -0.406737, 
0.754251, 0.848787, -0.256795, -0.999289, -0.328867, 0.806548, 0.801567, -0.336767, -0.998939, -0.248690, 
0.853187, 0.748724, -0.414376, -0.991581, -0.166769, 0.893841, 0.690630, -0.489078, -0.977268, -0.083678, 
0.928226, 0.627691, -0.560350, -0.956100, -0.000000, 0.956100, 0.560350, -0.627691, -0.928226, 0.083678, 
0.977268, 0.489078, -0.690630, -0.893841, 0.166769, 0.991581, 0.414376, -0.748724, -0.853187, 0.248690, 
0.998939, 0.336767, -0.801567, -0.806548, 0.328867, 0.999289, 0.256795, -0.848787, -0.754251, 0.406737, 
0.992631, 0.175023, -0.890054, -0.696664, 0.481754, 0.979010, 0.092023, -0.925077, -0.634191, 0.553392, 
0.958522, 0.008377, -0.953612, -0.567269, 0.621148, 0.931310, -0.075327, -0.975458, -0.496368, 0.684547, 
0.897566, -0.158503, -0.990461, -0.421985, 0.743145, 0.857527, -0.240567, -0.998518, -0.344643, 0.796530, 
0.811472, -0.320944, -0.999570, -0.264883, 0.844328, 0.759725, -0.399069, -0.993611, -0.183265, 0.886204, 
0.702650, -0.474396, -0.980683, -0.100362, 0.921863, 0.640646, -0.546394, -0.960876, -0.016754, 0.951057, 
0.574148, -0.614561, -0.934329, 0.066970, 0.973579, 0.503623, -0.678416, -0.901228, 0.150226, 0.989272, 
0.429566, -0.737513, -0.861806, 0.232427, 0.998027, 0.352495, -0.791437, -0.816339, 0.312998, 0.999781, 
0.272952, -0.839809, -0.765146, 0.391374, 0.994522, 0.191494, -0.882291, -0.708586, 0.467004, 0.982287, 
0.108693, -0.918584, -0.647056, 0.539359, 0.963163, 0.025130, -0.948434, -0.580987, 0.607930, 0.937282, 
-0.058609, -0.971632, -0.510843, 0.672238, 0.904827, -0.141938, -0.988014, -0.437116, 0.731830, 0.866025, 
-0.224271, -0.997466, -0.360322, 0.786288, 0.821149, -0.305031, -0.999921, -0.281002, 0.835232, 0.770513, 
0.000000};
	float32_t pState[PSTATE_LENGTH] = {0};

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
	
	
		
		
		float32_t max_v;
		uint32_t max_i;
		
		arm_abs_f32(piloto19k, suma, BUFFER_LENGTH/4);
		arm_max_f32(suma, BUFFER_LENGTH/4, &max_v, &max_i);
		arm_scale_f32(piloto19k, 1/2*max_v, piloto19k, BUFFER_LENGTH/4);
		
		arm_abs_f32(piloto38k, suma, BUFFER_LENGTH/4);
		arm_max_f32(suma, BUFFER_LENGTH/4, &max_v, &max_i);
		arm_scale_f32(piloto38k, 1/2*max_v, piloto38k, BUFFER_LENGTH/4);
	
	
	
	
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
  RCC_OscInitStruct.PLL.PLLM = 8;
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
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 192;
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
  hi2s3.Init.AudioFreq = I2S_AUDIOFREQ_192K;
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
