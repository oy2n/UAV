#include <math.h>
#include "stdbool.h"
#include "delay.h"
#include "config.h"
#include "bmp280.h"
#include "iic.h"

/*����bmp280��ѹ���¶ȹ����� ����ģʽ*/
#define BMP280_PRESSURE_OSR         (BMP280_OVERSAMP_16X)
#define BMP280_TEMPERATURE_OSR      (BMP280_OVERSAMP_4X)
#define BMP280_MODE                 (BMP280_PRESSURE_OSR << 2 | BMP280_TEMPERATURE_OSR << 5 | BMP280_NORMAL_MODE) //

/*����bmp280��ѹIIR�˲���*/
#define BMP280_FILTER               (4 << 2)	// BMP280_FILTER_COEFF_16

#define BMP280_DATA_FRAME_SIZE      (6)

typedef struct 
{
    u16 dig_T1;	/* calibration T1 data */
    s16 dig_T2; /* calibration T2 data */
    s16 dig_T3; /* calibration T3 data */
    u16 dig_P1;	/* calibration P1 data */
    s16 dig_P2; /* calibration P2 data */
    s16 dig_P3; /* calibration P3 data */
    s16 dig_P4; /* calibration P4 data */
    s16 dig_P5; /* calibration P5 data */
    s16 dig_P6; /* calibration P6 data */
    s16 dig_P7; /* calibration P7 data */
    s16 dig_P8; /* calibration P8 data */
    s16 dig_P9; /* calibration P9 data */
    s32 t_fine; /* calibration t_fine data */
} bmp280Calib;


bmp280Calib  bmp280Cal;
static bool isInit = false;
s32 bmp280RawPressure = 0;
s32 bmp280RawTemperature = 0;
float baroTemperature,baroPressure;
u8 bmpdata[BMP280_DATA_FRAME_SIZE];

bool bmp280Init(void)
{	
	if (isInit) return true;
	u8 id = 0x00;
	u8 data = 0x00;
	for (int i=0; i<10; i++)
	{
		HAL_I2C_Mem_Read(&hi2c1,BMP280_I2C_ADDR << 1,BMP280_CHIP_ID,I2C_MEMADD_SIZE_8BIT,&id,1,1000);//��ȡID
		if (id == BMP280_DEFAULT_CHIP_ID)
		{
			break;
		}
		HAL_Delay(10);
	}
	
	if (id == BMP280_DEFAULT_CHIP_ID)//��ȡ����
	{
		HAL_I2C_Mem_Read(&hi2c1,BMP280_I2C_ADDR << 1,BMP280_TEMPERATURE_CALIB_DIG_T1_LSB_REG,I2C_MEMADD_SIZE_8BIT,(u8*)&bmp280Cal,24,1000);//��ȡУ׼����
		data = BMP280_MODE;
		HAL_I2C_Mem_Write(&hi2c1,BMP280_I2C_ADDR << 1,BMP280_CTRL_MEAS_REG,I2C_MEMADD_SIZE_8BIT,&data,1,1000);//���ù������ʺ͹���ģʽ
		data = BMP280_FILTER;
		HAL_I2C_Mem_Write(&hi2c1,BMP280_I2C_ADDR << 1,BMP280_CONFIG_REG,I2C_MEMADD_SIZE_8BIT,&data,1,1000);//����IIR�˲�
		isInit = true;
		printf("BMP280 I2C connection [OK].\n");
//		printf("BMP280 Calibrate Registor Are: \r\n");
//		for(i=0;i<24;i++)
//			printf("Registor %2d: 0x%X\n",i,p[i]);
	}
    else
	{
		printf("BMP280 I2C connection [FAIL].\n");
	}

    return isInit;
}

void bmp280GetPressure(void)
{
    if(!IICReadRegister(&hi2c1,BMP280_I2C_ADDR << 1,BMP280_PRESSURE_MSB_REG,I2C_MEMADD_SIZE_8BIT,&bmpdata[0],BMP280_DATA_FRAME_SIZE))
    {
    	bmp280RawPressure = (s32)((((uint32_t)(bmpdata[0])) << 12) | (((uint32_t)(bmpdata[1])) << 4) | ((uint32_t)bmpdata[2] >> 4));
    	bmp280RawTemperature = (s32)((((uint32_t)(bmpdata[3])) << 12) | (((uint32_t)(bmpdata[4])) << 4) | ((uint32_t)bmpdata[5] >> 4));
    }
}

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of "5123" equals 51.23 DegC
// t_fine carries fine temperature as global value
u32 bmp280CompensateT(s32 adcT)
{
    s32 var1, var2, T;

    var1 = ((((adcT >> 3) - ((s32)bmp280Cal.dig_T1 << 1))) * ((s32)bmp280Cal.dig_T2)) >> 11;
    var2  = (((((adcT >> 4) - ((s32)bmp280Cal.dig_T1)) * ((adcT >> 4) - ((s32)bmp280Cal.dig_T1))) >> 12) * ((s32)bmp280Cal.dig_T3)) >> 14;
    bmp280Cal.t_fine = var1 + var2;
    T = (bmp280Cal.t_fine * 5 + 128) >> 8;

    return T;
}

// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8 fractional bits).
// Output value of "24674867" represents 24674867/256 = 96386.2 Pa = 963.862 hPa
u32 bmp280CompensateP(s32 adcP)
{
    int64_t var1, var2, p;
    var1 = ((int64_t)bmp280Cal.t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)bmp280Cal.dig_P6;
    var2 = var2 + ((var1*(int64_t)bmp280Cal.dig_P5) << 17);
    var2 = var2 + (((int64_t)bmp280Cal.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)bmp280Cal.dig_P3) >> 8) + ((var1 * (int64_t)bmp280Cal.dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)bmp280Cal.dig_P1) >> 33;
    if (var1 == 0)
        return 0;
    p = 1048576 - adcP;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)bmp280Cal.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)bmp280Cal.dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)bmp280Cal.dig_P7) << 4);
    return (uint32_t)p;
}

void bmp280GetData(float* pressure, float* temperature)
{
	bmp280GetPressure();
	*temperature = (float)bmp280CompensateT(bmp280RawTemperature)/100.0f;	/*��λ��*/
	*pressure = (float)bmp280CompensateP(bmp280RawPressure)/256.0f;		/*��λPa*/
	//*asl=bmp280PressureToAltitude(pressure);	/*ת���ɺ���*/
}

void bmp280GetDat()
{
	bmp280GetPressure();
	baroTemperature = (float)bmp280CompensateT(bmp280RawTemperature)/100.0f;	/*��λ��*/
	baroPressure = (float)bmp280CompensateP(bmp280RawPressure)/256.0f;		/*��λPa*/
	//*asl=bmp280PressureToAltitude(pressure);	/*ת���ɺ���*/
}

#define CONST_PF 0.1902630958	//(1/5.25588f) Pressure factor
#define FIX_TEMP 25				// Fixed Temperature. ASL is a function of pressure and temperature, but as the temperature changes so much (blow a little towards the flie and watch it drop 5 degrees) it corrupts the ASL estimates.
								// TLDR: Adjusting for temp changes does more harm than good.
/**
 * Converts pressure to altitude above sea level (ASL) in meters
 */
float bmp280PressureToAltitude(float* pressure/*, float* groundPressure, float* groundTemp*/)
{
    if (*pressure > 0)
    {
        return ((pow((1015.7f / *pressure), CONST_PF) - 1.0f) * (FIX_TEMP + 273.15f)) / 0.0065f;
    }
    else
    {
        return 0;
    }
}
