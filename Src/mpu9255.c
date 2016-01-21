#include <stdlib.h> 
#include <stdio.h>
#include "mpu9255.h"
#include "spi.h"
#include "system.h"

uint8_t txData,rxData;

uint8_t MPU9255_Init(void){
	if(MPU9255_Read_Reg(WHO_AM_I)==0x73)
	{
		MPU9255_Write_Reg(USER_CTRL,0X10); //ʹ��MPU9255SPI
		MPU9255_Write_Reg(PWR_MGMT_1,0X80);  //��Դ������λMPU9255
		MPU9255_Write_Reg(SMPLRT_DIV,0x07);//������1000/(1+7)=125HZ
		MPU9255_Write_Reg(CONFIG,GYRO_BW_5);				//���������¶ȵ�ͨ�˲��� 0x06 5hz
		MPU9255_Write_Reg(GYRO_CONFIG,GYRO_2000DPS);  //�����ǲ�����Χ 0X18 +2000 dps
		MPU9255_Write_Reg(ACCEL_CONFIG,ACC_16G); //���ٶȼƲ�����Χ 0X18 +-16g
		MPU9255_Write_Reg(ACCEL_CONFIG2,ACC_BW_5);	//���ٶȼƵ�ͨ�˲�����5Hz
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

//SPIд�Ĵ���
//reg:ָ���ļĴ�����ַ
//value:д���ֵ
uint8_t MPU9255_Write_Reg(uint8_t reg,uint8_t value)
{
	uint8_t status;
	SPI_MPU9255_CS_L;											  //ʹ��SPI����
	status = HAL_SPI_Transmit(&hspi1, &reg, 1, 0xFFFF);
	HAL_SPI_Transmit(&hspi1, &value, 1, 0xFFFF);
	SPI_MPU9255_CS_H;										  	//��ֹMPU9255
	return(status);													//����״ֵ̬
}

//SPI��ȡָ���Ĵ���
//reg:ָ���Ĵ����ĵ�ַ
uint8_t MPU9255_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;
	SPI_MPU9255_CS_L;	
	reg = reg|0x80;
	HAL_SPI_Transmit(&hspi1, &reg, 1, 0xFFFF);	 	//���Ͷ�����+�Ĵ�����
 	HAL_SPI_Receive(&hspi1, &reg_val, 1, 0xFFFF);				//��ȡ�Ĵ���ֵ
	SPI_MPU9255_CS_H;																//��ֹSPI����
	return(reg_val);
}

//SPI��MPU9255����
void MPU9255_ReadValue(uint8_t *dataPointer)
{
	uint8_t i,data=ACCEL_XOUT_H|0x80;
	SPI_MPU9255_CS_L;  //ʹ��SPI����
	HAL_SPI_Transmit(&hspi1, &data, 1, 0xFFFF);
//	for(i=0;i<14;i++)//һ����ȡ14�ֽڵ�����
//	{
	HAL_SPI_Receive(&hspi1, dataPointer, 14, 0xFFFF); //ѭ����ȡ
//		dataPointer = dataPointer+1;
//	}
//	//���ٶȼ�
//	MPU9250_ACC_LAST.X = ((int16_t)(*MPU9250_buf)[0]<<8) | (*MPU9250_buf)[1];
//	MPU9250_ACC_LAST.Y = ((int16_t)(*MPU9250_buf)[2]<<8) | (*MPU9250_buf)[3];
//	MPU9250_ACC_LAST.Z = ((int16_t)(*MPU9250_buf)[4]<<8) | (*MPU9250_buf)[5];
//	//�¶�
//	MPU9250_TEMP_LAST =  ((int16_t)(*MPU9250_buf)[6]<<8) | (*MPU9250_buf)[7];
//	//������
//	MPU9250_GYRO_LAST.X = ((int16_t)(*MPU9250_buf)[8]<<8) | (*MPU9250_buf)[9];
//	MPU9250_GYRO_LAST.Y = ((int16_t)(*MPU9250_buf)[10]<<8) | (*MPU9250_buf)[11];
//	MPU9250_GYRO_LAST.Z = ((int16_t)(*MPU9250_buf)[12]<<8) | (*MPU9250_buf)[13];
	SPI_MPU9255_CS_H;  //��ֹMPU9250
}
