
#ifndef __GPS_H
#define	__GPS_H
#include<stdint.h>
#include<stdio.h>
#include<string.h>
#include"sys.h"
#define EARTHR 6371004
//-----GNSS fixType-----//
typedef enum
{
  NO_FIX = 0x00,//�޶�λ
  DEAD_RECKONING_ONLY = 0x01,//���ߵ���λ
  FIX_2D = 0x02,//2ά��λ
  FIX_3D = 0x03,//3ά��λ
  GNSS_DEAD_RECKONING = 0x04,//GPS��ߵ���λ
  TIME_ONLY_FIX = 0x05//ֻ��ʱ�䶨λ
}GNSS_FixType_Def;

typedef struct
{
  float E;
  float N;
  float U;
}Vector3_Nav;

typedef struct
{
  float Last_Time;
  float Now_Time;
  float Time_Delta;
  uint16_t Time_Delta_INT;//��λms
}Testime;

void GPS_PVT_Parse(void);
void Ublox_Set_Output_PVT_10hz_Baud_Set(unsigned long Baud);
void Set_GPS_USART(void);

extern double Last_Longitude,Last_Latitude;
extern int32_t Longitude_Origion,Latitude_Origion,Altitude_Origion;
extern double Longitude,Latitude;

extern double Longitude_Deg,Latitude_Deg;
extern double Longitude_Min,Latitude_Min;
extern u16 TimeBeijing[3];
extern char TimeStr[8];
extern float GPS_Quality;
extern uint16_t GPS_Sate_Num,Last_GPS_Sate_Num;
extern float GPS_Yaw;
extern float GPS_Ground_Speed;
extern float GPS_Speed_Resolve[2];
extern Vector3_Nav GPS_Vel;
extern Vector3_Nav GPS_Vel_Div;
extern float Horizontal_Acc_Est;//ˮƽλ�þ���
extern float GPS_Pos_DOP;
extern uint8_t  GPS_Fix_Flag[4];

extern uint8_t GPS_FixType;
extern float High_GPS;

#endif
