#include "FT6336.h"

 
_m_tp_dev tp_dev=
{
	FT6336_Init,
	FT6336_Scan,
	0,
	0,
	0, 
	0,
	0,
	0,
	0,	  	 		
	0,
	0,	  	 		
};		

//向FT6336写入一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:写数据长度
//返回值:0,成功;1,失败.
uint8_t FT6336_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
    return (uint8_t)HAL_I2C_Mem_Write(&hi2c2,FT_CMD_WR,reg,I2C_MEMADD_SIZE_8BIT,buf,len,0xffffff);
}
//从FT6336读出一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:读数据长度			  
void FT6336_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len)
{
	HAL_I2C_Mem_Read(&hi2c2,FT_CMD_WR,reg,I2C_MEMADD_SIZE_8BIT,buf,len,0xffffff);    
} 
//初始化FT6336触摸屏
uint8_t FT6336_Init(void)
{
    return 0;
}
const uint16_t FT5206_TPX_TBL[5]={FT_TP1_REG,FT_TP2_REG,FT_TP3_REG,FT_TP4_REG,FT_TP5_REG};
//扫描触摸屏(采用查询方式)
//mode:0,正常扫描.
//返回值:当前触屏状态.
//0,触屏无触摸;1,触屏有触摸
uint8_t FT6336_Scan(uint8_t mode)
{
	uint8_t buf[4];
	uint8_t i=0;
	uint8_t res=0;
	uint8_t temp;
	static uint8_t t=0;//控制查询间隔,从而降低CPU占用率   
	t++;
	if((t%10)==0||t<10)//空闲时,每进入10次CTP_Scan函数才检测1次,从而节省CPU使用率
	{
		FT6336_RD_Reg(FT_REG_NUM_FINGER,&mode,1);//读取触摸点的状态  
		if((mode&0XF)&&((mode&0XF)<6))
		{
			temp=0XFF<<(mode&0XF);//将点的个数转换为1的位数,匹配tp_dev.sta定义 
			tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
			for(i=0;i<5;i++)
			{
				if(tp_dev.sta&(1<<i))	//触摸有效?
				{
					FT6336_RD_Reg(FT5206_TPX_TBL[i],buf,4);	//读取XY坐标值 
					if(tp_dev.touchtype&0X01)//横屏
					{
						tp_dev.y[i]=((uint16_t)(buf[0]&0X0F)<<8)+buf[1];
						tp_dev.x[i]=((uint16_t)(buf[2]&0X0F)<<8)+buf[3];
					}else
					{
						tp_dev.x[i]=(((uint16_t)(buf[0]&0X0F)<<8)+buf[1]);
						tp_dev.y[i]=((uint16_t)(buf[2]&0X0F)<<8)+buf[3];
					}  
					if((buf[0]&0XF0)!=0X80)tp_dev.x[i]=tp_dev.y[i]=0;//必须是contact事件，才认为有效
					//printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
				}			
			} 
			res=1;
			if(tp_dev.x[0]==0 && tp_dev.y[0]==0)mode=0;	//读到的数据都是0,则忽略此次数据
			t=0;		//触发一次,则会最少连续监测10次,从而提高命中率
		}
	}
	if((mode&0X1F)==0)//无触摸点按下
	{ 
		if(tp_dev.sta&TP_PRES_DOWN)	//之前是被按下的
		{
			tp_dev.sta&=~(1<<7);	//标记按键松开
		}else						//之前就没有被按下
		{ 
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
			tp_dev.sta&=0XE0;	//清除点有效标记	
		}	 
	} 	
	if(t>240)t=10;//重新从10开始计数
	return res;
}

uint16_t FT6336_get_x(void)
{
    return tp_dev.x[0];
}
uint16_t FT6336_get_y(void)
{
    return tp_dev.y[0];
}
