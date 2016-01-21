#include <stdlib.h> 
#include <stdio.h>
#include "mpu9255.h"
#include "spi.h"
#include "system.h"

uint8_t txData,rxData;

uint8_t MPU9255_Init(void){
	if(MPU9255_Read_Reg(WHO_AM_I)==0x73)
	{
		MPU9255_Write_Reg(USER_CTRL,0X10); //使能MPU9255SPI
		MPU9255_Write_Reg(PWR_MGMT_1,0X80);  //电源管理，复位MPU9255
		MPU9255_Write_Reg(SMPLRT_DIV,0x07);//采样率1000/(1+7)=125HZ
		MPU9255_Write_Reg(CONFIG,GYRO_BW_5);				//陀螺仪与温度低通滤波器 0x06 5hz
		MPU9255_Write_Reg(GYRO_CONFIG,GYRO_2000DPS);  //陀螺仪测量范围 0X18 +2000 dps
		MPU9255_Write_Reg(ACCEL_CONFIG,ACC_16G); //加速度计测量范围 0X18 +-16g
		MPU9255_Write_Reg(ACCEL_CONFIG2,ACC_BW_5);	//加速度计低通滤波器，5Hz
		MPU9255_Write_Reg(XG_OFFSET_H,0x00);
		MPU9255_Write_Reg(XG_OFFSET_L,0x00);
		MPU9255_Write_Reg(YG_OFFSET_H,0x00);
		MPU9255_Write_Reg(YG_OFFSET_L,0x00);
		MPU9255_Write_Reg(ZG_OFFSET_H,0x00);
		MPU9255_Write_Reg(ZG_OFFSET_L,0x00);
		return 1;
	}
	return 0;
}

//SPI写寄存器
//reg:指定的寄存器地址
//value:写入的值
uint8_t MPU9255_Write_Reg(uint8_t reg,uint8_t value)
{
	uint8_t status;
	SPI_MPU9255_CS_L;											  //使能SPI传输
	status = HAL_SPI_Transmit(&hspi1, &reg, 1, 0xFFFF);
	HAL_SPI_Transmit(&hspi1, &value, 1, 0xFFFF);
	SPI_MPU9255_CS_H;										  	//禁止MPU9255
	return(status);													//返回状态值
}

//SPI读取指定寄存器
//reg:指定寄存器的地址
uint8_t MPU9255_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;
	SPI_MPU9255_CS_L;	
	reg = reg|0x80;
	HAL_SPI_Transmit(&hspi1, &reg, 1, 0xFFFF);	 	//发送读命令+寄存器号
 	HAL_SPI_Receive(&hspi1, &reg_val, 1, 0xFFFF);				//读取寄存器值
	SPI_MPU9255_CS_H;																//禁止SPI传输
	return(reg_val);
}

//SPI读MPU9255数据
void MPU9255_ReadValue(uint8_t *dataPointer)
{
	uint8_t i,data=ACCEL_XOUT_H|0x80;
	SPI_MPU9255_CS_L;  //使能SPI传输
	HAL_SPI_Transmit(&hspi1, &data, 1, 0xFFFF);
//	for(i=0;i<14;i++)//一共读取14字节的数据
//	{
	HAL_SPI_Receive(&hspi1, dataPointer, 14, 0xFFFF); //循环读取
//		dataPointer = dataPointer+1;
//	}
//	//加速度计
//	MPU9250_ACC_LAST.X = ((int16_t)(*MPU9250_buf)[0]<<8) | (*MPU9250_buf)[1];
//	MPU9250_ACC_LAST.Y = ((int16_t)(*MPU9250_buf)[2]<<8) | (*MPU9250_buf)[3];
//	MPU9250_ACC_LAST.Z = ((int16_t)(*MPU9250_buf)[4]<<8) | (*MPU9250_buf)[5];
//	//温度
//	MPU9250_TEMP_LAST =  ((int16_t)(*MPU9250_buf)[6]<<8) | (*MPU9250_buf)[7];
//	//陀螺仪
//	MPU9250_GYRO_LAST.X = ((int16_t)(*MPU9250_buf)[8]<<8) | (*MPU9250_buf)[9];
//	MPU9250_GYRO_LAST.Y = ((int16_t)(*MPU9250_buf)[10]<<8) | (*MPU9250_buf)[11];
//	MPU9250_GYRO_LAST.Z = ((int16_t)(*MPU9250_buf)[12]<<8) | (*MPU9250_buf)[13];
	SPI_MPU9255_CS_H;  //禁止MPU9250
}
