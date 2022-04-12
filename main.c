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
L_SDN;
Usart1_InitConfig(115200); //串口 1 初始化:波特率为 115200 ，工作模式为 8-n-1
MD_DiZhi = 7;
Zhi_Ling = 0;
AD_Config();
AD_KaiShi();
delay_ms(100);
H_SDN;
/*无限循环*/
while(1)
{
for(shuju_zhizhen = 0 ; shuju_zhizhen < 19;shuju_zhizhen = shuju_zhizhen + 2)
{
delay_ms(10);
adcx=Get_Adc(ADC_Channel_5);//adc 通道 5
Sensor_Data[shuju_zhizhen] = (adcx>>8) & 0x0ff;//这里一共能放 20 组 都为 1 出 1 其它出 0
按位与 得到高八位
Sensor_Data[shuju_zhizhen+1] = adcx & 0xff; //得到低八位
}
if(MO_shi > 0)//工作模式
{
Lianxu_sj();
}
if (Zhi_Ling == 1) //USART1 收到消息
{
ZhiLing_jiexi(Usart1_Buf,Buffer_Address1); //变量在 usart.h 里面
Buffer_Address1 = 0;
}
}
}
/*---------------------------函数自定义 --------------------------------*/
/**
* 函数名称: Nvic_InitConfig
* 描 述: 中断向量初始化配置
* 输 入: 无
* 输 出: 无
* 返 回: 无
* 说 明: 设置中断优先级
***/
void Nvic_InitConfig()
{
NVIC_InitTypeDef NVIC_InitStructure; //定义 NVIC 初始化结构体
NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);//将向量表基地选址在 0x08000000
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置中断优先级组为 2，先占优先级
和从优先级各两位(可设 0～3)
/*串口 1 中断向量配置*/
NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; //串口 1 中断
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //设置抢先优先级
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //设置响应优先级
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能 NVIC
NVIC_Init(&NVIC_InitStructure);
}
