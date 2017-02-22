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
	q31_t canal_L[BUFFER_LENGTH/4] = {0};
	q31_t canal_R[BUFFER_LENGTH/4] = {0};
	q31_t canal_L_filtrado[BUFFER_LENGTH/4] = {0};
	q31_t canal_R_filtrado[BUFFER_LENGTH/4] = {0};
	q31_t suma[BUFFER_LENGTH/4] = {0};
	q31_t resta[BUFFER_LENGTH/4] = {0};
	q31_t mpx[BUFFER_LENGTH/4] = {0};
	q31_t lowpassfilter[FILTER_LENGTH] = {0xffaf2f98,0xffdcbf33,0xfff06542,0x001babeb,0x005ba972,0x00a65aa3,0x00ebdabf,0x01199df6,0x011f7503,0x00f4d858,0x009cdbc5,0x002728ad,
		0xffaddd2c,0xff4f81ad,0xff26967a,0xff41e1d6,0xff9dffdc,0x00249eca,0x00b07c6b,0x0116c747,0x013338eb,0x00f3d24b,0x00608144,0xff9bf82c,0xfedc9703,0xfe5e58f4,0xfe517067,
		0xfeca0dd9,0xffb64fda,0x00de660f,0x01efa04f,0x0291312c,0x027d7b3d,0x0199995a,0x00043e8f,0xfe16e41f,0xfc575332,0xfb5bb4b7,0xfba7227f,0xfd86e787,0x00f97da5,0x05a611c2,
		0x0ae7c947,0x0feb7159,0x13d9343a,0x16014e83,0x16014e83,0x13d9343a,0x0feb7159,0x0ae7c947,0x05a611c2,0x00f97da5,0xfd86e787,0xfba7227f,0xfb5bb4b7,0xfc575332,0xfe16e41f,
		0x00043e8f,0x0199995a,0x027d7b3d,0x0291312c,0x01efa04f,0x00de660f,0xffb64fda,0xfeca0dd9,0xfe517067,0xfe5e58f4,0xfedc9703,0xff9bf82c,0x00608144,0x00f3d24b,0x013338eb,
		0x0116c747,0x00b07c6b,0x00249eca,0xff9dffdc,0xff41e1d6,0xff26967a,0xff4f81ad,0xffaddd2c,0x002728ad,0x009cdbc5,0x00f4d858,0x011f7503,0x01199df6,0x00ebdabf,0x00a65aa3,
		0x005ba972,0x001babeb,0xfff06542,0xffdcbf33,0xffaf2f98};
	q31_t piloto19k[BUFFER_LENGTH/4] = {0x00000000,0x4c19f50d,0x7a617f8b,0x78b3e9f5,0x47b98bc3,0xfaa3d18f,0xafa7cf3a,0x8427e14c,0x892fe120,0xbcc713e6,0x0ab5f4a1,0x5472571a,
		0x7d171f34,0x74b6f796,0x3e9a1bbb,0xeff514a3,0xa7996f9c,0x81e20e3e,0x8d969aae,0xc620d227,0x1558ada5,0x5c33160d,0x7eec2059,0x6fe870df,0x350a4251,0xe5632654,0xa029ccb4,
		0x807eb1a4,0x92cac5eb,0xcfe27b5f,0x1fd511fb,0x634e45f9,0x7fdd38d0,0x6a50f768,0x2b1b2b05,0xdb01018f,0x996640c8,0x80004998,0x98c30ab7,0xd9fa8a9d,0x2a184d63,0x69b7243d,
		0x7fe8b7ab,0x63fa95a2,0x20deac5b,0xd0e14b6f,0x935af11b,0x8067b918,0x9f74b0d6,0xe456dfcd,0x340ff243,0x6f622e68,0x7f0e8845,0x5cf0acca,0x166727d6,0xc71631c8,0x8e12b81e,
		0x81b44666,0xa6d3b326,0xeee4e042,0x3daa1aba,0x744536ea,0x7d503269,0x553fe07b,0x0bc768f2,0xbdb14a7d,0x899711f2,0x83e39c5c,0xaed2d53d,0xf991981d,0x46d588c7,0x78577756,
		0x7ab0d78d,0x4cf60000,0x0112836b,0xb4c373ef,0x85f00b5e,0x86f1ce9b,0xb763bb28,0x0449dc59,0x4f81c53e,0x7b91a024,0x77352d36,0x4421ed80,0xf65bb0f5,0xac5cb6b2,0x8324335f,
		0x8ad96094,0xc0770337,0x0efa6d33,0x579f3d55,0x7dede5d6,0x72e37485,0x3ad3834c,0xebb62ebc,0xa48c28c4,0x81388f5d,0x8f934f66,0xc9fc6198,0x199018be,0x5f1f5ea1,0x7f680b5a,
		0x6dc36ef8,0x311b7764,0xe1351ad1,0x9d5fd273,0x8030922a,0x95171e6f,0xd3e2bd9f,0x23f7dd56,0x65f4b13f,0x7ffd69ac,0x67de5081,0x270b3d74,0xd6eb51df,0x96e4952d,0x800e15ce,
		0x9b5ae692,0xde185075,0x2e1f0bc6,0x6c12effe,0x7facf495,0x613eaefa,0x1cb4e780,0xcceb4d43,0x9126145f,0x80d15837,0xa25367f6,0xe88ac506,0x37f368d2,0x716f1e70,0x7e773c8d,
		0x59f06f29,0x122b0572,0xc34701e8,0x8c2ea08d,0x8278fac5,0xa9f41e41,0xf32758eb,0x41634df2,0x75ff9c9c,0x7c5e6dbb,0x5200af61,0x078083c2,0xba0fc004,0x880724cf,0x850204c5,
		0xb22f570b,0xfddafe1a,0x4a5dc90a,0x79bc384e,0x79664c0b,0x497daff5,0xfcc8897b,0xb15613ff,0x84b716ca,0x8867e8c3,0xbaf64a7d,0x08927d1d,0x52d2bad1,0x7c9e3bc6,0x75942c6c,
		0x4076b99f,0xf21655dd,0xa929a8c2,0x82446950,0x8ca48cb9,0xc43935d6,0x133a9795,0x5ab2f3c9,0x7ea079cf,0x70eeeb3e,0x36fc0211,0xe77d1dc6,0xa1992b91,0x80b381ad,0x91b05503,
		0xcde777bb,0x1dc02ac4,0x61f04f80,0x7fbf5700,0x6b7edfff,0x2d1e8ed8,0xdd0fe936,0x9ab231b0,0x80072fc3,0x97823208,0xd7efae13,0x281051ee,0x687dcdf9,0x7ff8d03e,0x654dce51,
		0x22f016cb,0xd2e17129,0x94812002,0x8040a901,0x9e0fb081,0xe23fd53d,0x32188846,0x6e4faafe,0x7f4c7e54,0x5e66d470,0x1882e23b,0xc903fdf0,0x8f1114c3,0x815f8632,0xa54d0c38,
		0xecc5686c,0x3bc6ca2b,0x735b7348,0x7dbb96b1,0x56d6573f,0x0de9aa24,0xbf894662,0x8a6bd395,0x8361c43b,0xad2d4530,0xf76d82e4,0x4509b584,0x7798173e,0x7b48e937,0x4ea9ec02,
		0x03377686,0xb682500c,0x8699b3f6,0x8643c7b3,0xb5a236f7,0x022501e7,0x4dd0a8f6,0x7afdfb3c,0x77f8db32,0x45f03ffd,0xf87f7c3f,0xadff50a0,0x83a19246,0x8a006365,0xbe9cb20f,
		0x0cd8a716,0x560be1c0,0x7d87053c,0x73d15f74,0x3cb8fe19,0xedd4fa8f,0xa60f90d8,0x8188c374,0x8e90e191,0xc80c972f,0x17753afb,0x5dac980b,0x7f2ea7ca,0x6ed9eba2,0x3314b2be,
		0xe34b1881,0x9ec15107,0x80530b6c,0x93ed1003,0xd1e0f43b,0x21e7af8c,0x64a5196f,0x7ff1ea33,0x691b6ad4,0x2914ae22,0xd8f4c28d,0x9821af80,0x80029655,0x9a0b4ec2,0xdc0822ab,
		0x2c1d4262,0x6ae8e192,0x7fcf6dd7,0x62a02d8e,0x1ecae530,0xcee4889d,0x923c9109,0x8097f4a7,0xa0e0a160,0xe66fe743,0x36039e69,0x706cb09b,0x7ec770a4,0x5b73d73d,0x1449d145,
		0xc52c7cb5,0x8d1c8b7c,0x82121a2b,0xa860c2ac,0xf10592ce,0x3f88fcca,0x75269f6d,0x7cdbcca2,0x53a3494f,0x09a44f0c,0xbbde1281,0x88cad2cb,0x846e5fdd,0xb07e3ac3,0xfbb623a8,
		0x489c44d9,0x790e3166,0x7a0ff4a3,0x4b3c8c12,0xfeed7c96,0xb30a0001,0x854f2874,0x87a888ab,0xb92a773a,0x066e67e4,0x512d2ac4,0x7c1c63a5,0x7668ee0f,0x424eb584,0xf438970f,
		0xaac01f86,0x82afcd98,0x8bbac917,0xc255e547,0x111b1fbf,0x592c4cdb,0x7e4bb99b,0x71ed47e3,0x38e9ce39,0xe998d82b,0xa30f5337,0x80f177bc,0x909dd199,0xcbf00dbe,0x1ba92034,
		0x608b4f2b,0x7f9846e9,0x6ca50ee6,0x2f1eb492,0xdf2153a6,0x9c056a5f,0x80174856,0x9648dbc4,0xd5e7b29e,0x26057564,0x673cf54a,0x7fffb669,0x6699bf39,0x24fefe72,0xd4e4d4fc,
		0x95af0899,0x8022c731,0x9cb1ba08,0xe02aee06,0x301d84a2,0x6d353a16,0x7f814e5d,0x5fd6334d,0x1a9cd9ad,0xcaf5bdb0,0x90178f22,0x8113dfa8,0xa3cce9f4,0xeaa7525c,0x39df2dda,
		0x72696553,0x7e1df1c3,0x58669065,0x100aeb5e,0xc165e446,0x8b49086b,0x82e8e0cd,0xab8da8e7,0xf54a0b60,0x4338ec1b,0x76d01ee1,0x7bd81eb5,0x505830c7,0x055c2e72,0xb846743e,
		0x874c160c,0x859e8076,0xb3e60af4,0x00000000,0x4c19f50d,0x7a617f8b,0x78b3e9f5,0x47b98bc3,0xfaa3d18f,0xafa7cf3a,0x8427e14c,0x892fe120,0xbcc713e6,0x0ab5f4a1,0x5472571a,
		0x7d171f34,0x74b6f796,0x3e9a1bbb,0xeff514a3,0xa7996f9c,0x81e20e3e,0x8d969aae,0xc620d227,0x1558ada5,0x5c33160d,0x7eec2059,0x6fe870df,0x350a4251,0xe5632654,0xa029ccb4,
		0x807eb1a4,0x92cac5eb,0xcfe27b5f,0x1fd511fb,0x634e45f9,0x7fdd38d0,0x6a50f768,0x2b1b2b05,0xdb01018f,0x996640c8,0x80004998,0x98c30ab7,0xd9fa8a9d,0x2a184d63,0x69b7243d,
		0x7fe8b7ab,0x63fa95a2,0x20deac5b,0xd0e14b6f,0x935af11b,0x8067b918,0x9f74b0d6,0xe456dfcd,0x340ff243,0x6f622e68,0x7f0e8845,0x5cf0acca,0x166727d6,0xc71631c8,0x8e12b81e,
		0x81b44666,0xa6d3b326,0xeee4e042,0x3daa1aba,0x744536ea,0x7d503269,0x553fe07b,0x0bc768f2,0xbdb14a7d,0x899711f2,0x83e39c5c,0xaed2d53d,0xf991981d,0x46d588c7,0x78577756,
		0x7ab0d78d,0x4cf60000,0x0112836b,0xb4c373ef,0x85f00b5e,0x86f1ce9b,0xb763bb28,0x0449dc59,0x4f81c53e,0x7b91a024,0x77352d36,0x4421ed80,0xf65bb0f5,0xac5cb6b2,0x8324335f,
		0x8ad96094,0xc0770337,0x0efa6d33,0x579f3d55,0x7dede5d6,0x72e37485,0x3ad3834c,0xebb62ebc,0xa48c28c4,0x81388f5d,0x8f934f66,0xc9fc6198,0x199018be,0x5f1f5ea1,0x7f680b5a,
		0x6dc36ef8,0x311b7764,0xe1351ad1,0x9d5fd273,0x8030922a,0x95171e6f,0xd3e2bd9f,0x23f7dd56,0x65f4b13f,0x7ffd69ac,0x67de5081,0x270b3d74,0xd6eb51df,0x96e4952d,0x800e15ce,
		0x9b5ae692,0xde185075,0x2e1f0bc6,0x6c12effe,0x7facf495,0x613eaefa,0x1cb4e780,0xcceb4d43,0x9126145f,0x80d15837,0xa25367f6,0xe88ac506,0x37f368d2,0x716f1e70,0x7e773c8d,
		0x59f06f29,0x122b0572,0x00000000,0x00000000,0x00000000};
	q31_t piloto38k[BUFFER_LENGTH/4] = {0x00000000,0x7a617f8b,0x47b98bc3,0xafa7cf3a,0x892fe120,0x0ab5f4a1,0x7d171f34,0x3e9a1bbb,0xa7996f9c,0x8d969aae,0x1558ada5,0x7eec2059,
		0x350a4251,0xa029ccb4,0x92cac5eb,0x1fd511fb,0x7fdd38d0,0x2b1b2b05,0x996640c8,0x98c30ab7,0x2a184d63,0x7fe8b7ab,0x20deac5b,0x935af11b,0x9f74b0d6,0x340ff243,0x7f0e8845,
		0x166727d6,0x8e12b81e,0xa6d3b326,0x3daa1aba,0x7d503269,0x0bc768f2,0x899711f2,0xaed2d53d,0x46d588c7,0x7ab0d78d,0x0112836b,0x85f00b5e,0xb763bb28,0x4f81c53e,0x77352d36,
		0xf65bb0f5,0x8324335f,0xc0770337,0x579f3d55,0x72e37485,0xebb62ebc,0x81388f5d,0xc9fc6198,0x5f1f5ea1,0x6dc36ef8,0xe1351ad1,0x8030922a,0xd3e2bd9f,0x65f4b13f,0x67de5081,
		0xd6eb51df,0x800e15ce,0xde185075,0x6c12effe,0x613eaefa,0xcceb4d43,0x80d15837,0xe88ac506,0x716f1e70,0x59f06f29,0xc34701e8,0x8278fac5,0xf32758eb,0x75ff9c9c,0x5200af61,
		0xba0fc004,0x850204c5,0xfddafe1a,0x79bc384e,0x497daff5,0xb15613ff,0x8867e8c3,0x08927d1d,0x7c9e3bc6,0x4076b99f,0xa929a8c2,0x8ca48cb9,0x133a9795,0x7ea079cf,0x36fc0211,
		0xa1992b91,0x91b05503,0x1dc02ac4,0x7fbf5700,0x2d1e8ed8,0x9ab231b0,0x97823208,0x281051ee,0x7ff8d03e,0x22f016cb,0x94812002,0x9e0fb081,0x32188846,0x7f4c7e54,0x1882e23b,
		0x8f1114c3,0xa54d0c38,0x3bc6ca2b,0x7dbb96b1,0x0de9aa24,0x8a6bd395,0xad2d4530,0x4509b584,0x7b48e937,0x03377686,0x8699b3f6,0xb5a236f7,0x4dd0a8f6,0x77f8db32,0xf87f7c3f,
		0x83a19246,0xbe9cb20f,0x560be1c0,0x73d15f74,0xedd4fa8f,0x8188c374,0xc80c972f,0x5dac980b,0x6ed9eba2,0xe34b1881,0x80530b6c,0xd1e0f43b,0x64a5196f,0x691b6ad4,0xd8f4c28d,
		0x80029655,0xdc0822ab,0x6ae8e192,0x62a02d8e,0xcee4889d,0x8097f4a7,0xe66fe743,0x706cb09b,0x5b73d73d,0xc52c7cb5,0x82121a2b,0xf10592ce,0x75269f6d,0x53a3494f,0xbbde1281,
		0x846e5fdd,0xfbb623a8,0x790e3166,0x4b3c8c12,0xb30a0001,0x87a888ab,0x066e67e4,0x7c1c63a5,0x424eb584,0xaac01f86,0x8bbac917,0x111b1fbf,0x7e4bb99b,0x38e9ce39,0xa30f5337,
		0x909dd199,0x1ba92034,0x7f9846e9,0x2f1eb492,0x9c056a5f,0x9648dbc4,0x26057564,0x7fffb669,0x24fefe72,0x95af0899,0x9cb1ba08,0x301d84a2,0x7f814e5d,0x1a9cd9ad,0x90178f22,
		0xa3cce9f4,0x39df2dda,0x7e1df1c3,0x100aeb5e,0x8b49086b,0xab8da8e7,0x4338ec1b,0x7bd81eb5,0x055c2e72,0x874c160c,0xb3e60af4,0x4c19f50d,0x78b3e9f5,0xfaa3d18f,0x8427e14c,
		0xbcc713e6,0x5472571a,0x74b6f796,0xeff514a3,0x81e20e3e,0xc620d227,0x5c33160d,0x6fe870df,0xe5632654,0x807eb1a4,0xcfe27b5f,0x634e45f9,0x6a50f768,0xdb01018f,0x80004998,
		0xd9fa8a9d,0x69b7243d,0x63fa95a2,0xd0e14b6f,0x8067b918,0xe456dfcd,0x6f622e68,0x5cf0acca,0xc71631c8,0x81b44666,0xeee4e042,0x744536ea,0x553fe07b,0xbdb14a7d,0x83e39c5c,
		0xf991981d,0x78577756,0x4cf60000,0xb4c373ef,0x86f1ce9b,0x0449dc59,0x7b91a024,0x4421ed80,0xac5cb6b2,0x8ad96094,0x0efa6d33,0x7dede5d6,0x3ad3834c,0xa48c28c4,0x8f934f66,
		0x199018be,0x7f680b5a,0x311b7764,0x9d5fd273,0x95171e6f,0x23f7dd56,0x7ffd69ac,0x270b3d74,0x96e4952d,0x9b5ae692,0x2e1f0bc6,0x7facf495,0x1cb4e780,0x9126145f,0xa25367f6,
		0x37f368d2,0x7e773c8d,0x122b0572,0x8c2ea08d,0xa9f41e41,0x41634df2,0x7c5e6dbb,0x078083c2,0x880724cf,0xb22f570b,0x4a5dc90a,0x79664c0b,0xfcc8897b,0x84b716ca,0xbaf64a7d,
		0x52d2bad1,0x75942c6c,0xf21655dd,0x82446950,0xc43935d6,0x5ab2f3c9,0x70eeeb3e,0xe77d1dc6,0x80b381ad,0xcde777bb,0x61f04f80,0x6b7edfff,0xdd0fe936,0x80072fc3,0xd7efae13,
		0x687dcdf9,0x654dce51,0xd2e17129,0x8040a901,0xe23fd53d,0x6e4faafe,0x5e66d470,0xc903fdf0,0x815f8632,0xecc5686c,0x735b7348,0x56d6573f,0xbf894662,0x8361c43b,0xf76d82e4,
		0x7798173e,0x4ea9ec02,0xb682500c,0x8643c7b3,0x022501e7,0x7afdfb3c,0x45f03ffd,0xadff50a0,0x8a006365,0x0cd8a716,0x7d87053c,0x3cb8fe19,0xa60f90d8,0x8e90e191,0x17753afb,
		0x7f2ea7ca,0x3314b2be,0x9ec15107,0x93ed1003,0x21e7af8c,0x7ff1ea33,0x2914ae22,0x9821af80,0x9a0b4ec2,0x2c1d4262,0x7fcf6dd7,0x1ecae530,0x923c9109,0xa0e0a160,0x36039e69,
		0x7ec770a4,0x1449d145,0x8d1c8b7c,0xa860c2ac,0x3f88fcca,0x7cdbcca2,0x09a44f0c,0x88cad2cb,0xb07e3ac3,0x489c44d9,0x7a0ff4a3,0xfeed7c96,0x854f2874,0xb92a773a,0x512d2ac4,
		0x7668ee0f,0xf438970f,0x82afcd98,0xc255e547,0x592c4cdb,0x71ed47e3,0xe998d82b,0x80f177bc,0xcbf00dbe,0x608b4f2b,0x6ca50ee6,0xdf2153a6,0x80174856,0xd5e7b29e,0x673cf54a,
		0x6699bf39,0xd4e4d4fc,0x8022c731,0xe02aee06,0x6d353a16,0x5fd6334d,0xcaf5bdb0,0x8113dfa8,0xeaa7525c,0x72696553,0x58669065,0xc165e446,0x82e8e0cd,0xf54a0b60,0x76d01ee1,
		0x505830c7,0xb846743e,0x859e8076,0x00000000,0x7a617f8b,0x47b98bc3,0xafa7cf3a,0x892fe120,0x0ab5f4a1,0x7d171f34,0x3e9a1bbb,0xa7996f9c,0x8d969aae,0x1558ada5,0x7eec2059,
		0x350a4251,0xa029ccb4,0x92cac5eb,0x1fd511fb,0x7fdd38d0,0x2b1b2b05,0x996640c8,0x98c30ab7,0x2a184d63,0x7fe8b7ab,0x20deac5b,0x935af11b,0x9f74b0d6,0x340ff243,0x7f0e8845,
		0x166727d6,0x8e12b81e,0xa6d3b326,0x3daa1aba,0x7d503269,0x0bc768f2,0x899711f2,0xaed2d53d,0x46d588c7,0x7ab0d78d,0x0112836b,0x85f00b5e,0xb763bb28,0x4f81c53e,0x77352d36,
		0xf65bb0f5,0x8324335f,0xc0770337,0x579f3d55,0x72e37485,0xebb62ebc,0x81388f5d,0xc9fc6198,0x5f1f5ea1,0x6dc36ef8,0xe1351ad1,0x8030922a,0xd3e2bd9f,0x65f4b13f,0x67de5081,
		0xd6eb51df,0x800e15ce,0xde185075,0x6c12effe,0x613eaefa,0xcceb4d43,0x80d15837,0xe88ac506,0x716f1e70,0x59f06f29,0xc34701e8,0x8278fac5,0xf32758eb,0x75ff9c9c,0x5200af61,
		0xba0fc004,0x850204c5,0xfddafe1a,0x79bc384e,0x497daff5,0xb15613ff,0x8867e8c3,0x08927d1d,0x7c9e3bc6,0x4076b99f,0xa929a8c2,0x8ca48cb9,0x133a9795,0x7ea079cf,0x36fc0211,
		0xa1992b91,0x91b05503,0x1dc02ac4,0x7fbf5700,0x2d1e8ed8,0x9ab231b0,0x97823208,0x281051ee,0x7ff8d03e,0x22f016cb,0x94812002,0x9e0fb081,0x32188846,0x7f4c7e54,0x1882e23b,
		0x8f1114c3,0xa54d0c38,0x3bc6ca2b,0x7dbb96b1,0x0de9aa24,0x8a6bd395,0xad2d4530,0x4509b584,0x7b48e937,0x03377686,0x8699b3f6,0xb5a236f7,0x4dd0a8f6,0x77f8db32,0xf87f7c3f,
		0x83a19246,0xbe9cb20f,0x560be1c0,0x73d15f74,0xedd4fa8f,0x8188c374,0xc80c972f,0x5dac980b,0x6ed9eba2,0xe34b1881,0x80530b6c,0xd1e0f43b,0x64a5196f,0x691b6ad4,0xd8f4c28d,
		0x80029655,0xdc0822ab,0x6ae8e192,0x62a02d8e,0x00000000};
	q31_t pState[PSTATE_LENGTH] = {0};

	// Armado de los canales
	volatile uint8_t CANAL = CH_L_1;

	// Filtro digital
	arm_fir_instance_q31 lowpass = {0};


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
