#include "stm32f10x.h" #include "USART.h" #include "AD_Zhuanhua.h" #include "delay.h" #include "modbus_jx.h" #include "stm32f10x_it.h"
/* Private define 宏定义---------------------------*/
#define H_SDN GPIO_SetBits(GPIOB,GPIO_Pin_8); //PA8=1
#define L_SDN GPIO_ResetBits(GPIOB,GPIO_Pin_8); //PA8=0 对引脚进行配置 低功耗关
断
/*---------------------------定 义----------------------------------------*/
u8 MD_DiZhi ; //modbus 通信地址
/*-------------------------变 量 声 明---------------------------------------*/
u8 Zhi_Ling; //上位机指令，大于零处理
u8 Usart1_Buf[200];//串口 1 缓存
u8 Buffer_Address1 ;//首地址为 0
u16 adcx;//ad 转换的值
u8Sensor_Data[20]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0
0,0x00,0x00,0x00,0x00,0x00,0x00};
u8 MO_shi = 0; //工作模式
u8 shuju_zhizhen = 0; //数据指针，指向数组
/*--------------------------函 数 声 明-------------------------------------*/
void Nvic_InitConfig(void);//中断配置
/*---------------------------主 函 数 入 口--------------------------------*/
/***
* 函数名称: main
* 描 述: 主函数
* 输入参数: 无
* 输 出: 无
* 返 回: 无
* 说 明: 无
***/
int main(void)
{
/*系统滴答时钟初始化*/
delay_init();
/*外设初始化*/
Nvic_InitConfig(); //中断初始化设置
