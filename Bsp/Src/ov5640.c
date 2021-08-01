/*
 * Copyright (c) Mr.Lee. 2021. All rights reserved.
 * Description: 
 * Author: Mr.Lee
 * Create: 2021-03-21
 */

#include "ov5640.h"
#include "simulate_i2c.h"
#include "dwt_delay.h"
#include "lcd_ssd1963.h"
#include "main.h"
#include "uart_device.h"
#include "dcmi.h"
#include "ov5640_af.h"
#include "ov5640_cfg.h"


	
////OV5640写寄存器
////返回值:0,成功;1,失败.
//uint8_t OV5640_WriteReg(uint16_t reg,uint8_t data)
//{
//	uint8_t res=0;
//	SCCB_Start(); 					//启动SCCB传输
//	if(SCCB_WR_Byte(OV5640_ADDR))res=1;	//写器件ID	  
//   	if(SCCB_WR_Byte(reg>>8))res=1;	//写寄存器高8位地址
//   	if(SCCB_WR_Byte(reg))res=1;		//写寄存器低8位地址		  
//   	if(SCCB_WR_Byte(data))res=1; 	//写数据	 
//  	SCCB_Stop();	  
//  	return	res;
//}
////OV5640读寄存器
////返回值:读到的寄存器值
//uint8_t OV5640_ReadReg(uint16_t reg)
//{
//	uint8_t val=0;
//	SCCB_Start(); 				//启动SCCB传输
//	SCCB_WR_Byte(OV5640_ADDR);	//写器件ID
//   	SCCB_WR_Byte(reg>>8);	    //写寄存器高8位地址   
//  	SCCB_WR_Byte(reg);			//写寄存器低8位地址	  
// 	SCCB_Stop();   
// 	//设置寄存器地址后，才是读
//	SCCB_Start();
//	SCCB_WR_Byte(OV5640_ADDR|0X01);//发送读命令	  
//   	val=SCCB_RD_Byte();		 	//读取数据
//  	SCCB_No_Ack();
//  	SCCB_Stop();
//  	return val;
//}





//初始化OV5640 
//配置完以后,默认输出是1600*1200尺寸的图片!! 
//返回值:0,成功
//    其他,错误代码
uint8_t OV5640_Init(void)
{ 
	uint16_t i=0;
	uint16_t reg;
	reg=OV5640_ReadReg(OV5640_CHIPIDH);	//读取ID 高八位
	reg<<=8;
	reg|=OV5640_ReadReg(OV5640_CHIPIDL);	//读取ID 低八位
	if(reg == OV5640_ID)
	{
		DEBUG("Camera OK ID %04X\r\n",reg);
	}
	else
	{
		DEBUG("Camera error ID %04X\r\n",reg);
		return 1;
	}
	OV5640_WriteReg(0x3103,0X11);	//system clock from pad, bit[1]
	OV5640_WriteReg(0X3008,0X82);	//软复位
	DelayM(10);
 	//初始化 OV5640 
	for(i=0;i<sizeof(ov5640_init_reg_tbl)/4;i++)
	{
	   	OV5640_WriteReg(ov5640_init_reg_tbl[i][0],ov5640_init_reg_tbl[i][1]);
 	}    
	//检查闪光灯是否正常
//	OV5640_Flash_Ctrl(1);//打开闪光灯
//	DelayM(50);
//	OV5640_Flash_Ctrl(0);//关闭闪光灯
	DEBUG("OV5640_Init OK\r\n");
  	return 0x00; 	//ok
} 
//OV5640切换为JPEG模式
void OV5640_JPEG_Mode(void) 
{
	uint16_t i=0; 
	//设置:输出JPEG数据
	for(i=0;i<(sizeof(OV5640_jpeg_reg_tbl)/4);i++)
	{
		OV5640_WriteReg(OV5640_jpeg_reg_tbl[i][0],OV5640_jpeg_reg_tbl[i][1]);  
	}  
}
//OV5640切换为RGB565模式
void OV5640_RGB565_Mode(void) 
{
	uint16_t i=0;
	//设置:RGB565输出
	for(i=0;i<(sizeof(ov5640_rgb565_reg_tbl)/4);i++)
	{
		OV5640_WriteReg(ov5640_rgb565_reg_tbl[i][0],ov5640_rgb565_reg_tbl[i][1]); 
	} 
} 
//EV曝光补偿设置参数表，支持7个等级
const static uint8_t OV5640_EXPOSURE_TBL[7][6]=
{
    0x10,0x08,0x10,0x08,0x20,0x10,//-3  
    0x20,0x18,0x41,0x20,0x18,0x10,//-
    0x30,0x28,0x61,0x30,0x28,0x10,//-1 
    0x38,0x30,0x61,0x38,0x30,0x10,//0  
    0x40,0x38,0x71,0x40,0x38,0x10,//+1 
    0x50,0x48,0x90,0x50,0x48,0x20,//+2   
    0x60,0x58,0xa0,0x60,0x58,0x20,//+3    
};

//EV曝光补偿
//exposure:0~6,代表补偿-3~3. 
void OV5640_Exposure(uint8_t exposure)
{
    OV5640_WriteReg(0x3212,0x03);	//start group 3
    OV5640_WriteReg(0x3a0f,OV5640_EXPOSURE_TBL[exposure][0]); 
	OV5640_WriteReg(0x3a10,OV5640_EXPOSURE_TBL[exposure][1]); 
    OV5640_WriteReg(0x3a1b,OV5640_EXPOSURE_TBL[exposure][2]); 
	OV5640_WriteReg(0x3a1e,OV5640_EXPOSURE_TBL[exposure][3]); 
    OV5640_WriteReg(0x3a11,OV5640_EXPOSURE_TBL[exposure][4]); 
    OV5640_WriteReg(0x3a1f,OV5640_EXPOSURE_TBL[exposure][5]); 
    OV5640_WriteReg(0x3212,0x13); //end group 3
	OV5640_WriteReg(0x3212,0xa3); //launch group 3
}

//灯光模式参数表,支持5个模式
const static uint8_t OV5640_LIGHTMODE_TBL[5][7]=
{ 
	0x04,0X00,0X04,0X00,0X04,0X00,0X00,//Auto,自动 
	0x06,0X1C,0X04,0X00,0X04,0XF3,0X01,//Sunny,日光
	0x05,0X48,0X04,0X00,0X07,0XCF,0X01,//Office,办公室
	0x06,0X48,0X04,0X00,0X04,0XD3,0X01,//Cloudy,阴天 
	0x04,0X10,0X04,0X00,0X08,0X40,0X01,//Home,室内
}; 
//白平衡设置
//0:自动
//1:日光sunny
//2,办公室office
//3,阴天cloudy
//4,家里home
void OV5640_Light_Mode(uint8_t mode)
{
	uint8_t i;
	OV5640_WriteReg(0x3212,0x03);	//start group 3
	for(i=0;i<7;i++)OV5640_WriteReg(0x3400+i,OV5640_LIGHTMODE_TBL[mode][i]);//设置饱和度 
	OV5640_WriteReg(0x3212,0x13); //end group 3
	OV5640_WriteReg(0x3212,0xa3); //launch group 3	
}
//色彩饱和度设置参数表,支持7个等级
const static uint8_t OV5640_SATURATION_TBL[7][6]=
{ 
	0X0C,0x30,0X3D,0X3E,0X3D,0X01,//-3 
	0X10,0x3D,0X4D,0X4E,0X4D,0X01,//-2	
	0X15,0x52,0X66,0X68,0X66,0X02,//-1	
	0X1A,0x66,0X80,0X82,0X80,0X02,//+0	
	0X1F,0x7A,0X9A,0X9C,0X9A,0X02,//+1	
	0X24,0x8F,0XB3,0XB6,0XB3,0X03,//+2
 	0X2B,0xAB,0XD6,0XDA,0XD6,0X04,//+3
}; 
//色度设置
//sat:0~6,代表饱和度-3~3. 
void OV5640_Color_Saturation(uint8_t sat)
{ 
	uint8_t i;
	OV5640_WriteReg(0x3212,0x03);	//start group 3
	OV5640_WriteReg(0x5381,0x1c);
	OV5640_WriteReg(0x5382,0x5a);
	OV5640_WriteReg(0x5383,0x06);
	for(i=0;i<6;i++)OV5640_WriteReg(0x5384+i,OV5640_SATURATION_TBL[sat][i]);//设置饱和度 
	OV5640_WriteReg(0x538b, 0x98);
	OV5640_WriteReg(0x538a, 0x01);
	OV5640_WriteReg(0x3212, 0x13); //end group 3
	OV5640_WriteReg(0x3212, 0xa3); //launch group 3	
}
//亮度设置
//bright:0~8,代表亮度-4~4.
void OV5640_Brightness(uint8_t bright)
{
	uint8_t brtval;
	if(bright<4)brtval=4-bright;
	else brtval=bright-4;
	OV5640_WriteReg(0x3212,0x03);	//start group 3
	OV5640_WriteReg(0x5587,brtval<<4);
	if(bright<4)OV5640_WriteReg(0x5588,0x09);
	else OV5640_WriteReg(0x5588,0x01);
	OV5640_WriteReg(0x3212,0x13); //end group 3
	OV5640_WriteReg(0x3212,0xa3); //launch group 3
}
//对比度设置
//contrast:0~6,代表亮度-3~3.
void OV5640_Contrast(uint8_t contrast)
{
	uint8_t reg0val=0X00;//contrast=3,默认对比度
	uint8_t reg1val=0X20;
  	switch(contrast)
	{
		case 0://-3
			reg1val=reg0val=0X14;	 	 
			break;	
		case 1://-2
			reg1val=reg0val=0X18; 	 
			break;	
		case 2://-1
			reg1val=reg0val=0X1C;	 
			break;	
		case 4://1
			reg0val=0X10;	 	 
			reg1val=0X24;	 	 
			break;	
		case 5://2
			reg0val=0X18;	 	 
			reg1val=0X28;	 	 
			break;	
		case 6://3
			reg0val=0X1C;	 	 
			reg1val=0X2C;	 	 
			break;	
	} 
	OV5640_WriteReg(0x3212,0x03); //start group 3
	OV5640_WriteReg(0x5585,reg0val);
	OV5640_WriteReg(0x5586,reg1val); 
	OV5640_WriteReg(0x3212,0x13); //end group 3
	OV5640_WriteReg(0x3212,0xa3); //launch group 3
}
//锐度设置
//sharp:0~33,0,关闭;33,auto;其他值,锐度范围.
void OV5640_Sharpness(uint8_t sharp)
{
	if(sharp<33)//设置锐度值
	{
		OV5640_WriteReg(0x5308,0x65);
		OV5640_WriteReg(0x5302,sharp);
	}else	//自动锐度
	{
		OV5640_WriteReg(0x5308,0x25);
		OV5640_WriteReg(0x5300,0x08);
		OV5640_WriteReg(0x5301,0x30);
		OV5640_WriteReg(0x5302,0x10);
		OV5640_WriteReg(0x5303,0x00);
		OV5640_WriteReg(0x5309,0x08);
		OV5640_WriteReg(0x530a,0x30);
		OV5640_WriteReg(0x530b,0x04);
		OV5640_WriteReg(0x530c,0x06);
	}
	
}
//特效设置参数表,支持7个特效
const static uint8_t OV5640_EFFECTS_TBL[7][3]=
{ 
	0X06,0x40,0X10,//正常 
	0X1E,0xA0,0X40,//冷色
	0X1E,0x80,0XC0,//暖色
	0X1E,0x80,0X80,//黑白
	0X1E,0x40,0XA0,//泛黄 
	0X40,0x40,0X10,//反色
	0X1E,0x60,0X60,//偏绿
}; 
//特效设置
//0:正常    
//1,冷色
//2,暖色   
//3,黑白
//4,偏黄
//5,反色
//6,偏绿	    
void OV5640_Special_Effects(uint8_t eft)
{ 
	OV5640_WriteReg(0x3212,0x03); //start group 3
	OV5640_WriteReg(0x5580,OV5640_EFFECTS_TBL[eft][0]);
	OV5640_WriteReg(0x5583,OV5640_EFFECTS_TBL[eft][1]);// sat U
	OV5640_WriteReg(0x5584,OV5640_EFFECTS_TBL[eft][2]);// sat V
	OV5640_WriteReg(0x5003,0x08);
	OV5640_WriteReg(0x3212,0x13); //end group 3
	OV5640_WriteReg(0x3212,0xa3); //launch group 3
}
//测试序列
//mode:0,关闭
//     1,彩条 
//     2,色块
void OV5640_Test_Pattern(uint8_t mode)
{
	if(mode==0)OV5640_WriteReg(0X503D,0X00);
	else if(mode==1)OV5640_WriteReg(0X503D,0X80);
	else if(mode==2)OV5640_WriteReg(0X503D,0X82);
} 
//闪光灯控制
//mode:0,关闭
//     1,打开 
void OV5640_Flash_Ctrl(uint8_t sw)
{
	OV5640_WriteReg(0x3016,0X02);
	OV5640_WriteReg(0x301C,0X02); 
	if(sw)OV5640_WriteReg(0X3019,0X02); 
	else OV5640_WriteReg(0X3019,0X00);
} 
//设置图像输出大小
//OV5640输出图像的大小(分辨率),完全由该函数确定
//offx,offy,为输出图像在OV5640_ImageWin_Set设定窗口(假设长宽为xsize和ysize)上的偏移
//由于开启了scale功能,用于输出的图像窗口为:xsize-2*offx,ysize-2*offy
//width,height:实际输出图像的宽度和高度
//实际输出(width,height),是在xsize-2*offx,ysize-2*offy的基础上进行缩放处理.
//一般设置offx和offy的值为16和4,更小也是可以,不过默认是16和4 
//返回值:0,设置成功
//    其他,设置失败
uint8_t OV5640_OutSize_Set(uint16_t offx,uint16_t offy,uint16_t width,uint16_t height)
{ 
    OV5640_WriteReg(0X3212,0X03);  	//开始组3
    //以下设置决定实际输出尺寸(带缩放)
    OV5640_WriteReg(0x3808,width>>8);	//设置实际输出宽度高字节
    OV5640_WriteReg(0x3809,width&0xff);//设置实际输出宽度低字节  
    OV5640_WriteReg(0x380a,height>>8);//设置实际输出高度高字节
    OV5640_WriteReg(0x380b,height&0xff);//设置实际输出高度低字节
	//以下设置决定输出尺寸在ISP上面的取图范围
	//范围:xsize-2*offx,ysize-2*offy
    OV5640_WriteReg(0x3810,offx>>8);	//设置X offset高字节
    OV5640_WriteReg(0x3811,offx&0xff);//设置X offset低字节
	
    OV5640_WriteReg(0x3812,offy>>8);	//设置Y offset高字节
    OV5640_WriteReg(0x3813,offy&0xff);//设置Y offset低字节
	
    OV5640_WriteReg(0X3212,0X13);		//结束组3
    OV5640_WriteReg(0X3212,0Xa3);		//启用组3设置
	return 0; 
}

//设置图像开窗大小(ISP大小),非必要,一般无需调用此函数
//在整个传感器上面开窗(最大2592*1944),用于OV5640_OutSize_Set的输出
//注意:本函数的宽度和高度,必须大于等于OV5640_OutSize_Set函数的宽度和高度
//     OV5640_OutSize_Set设置的宽度和高度,根据本函数设置的宽度和高度,由DSP
//     自动计算缩放比例,输出给外部设备.
//width,height:宽度(对应:horizontal)和高度(对应:vertical)  
//返回值:0,设置成功
//    其他,设置失败
uint8_t OV5640_ImageWin_Set(uint16_t offx,uint16_t offy,uint16_t width,uint16_t height)
{
	uint16_t xst,yst,xend,yend;
	xst=offx;
	yst=offy;
	xend=offx+width-1;
	yend=offy+height-1;  
    OV5640_WriteReg(0X3212,0X03);		//开始组3
	OV5640_WriteReg(0X3800,xst>>8);	
	OV5640_WriteReg(0X3801,xst&0XFF);	
	OV5640_WriteReg(0X3802,yst>>8);	
	OV5640_WriteReg(0X3803,yst&0XFF);	
	OV5640_WriteReg(0X3804,xend>>8);	
	OV5640_WriteReg(0X3805,xend&0XFF);
	OV5640_WriteReg(0X3806,yend>>8);	
	OV5640_WriteReg(0X3807,yend&0XFF);
    OV5640_WriteReg(0X3212,0X13);		//结束组3
    OV5640_WriteReg(0X3212,0Xa3);		//启用组3设置	 
	return 0;
}   
//初始化自动对焦
//返回值:0,成功;1,失败.
uint8_t OV5640_Focus_Init(void)
{ 
	uint16_t i; 
	uint16_t addr=0x8000;
	uint8_t state=0x8F;
	OV5640_WriteReg(0x3000, 0x20);			//reset MCU	 
	for(i=0;i<sizeof(OV5640_AF_Config);i++) //发送配置数组
	{
		OV5640_WriteReg(addr,OV5640_AF_Config[i]);
		addr++;
	}  
	OV5640_WriteReg(0x3022,0x00);
	OV5640_WriteReg(0x3023,0x00);
	OV5640_WriteReg(0x3024,0x00);
	OV5640_WriteReg(0x3025,0x00);
	OV5640_WriteReg(0x3026,0x00);
	OV5640_WriteReg(0x3027,0x00);
	OV5640_WriteReg(0x3028,0x00);
	OV5640_WriteReg(0x3029,0x7f);
	OV5640_WriteReg(0x3000,0x00); 
	i=0;
	do
	{
		state=OV5640_ReadReg(0x3029);	
		DelayM(5);
		i++;
		if(i>1000)
		{
			DEBUG("\r\n");
			return 1;
		}
	}while(state!=0x70); 
	return 0;    
}  
//执行一次自动对焦
//返回值:0,成功;1,失败.
uint8_t OV5640_Focus_Single(void)
{
	uint8_t temp; 
	uint16_t retry=0; 
	OV5640_WriteReg(0x3022,0x03);		//触发一次自动对焦 
	while(1)
	{
		retry++;
		temp=OV5640_ReadReg(0x3029);	//检查对焦完成状态
		if(temp==0x10)break;		// focus completed
		DelayM(5);
		if(retry>1000)return 1;
	}
	return 0;	 		
}
//持续自动对焦,当失焦后,会自动继续对焦
//返回值:0,成功;其他,失败.
uint8_t OV5640_Focus_Constant(void)
{
	uint8_t temp=0;   
	uint16_t retry=0; 
	OV5640_WriteReg(0x3023,0x01);
	OV5640_WriteReg(0x3022,0x08);//发送IDLE指令 
	do 
	{
		temp=OV5640_ReadReg(0x3023); 
		retry++;
		if(retry>1000)
		{
			DEBUG("\r\n");
			return 2;
		}
		DelayM(5);
	} while(temp!=0x00);   
	OV5640_WriteReg(0x3023,0x01);
	OV5640_WriteReg(0x3022,0x04);//发送持续对焦指令 
	retry=0;
	do 
	{
		temp=OV5640_ReadReg(0x3023); 
		retry++;
		if(retry>1000)
		{
			DEBUG("\r\n");
			return 2;
		}
		DelayM(5);
	}while(temp!=0x00);//0,对焦完成;1:正在对焦
	return 0;
} 	
	

/*-------------------------------------------------------------------*/


void Ov5640IoInit(void)
{
    HAL_GPIO_WritePin(DCMI_RESET_GPIO_Port, DCMI_RESET_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DCMI_PWDN_GPIO_Port, DCMI_PWDN_Pin, GPIO_PIN_SET);
    DelayM(10);	
	HAL_GPIO_WritePin(DCMI_PWDN_GPIO_Port, DCMI_PWDN_Pin, GPIO_PIN_RESET);
    DelayM(10);
    HAL_GPIO_WritePin(DCMI_RESET_GPIO_Port, DCMI_RESET_Pin, GPIO_PIN_SET);
    DelayM(50);
	
	SimulateI2cInit();
	DEBUG("Ov5640IoInit \r\n");
}

/**
  * @brief  写一字节数据到OV5640寄存器
  * @param  Addr: OV5640 的寄存器地址
  * @param  Data: 要写入的数据
  * @retval 返回0表示写入正常，0xFF表示错误
  */
uint8_t OV5640_WriteReg(uint16_t Addr, uint8_t Data)
{
	uint8_t ret = 0;
	
	SimulateI2cStart(&dcmiI2cPort);
	
	ret = I2cWriteByte(&dcmiI2cPort, OV5640_ADDR);
	if (ret != I2C_ACK)
	{
		DEBUG();
		SimulateI2cStop(&dcmiI2cPort);		
		return 0xff;
	}
	ret = I2cWriteByte(&dcmiI2cPort, (uint8_t)((Addr>>8) & 0xFF));
	if (ret != I2C_ACK)
	{
		DEBUG();
		SimulateI2cStop(&dcmiI2cPort);		
		return 0xff;
	}	
	ret = I2cWriteByte(&dcmiI2cPort, (uint8_t)(Addr & 0xFF));
	if (ret != I2C_ACK)
	{
		DEBUG();
		SimulateI2cStop(&dcmiI2cPort);
		return 0xff;
	}	
 	ret = I2cWriteByte(&dcmiI2cPort, Data);
	if (ret != I2C_ACK)
	{
		DEBUG();	
		SimulateI2cStop(&dcmiI2cPort);
		return 0xff;
	}	
	SimulateI2cStop(&dcmiI2cPort);
	
	return 0;
}


/**
  * @brief  从OV5640寄存器中读取一个字节的数据
  * @param  Addr: 寄存器地址
  * @retval 返回读取得的数据
  */
uint8_t OV5640_ReadReg(uint16_t Addr)
{
	uint8_t ret;
	uint8_t ack;

	SimulateI2cStart(&dcmiI2cPort);

	ack = I2cWriteByte(&dcmiI2cPort, OV5640_ADDR);
	if (ack != I2C_ACK)
	{
		DEBUG();
		SimulateI2cStop(&dcmiI2cPort);
		return 0xff;
	}		
	ack = I2cWriteByte(&dcmiI2cPort, (uint8_t)((Addr>>8) & 0xFF));
	if (ack != I2C_ACK)
	{
		DEBUG();
		SimulateI2cStop(&dcmiI2cPort);
		return 0xff;
	}		
	ack = I2cWriteByte(&dcmiI2cPort, (uint8_t)(Addr & 0xFF));	
	if (ack != I2C_ACK)
	{
		DEBUG();
		SimulateI2cStop(&dcmiI2cPort);
		return 0xff;
	}	
	SimulateI2cStart(&dcmiI2cPort);
	ack = I2cWriteByte(&dcmiI2cPort, OV5640_ADDR | 0x01);
	if (ack != I2C_ACK)
	{
		DEBUG();
		SimulateI2cStop(&dcmiI2cPort);
		return 0xff;
	}	
	
	ret = I2cReadByte(&dcmiI2cPort, I2C_NOACK);
	
	SimulateI2cStop(&dcmiI2cPort);

	return ret;
}

void OV5640_CONFIG(void)
{
	//自动对焦初始化
	OV5640_RGB565_Mode();	//RGB565模式 
	OV5640_Focus_Init();
	
	OV5640_Light_Mode(0);	//自动模式
	OV5640_Color_Saturation(3);//色彩饱和度0
	OV5640_Brightness(4);	//亮度0
	OV5640_Contrast(3);		//对比度0
	OV5640_Sharpness(33);	//自动锐度
	OV5640_Focus_Constant();//启动持续对焦
	
	OV5640_OutSize_Set(4, 0, SSD_HOR_RESOLUTION-200, SSD_VER_RESOLUTION-100);
	Ov5640DcmiDmaConfig();
}

extern DCMI_HandleTypeDef hdcmi;
extern DMA_HandleTypeDef hdma_dcmi;
// 配置 DCMI/DMA 以捕获摄像头数据
void Ov5640DcmiDmaConfig(void)
{
	CameraSetWindow();
//	LcdWriteCmd8Bit(0x2c);	
	//关闭所有中断，函数HAL_DCMI_Init()会默认打开很多中断，开启这些中断
	//以后我们就需要对这些中断做相应的处理，否则的话就会导致各种各样的问题，
	//但是这些中断很多都不需要，所以这里将其全部关闭掉，也就是将IER寄存器清零。
	//关闭完所有中断以后再根据自己的实际需求来使能相应的中断。
//	DCMI->IER=0x0;										
//	
//	__HAL_DCMI_ENABLE_IT(&hdcmi,DCMI_IT_FRAME);		//开启帧中断
//	__HAL_DCMI_ENABLE(&hdcmi);						//使能DCMI	
	
//    __HAL_UNLOCK(&hdma_dcmi);
//	HAL_DMA_Start(&hdma_dcmi, (uint32_t)&DCMI->DR, LCD_DATA_ADDR, 1);
//	HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS, LCD_DATA_ADDR, 1);

//	__HAL_DMA_ENABLE(&hdma_dcmi); 	//使能DMA	
	DMA2_Stream1->CR|=1<<0;		//开启DMA2,Stream1 
//	DCMI->CR |= DCMI_CR_ENABLE;
	DCMI->CR |= DCMI_CR_CAPTURE;    	//DCMI捕获使能 	
//	HAL_DMA_Start(&hdma_dcmi, (uint32_t)&DCMI->DR, LCD_DATA_ADDR, 1);
	
}


//DCMI,关闭传输
void DcmiStop(void)
{
	DCMI->CR&=~(1<<0); 			//DCMI捕获关闭   
	while(DCMI->CR&0X01);		//等待传输结束 
	DMA2_Stream1->CR&=~(1<<0);	//关闭DMA2,Stream1  	
} 










/************************ (C) COPYRIGHT Mr.Lee *****END OF FILE****/
//Ov5640IoInit();
//Ov5640DcmiDmaConfig();

