/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
    READY1,
    TRANS1,
	RECEIVE1,
    READY2,
	TRANS2,
	RECEIVE2,
	DONE_
} STATE_TX;

typedef enum {
    BEGIN,
    CMD,
	END
} STATE_V1;
STATE_V1 state_v1 = BEGIN;


typedef enum {
  CK_77,
  CK_0D,
  CK_00,
  CK_84,
  READ_X1,
  READ_X2,
  READ_X3,
  READ_Y1,
  READ_Y2,
  READ_Y3,
  READ_Z1,
  READ_Z2,
  READ_Z3,
  CHECKSUM
} IMU_STATE_t;

// PC IOmemoryMap
enum{
	// # float variables = 32
	HoldingReg00_L, // float f0
	HoldingReg00_H,
	HoldingReg01_L, // float f1
	HoldingReg01_H,
	HoldingReg02_L, // float f2
	HoldingReg02_H,
	HoldingReg03_L, // float f3
	HoldingReg03_H,
	HoldingReg04_L, // float f4
	HoldingReg04_H,
	HoldingReg05_L, // float f5
	HoldingReg05_H,
	HoldingReg06_L, // float f6
	HoldingReg06_H,
	HoldingReg07_L, // float f7
	HoldingReg07_H,
	HoldingReg08_L, // float f8
	HoldingReg08_H,
	HoldingReg09_L,	// float f9
	HoldingReg09_H,
	HoldingReg10_L, // float f10
	HoldingReg10_H,
	HoldingReg11_L, // float f11
	HoldingReg11_H,
	HoldingReg12_L, // float f12
	HoldingReg12_H,
	HoldingReg13_L, // float f13
	HoldingReg13_H,
	HoldingReg14_L, // float f14
	HoldingReg14_H,
	HoldingReg15_L, // float f15
	HoldingReg15_H,
	HoldingReg16_L, // float f16
	HoldingReg16_H,
	HoldingReg17_L, // float f17
	HoldingReg17_H,
	HoldingReg18_L, // float f18
	HoldingReg18_H,
	HoldingReg19_L, // float f19
	HoldingReg19_H,
	HoldingReg20_L, // float f20
	HoldingReg20_H,
	HoldingReg21_L, // float f21
	HoldingReg21_H,
	HoldingReg22_L, // float f22
	HoldingReg22_H,
	HoldingReg23_L, // float f23
	HoldingReg23_H,
	HoldingReg24_L, // float f24
	HoldingReg24_H,
	HoldingReg25_L, // float f25
	HoldingReg25_H,
	HoldingReg26_L, // float f26
	HoldingReg26_H,
	HoldingReg27_L, // float f27
	HoldingReg27_H,

	IMUAngle_L,		// float f28
	IMUAngle_H,


	KpCoeffReg_L, // float f29
	KpCoeffReg_H,

	KdCoeffReg_L, // float f30
	KdCoeffReg_H,

	KiCoeffReg_L, // float f31
	KiCoeffReg_H,

	DACPrecisionAzimuthReg_L,	// float32
	DACPrecisionAzimuthReg_H,

	DACPrecisionElevationReg_L,	// float33
	DACPrecisionElevationReg_H,

	DACCoarseAzimuthReg_L,	// float 34
	DACCoarseAzimuthReg_H,

	DACCoarseElevationReg_L,	// float 35
	DACCoarseElevationReg_H,

	// # uint16_t variables = 16
	HoldingReg32,  // int16_t i16_0
	HoldingReg33,  // int16_t i16_1
	HoldingReg34,  // int16_t i16_2
	HoldingReg35,  // int16_t i16_3
	HoldingReg36,  // int16_t i16_4
	HoldingReg37,  // int16_t i16_6
	HoldingReg38,  // int16_t i16_7
	HoldingReg39,  // int16_t i16_8
	HoldingReg40,  // int16_t i16_9
	HoldingReg41,  // int16_t i16_10
	HoldingReg42,  // int16_t i16_11
	HoldingReg43,  // int16_t i16_12
	HoldingReg44,  // int16_t i16_13
	HoldingReg45,  // int16_t i16_14
	HoldingReg46,  // int16_t i16_15

	PrecisionValveRelay,
	CoarseValveRelay,

	ClutchRelay,

	ButtonManual,
	ButtonAzimuthUp,
	ButtonAzimuthDown,
	ButtonElevationUp,
	ButtonElevationDown,

	HoldingRegSize
};

typedef struct {
	uint16_t HoldingReg00_L;	// float 0
	uint16_t HoldingReg00_H;
	uint16_t HoldingReg01_L;	// float 1
	uint16_t HoldingReg01_H;
	uint16_t HoldingReg02_L;	// float 2
	uint16_t HoldingReg02_H;
	uint16_t HoldingReg03_L;	// float 3
	uint16_t HoldingReg03_H;
	uint16_t HoldingReg04_L;	// float 4
	uint16_t HoldingReg04_H;
	uint16_t HoldingReg05_L;	// float 5
	uint16_t HoldingReg05_H;
	uint16_t HoldingReg06_L;	// float 6
	uint16_t HoldingReg06_H;
	uint16_t HoldingReg07_L;	// float 7
	uint16_t HoldingReg07_H;
	uint16_t HoldingReg08_L;	// float 8
	uint16_t HoldingReg08_H;
	uint16_t HoldingReg09_L;	// float 9
	uint16_t HoldingReg09_H;
	uint16_t HoldingReg10_L;	// float 10
	uint16_t HoldingReg10_H;
	uint16_t HoldingReg11_L;	// float 11
	uint16_t HoldingReg11_H;
	uint16_t HoldingReg12_L;	// float 12
	uint16_t HoldingReg12_H;
	uint16_t HoldingReg13_L;	// float 13
	uint16_t HoldingReg13_H;
	uint16_t HoldingReg14_L;	// float 14
	uint16_t HoldingReg14_H;
	uint16_t HoldingReg15_L;	// float 15
	uint16_t HoldingReg15_H;
	uint16_t HoldingReg16_L;	// float 16
	uint16_t HoldingReg16_H;
	uint16_t HoldingReg17_L;	// float 17
	uint16_t HoldingReg17_H;
	uint16_t HoldingReg18_L;	// float 18
	uint16_t HoldingReg18_H;
	uint16_t HoldingReg19_L;	// float 19
	uint16_t HoldingReg19_H;
	uint16_t HoldingReg20_L;	// float 20
	uint16_t HoldingReg20_H;
	uint16_t HoldingReg21_L;	// float 21
	uint16_t HoldingReg21_H;
	uint16_t HoldingReg22_L;	// float 22
	uint16_t HoldingReg22_H;
	uint16_t HoldingReg23_L;	// float 23
	uint16_t HoldingReg23_H;
	uint16_t HoldingReg24_L;	// float 24
	uint16_t HoldingReg24_H;
	uint16_t HoldingReg25_L;	// float 25
	uint16_t HoldingReg25_H;
	uint16_t HoldingReg26_L;	// float 26
	uint16_t HoldingReg26_H;
	uint16_t HoldingReg27_L;	// float 27
	uint16_t HoldingReg27_H;

	float IMUAngle;	// float 28

	float KpCoeffReg;	// float 29
	float KiCoeffReg;	// float 30
	float KdCoeffReg;	// float 31

	float DACPrecisionAzimuthReg;	// float 32
	float DACPrecisionElevationReg;	// float 33
	float DACCoarseAzimuthReg;	// float 34
	float DACCoarseElevationReg;	// float 35

	uint16_t HoldingReg32;
	uint16_t HoldingReg33;
	uint16_t HoldingReg34;
	uint16_t HoldingReg35;
	uint16_t HoldingReg36;
	uint16_t HoldingReg37;
	uint16_t HoldingReg38;
	uint16_t HoldingReg39;
	uint16_t HoldingReg40;
	uint16_t HoldingReg41;
	uint16_t HoldingReg42;
	uint16_t HoldingReg43;
	uint16_t HoldingReg44;
	uint16_t HoldingReg45;
	uint16_t HoldingReg46;

	uint16_t PercisionValveRelay;
	uint16_t CoarseValveRelay;
	uint16_t ClutchRelay;
	uint16_t ButtonManual;
	uint16_t ButtonAzimuthUp;
	uint16_t ButtonAzimuthDown;
	uint16_t ButtonElevationUp;
	uint16_t ButtonElevationDown;
} IO_MemoryMap_Struct_t;

typedef union {
	uint16_t Array[HoldingRegSize];
	IO_MemoryMap_Struct_t Named;
} IO_MemoryMap_Union_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define IMU_query_len 5
#define BoardID  45
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
// #define _RET(fval,idx) fval = (float*)IO_MemoryMap[idx])
// #define _WRT(idx, fval) {IO_MemoryMap[idx]= (uint16_t*)fval[0];IO_MemoryMap[idx]= (uint16_t*)fval[1]}
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi3;

TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim7;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
IO_MemoryMap_Union_t IO_MemoryMap_Data;
//uint16_t IO_MemoryMap[HoldingRegSize];
#define BUFFER_SIZE 128
uint8_t ModbusFrame[BUFFER_SIZE];
uint8_t frame[BUFFER_SIZE];
uint8_t idx;
uint16_t bufferIdx = 0;
float f0 = 3.14;
float f1;
uint8_t us_data;
uint8_t cmd;
bool cmd_flag;
uint8_t ack = 0x24;
uint8_t res[6] = {0x42, 0x20, 0x00, 0x00, 0xff, 0x45};
/* Khai báo mảng 8 byte */
uint8_t frame_8byte[8] = {0xAA, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x16};

// const uint8_t IMU_query[IMU_query_len] = {0x77, 0x04, 0x00, 0x04, 0x08};
uint8_t IMU_RxData;


uint32_t cnt;
uint8_t csm;
int32_t Ang_curr;
int32_t Ang_old;
int32_t Denta_Ang;
int32_t IMU_Angle;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI3_Init(void);
static void MX_TIM4_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM7_Init(void);
/* USER CODE BEGIN PFP */
void DAC7716_Write(uint8_t ID, uint16_t Data12bit);
void DAC7716_SetVol(float Voltage, uint8_t ID);

// Utility
void  _RET(float* fval, uint16_t idx);
void _WRT(uint16_t idx, float* fval);

// Ouput_Relay
void Relay_Update(uint16_t *pMap);
void Relay_init(void);

//Btn
void Btn_Update(uint16_t *pMap);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void DAC7716_Write(uint8_t ID, uint16_t Data12bit){
	uint8_t S_Data[3];

	S_Data[0] = ID;
	S_Data[1] = (uint8_t)(Data12bit>>4); // Lay 8 bit cao
	S_Data[2] = (uint8_t)(Data12bit);    // Lay 4 bit thap dich trai

	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3,S_Data,3,100);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);
}

void DAC7716_SetVol(float Voltage, uint8_t DACchannel){

	int16_t u16Val;
	int16_t i16Val;

	/* Saturated Value*/
	if(Voltage > 10.0) Voltage = 10.0;
	if(Voltage <-10.0) Voltage = -10.0;

	// Code = (Vout+10)*(4095/20)
	//Code = (uint16_t)((Voltage + 10)*(204.75));

	/* Normalised to i16*/
	// Code = Voltage/(0.5*4*5)*2047;
	i16Val = (int16_t) (Voltage/(10.0)*2047);

	/* Convert int16_t to uint16_t*/
	if(i16Val<0) {
		u16Val = i16Val + 0x8000;
	}else {
		u16Val = i16Val;
	}

	/* Write to DAC7716*/
	DAC7716_Write(DACchannel, u16Val);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){     // every 100ms

	const uint8_t IMU_query[IMU_query_len] = {0x77, 0x04, 0x00, 0x04, 0x08};
	if (htim->Instance == TIM4){
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1);
		HAL_UART_Transmit_IT(&huart1, IMU_query, 5);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);
//
//	    Denta_Ang = Ang_curr - Ang_old;
//	    Ang_old = Ang_curr;
//	    if (Denta_Ang >= -36000 && Denta_Ang < -18000){
//	      IMU_Angle += (Denta_Ang + 36000);
//	    }
//	    else if (Denta_Ang >= -18000 && Denta_Ang <= 18000){
//	    	IMU_Angle += Denta_Ang;
//	    }
//	    else if (Denta_Ang > 18000 && Denta_Ang <= 36000){
//	    	IMU_Angle -= (36000 - Denta_Ang);
//	    }
//	    IO_MemoryMap_Data.Named.IMUAngle = IMU_Angle;
		IO_MemoryMap_Data.Named.IMUAngle += 1;

	}
	if (htim->Instance == TIM7){
	  IO_MemoryMap_Data.Named.ButtonAzimuthDown = !IO_MemoryMap_Data.Named.ButtonAzimuthDown;
	  IO_MemoryMap_Data.Named.ButtonAzimuthUp = !IO_MemoryMap_Data.Named.ButtonAzimuthUp;
	  IO_MemoryMap_Data.Named.ButtonElevationDown = !IO_MemoryMap_Data.Named.ButtonElevationDown;
	  IO_MemoryMap_Data.Named.ButtonElevationUp = !IO_MemoryMap_Data.Named.ButtonElevationUp;
	  IO_MemoryMap_Data.Named.ButtonManual = !IO_MemoryMap_Data.Named.ButtonManual;
	}
}
/* Xu ly ngăt nhan RS485 tung byte trong chuoi
 * */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	if (huart->Instance == USART1){

		static IMU_STATE_t state = CK_77;
		static uint8_t chksum = 0;
		static uint8_t byte1, byte2, byte3;
		HAL_UART_Receive_IT(&huart1, &IMU_RxData, 1);
		switch (state)
		  {
		  case CK_77:
		    if (IMU_RxData == 0x77)
		    {
		      state = CK_0D;
		    }
		    break;
		  case CK_0D:
		    if (IMU_RxData == 0x0D)
		    {
		    	chksum += IMU_RxData;
		      state = CK_00;
		    }
		    else
		    {
		      state = CK_77;
		    }
		    break;
		  case CK_00:
		    if (IMU_RxData == 0x00)
		    {
		    	chksum += IMU_RxData;
		      state = CK_84;
		    }
		    else
		    {
		      state = CK_77;
		    }
		    break;
		  case CK_84:
		    if (IMU_RxData == 0x84)
		    {
		    	chksum += IMU_RxData;
		      state = READ_X1;
		    }
		    else
		    {
		      state = CK_77;
		    }
		    break;
		  case READ_X1:
		    state = READ_X2;
		    chksum += IMU_RxData;
		    break;
		  case READ_X2:
		    state = READ_X3;
		    chksum += IMU_RxData;
		    break;
		  case READ_X3:
		    state = READ_Y1;
		    chksum += IMU_RxData;
		    break;
		  case READ_Y1:
		    state = READ_Y2;
		    chksum += IMU_RxData;
		    break;
		  case READ_Y2:
		    state = READ_Y3;
		    chksum += IMU_RxData;
		    break;
		  case READ_Y3:
		    state = READ_Z1;
		    chksum += IMU_RxData;
		    break;
		  case READ_Z1:
		    byte1 = IMU_RxData;
		    chksum += IMU_RxData;
		    //        Ang_curr = (RxData & 0x0F) * 10000;
		    state = READ_Z2;
		    break;
		  case READ_Z2:
		    byte2 = IMU_RxData;
		    chksum += IMU_RxData;
		    //        Ang_curr += ((RxData & 0xF0) >> 4) * 1000;
		    //        Ang_curr += (RxData & 0x0F) * 100;
		    state = READ_Z3;
		    break;
		  case READ_Z3:
		    byte3 = IMU_RxData;
		    chksum += IMU_RxData;
		    //        Ang_curr += ((RxData & 0xF0) >> 4) * 10;
		    //        Ang_curr += (RxData & 0x0F);
		    state = CHECKSUM;
		    break;
		  case CHECKSUM:
		    if (IMU_RxData == chksum)
		    {
		      cnt++;
		      Ang_curr = (byte1 & 0x0F) * 10000;
		      Ang_curr += ((byte2 & 0xF0) >> 4) * 1000;
		      Ang_curr += (byte2 & 0x0F) * 100;
		      Ang_curr += ((byte3 & 0xF0) >> 4) * 10;
		      Ang_curr += (byte3 & 0x0F);
		      chksum = 0;
		    }
		    state = CK_77;
		    break;
		  default:
		    break;
		  }
	}
}
//void  _RET(float* fval, uint16_t idx){
//	*fval = *(float*)(&IO_MemoryMap[idx]);
//}
//void _WRT(uint16_t idx, float* fval){
//	uint16_t* pi16 = (uint16_t*)fval;
//	IO_MemoryMap[idx]= *pi16;
//	IO_MemoryMap[idx+1]= *(pi16+1);
//}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	bufferIdx = Size;
  HAL_UARTEx_ReceiveToIdle_IT(&huart3, frame, BUFFER_SIZE);
}
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART3) {
        HAL_UARTEx_ReceiveToIdle_IT(huart, frame, BUFFER_SIZE);
    }
}
void Relay_Update(uint16_t *pMap){
	static uint8_t bitOld[4] = {0,0,0,0};
	uint8_t bit_tmp;

	bit_tmp = pMap[ClutchRelay];
	if(bit_tmp != bitOld[0]){
		bitOld[0] = bit_tmp;
		if(bit_tmp) CLUTCH_RELAY_ON;
		else CLUTCH_RELAY_OFF;
	}

	bit_tmp = pMap[PrecisionValveRelay];
	if(bit_tmp != bitOld[2]){
		bitOld[2] = bit_tmp;
		if(bit_tmp) PRE_VALVE_RELAY_ON;
		else PRE_VALVE_RELAY_OFF;
	}

	bit_tmp = pMap[CoarseValveRelay];
	if(bit_tmp != bitOld[3]){
		bitOld[3] = bit_tmp;
		if(bit_tmp) COA_VALVE_RELAY_ON;
		else COA_VALVE_RELAY_OFF;
	}
}
void Btn_Update(uint16_t *pMap){
	pMap[ButtonManual] = BTN_MANUAL_READ;
	pMap[ButtonElevationUp] = BTN_ELEVATION_UP_READ;
	pMap[ButtonElevationDown] = BTN_ELEVATION_DOWN_READ;
	pMap[ButtonAzimuthUp] = BTN_AZIMUTH_UP_READ;
	pMap[ButtonAzimuthDown] = BTN_AZIMUTH_DOWN_READ;
}
void Relay_init(){
	CLUTCH_RELAY_OFF;
	PRE_VALVE_RELAY_OFF;
	COA_VALVE_RELAY_OFF;
}
float PID(IO_MemoryMap_Struct_t Named, float angle, float angle_real){
	static float ek_1, ek_2, Ik_1;
	float Kp = Named.KpCoeffReg;
	float Ki = Named.KiCoeffReg;
	float Kd = Named.KdCoeffReg;

	float ek = angle - angle_real;

	float Pk = Kp*ek;
	float Ik = Ik_1 + Ki*(ek+ek_1)/2;
	float Dk = Kd*(3*ek-4*ek_1+ek_2);

	ek_2 = ek_1;
	ek_1 = ek;

	Ik_1 = Ik;

	return Pk + Ik + Dk;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim4);
  HAL_TIM_Base_Start_IT(&htim7);
  HAL_UART_Receive_IT(&huart1, &IMU_RxData, 1);

//  HAL_UART_Receive_IT(&huart3, &us_data, 1);
  Relay_init();

  modbus_configure(&huart3, BoardID, HoldingRegSize, 0);
  HAL_UARTEx_ReceiveToIdle_IT(&huart3, frame, BUFFER_SIZE );
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  modbus_update(&huart3, IO_MemoryMap_Data.Array);


	  Relay_Update(IO_MemoryMap_Data.Array);
//	  Btn_Update(IO_MemoryMap_Data.Array);

	  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
//	  HAL_Delay(500);
	  DAC7716_SetVol(-9.5,0x04);  //day buoc
//	  DAC7716_SetVol(-8.5,0x05);
//	  DAC7716_SetVol(-7.5,0x06);
//	  DAC7716_SetVol(-6.5,0x07);
//	  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_3);
//
//	  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_2);
//
//	  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_1);
//
//	  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_0);
//
//	  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_1);
//
//	  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_0);
//

//
//	  _RET(&f0,HoldingReg00_L);
//	  _WRT(HoldingReg01_L, &f1);

//	  if(cmd_flag){
//		  cmd_flag = false;
//		  if(cmd == 0x01){
//			  HAL_UART_Transmit(&huart3, &ack, 1, 100);
//		  }
//		  else if(cmd == 0x00){
//			  HAL_UART_Transmit(&huart3, &ack, 1, 100);
//		  }
//		  else if(cmd == 0x96){
//			  HAL_UART_Transmit(&huart3, &ack, 1, 100);
//		  }
//		  else if(cmd == 0x02){
//			  HAL_UART_Transmit(&huart3, res, 6, 100);
//			  res[2] += 1;
//			  res[3] += 1;
//			  res[4] -= 1;
//		  }
//	  }


  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* USER CODE END SPI3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 6400-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 1000-1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 64000-1;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 1000-1;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2
                          |GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC0 PC1 PC2
                           PC3 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2
                          |GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PD0 PD1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure peripheral I/O remapping */
  __HAL_AFIO_REMAP_PD01_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

