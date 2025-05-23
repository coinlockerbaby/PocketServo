/*
*********************************************************************************************************
*
*	模块名称 : LCD 中景园库
*	文件名称 : lcd.c
*	版    本 : V1.0
*	说    明 : 
*	修改记录 :
*		版本号  日期         作者      说明
*		V1.0    2025-03-08  astronG   正式发布
*
*	Copyright (C), 2020-2030, astronG
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "lcd.h"
#include "lcdfont.h"

/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
                xend,yend   终止坐标
								color       要填充的颜色
      返回值：  无
******************************************************************************/
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{          
	uint16_t i,j; 
	LCD_Address_Set(xsta,ysta,xend-1,yend-1);//设置显示范围
	for(i=ysta;i<yend;i++)
	{													   	 	
		for(j=xsta;j<xend;j++)
		{
			LCD_WR_DATA(color);
		}
	} 					  	    
}

/******************************************************************************
      函数说明：在指定位置画点
      入口数据：x,y 画点坐标
                color 点的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
	LCD_Address_Set(x,y,x,y);//设置光标位置 
	LCD_WR_DATA(color);
} 


/******************************************************************************
      函数说明：画线
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   线的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1;
	uRow=x1;//画线起点坐标
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向 
	else if (delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//水平线 
	else {incy=-1;delta_y=-delta_y;}
	if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		LCD_DrawPoint(uRow,uCol,color);//画点
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}


/******************************************************************************
      函数说明：画矩形
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   矩形的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}


/******************************************************************************
      函数说明：画圆
      入口数据：x0,y0   圆心坐标
                r       半径
                color   圆的颜色
      返回值：  无
******************************************************************************/
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t color)
{
	int a,b;
	a=0;b=r;	  
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);             //3           
		LCD_DrawPoint(x0+b,y0-a,color);             //0           
		LCD_DrawPoint(x0-a,y0+b,color);             //1                
		LCD_DrawPoint(x0-a,y0-b,color);             //2             
		LCD_DrawPoint(x0+b,y0+a,color);             //4               
		LCD_DrawPoint(x0+a,y0-b,color);             //5
		LCD_DrawPoint(x0+a,y0+b,color);             //6 
		LCD_DrawPoint(x0-b,y0+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//判断要画的点是否过远
		{
			b--;
		}
	}
}

/******************************************************************************
      函数说明：显示汉字串
      入口数据：x,y显示坐标
                *s 要显示的汉字串
                fc 字的颜色
                bc 字的背景色
                sizey 字号 可选 16 24 32
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	while(*s!=0)
	{
		if(sizey==12) LCD_ShowChinese12x12(x,y,s,fc,bc,sizey,mode);
		else if(sizey==16) LCD_ShowChinese16x16(x,y,s,fc,bc,sizey,mode);
		else if(sizey==24) LCD_ShowChinese24x24(x,y,s,fc,bc,sizey,mode);
		else if(sizey==32) LCD_ShowChinese32x32(x,y,s,fc,bc,sizey,mode);
		else return;
		s+=2;
		x+=sizey;
	}
}

/******************************************************************************
      函数说明：显示单个12x12汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese12x12(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//汉字数目
	uint16_t TypefaceNum;//一个字符所占字节大小
	uint16_t x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	                         
	HZnum=sizeof(tfont12)/sizeof(typFNT_GB12);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if((tfont12[k].Index[0]==*(s))&&(tfont12[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont12[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont12[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
} 

/******************************************************************************
      函数说明：显示单个16x16汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese16x16(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//汉字数目
	uint16_t TypefaceNum;//一个字符所占字节大小
	uint16_t x0=x;
  TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont16[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont16[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
} 


/******************************************************************************
      函数说明：显示单个24x24汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese24x24(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//汉字数目
	uint16_t TypefaceNum;//一个字符所占字节大小
	uint16_t x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont24[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont24[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
} 

/******************************************************************************
      函数说明：显示单个32x32汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChinese32x32(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//汉字数目
	uint16_t TypefaceNum;//一个字符所占字节大小
	uint16_t x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//非叠加方式
					{
						if(tfont32[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//叠加方式
					{
						if(tfont32[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//画一个点
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
	}
}


/******************************************************************************
      函数说明：显示单个字符
      入口数据：x,y显示坐标
                num 要显示的字符
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t temp,sizex,t,m=0;
	uint16_t i,TypefaceNum;//一个字符所占字节大小
	uint16_t x0=x;
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	num=num-' ';    //得到偏移后的值
	LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //设置光标位置 
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(sizey==12)temp=ascii_1206[num][i];		       //调用6x12字体
		else if(sizey==16)temp=ascii_1608[num][i];		 //调用8x16字体
		else if(sizey==24)temp=ascii_2412[num][i];		 //调用12x24字体
		else if(sizey==32)temp=ascii_3216[num][i];		 //调用16x32字体
		else return;
		for(t=0;t<8;t++)
		{
			if(!mode)//非叠加模式
			{
				if(temp&(0x01<<t))LCD_WR_DATA(fc);
				else LCD_WR_DATA(bc);
				m++;
				if(m%sizex==0)
				{
					m=0;
					break;
				}
			}
			else//叠加模式
			{
				if(temp&(0x01<<t))LCD_DrawPoint(x,y,fc);//画一个点
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y++;
					break;
				}
			}
		}
	}   	 	  
}


/******************************************************************************
      函数说明：显示字符串
      入口数据：x,y显示坐标
                *p 要显示的字符串
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{         
	while(*p!='\0')
	{       
		LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
		x+=sizey/2;
		p++;
	}  
}


/******************************************************************************
      函数说明：显示数字
      入口数据：m底数，n指数
      返回值：  无
******************************************************************************/
uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;
	return result;
}


/******************************************************************************
      函数说明：显示整数变量
      入口数据：x,y显示坐标
                num 要显示整数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LCD_ShowIntNum(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;
	uint8_t sizex=sizey/2;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+t*sizex,y,' ',fc,bc,sizey,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
} 

/**
************************************************************************
* @brief:      	LCD_ShowFloatNum: 在LCD上显示格式化的浮点数，支持负数
* @param:      	x - x坐标
*              	y - y坐标
*              	num - 要显示的浮点数
*              	len - 整数位数
*              	decimal - 小数位数
*              	fc - 字的颜色
*              	bc - 背景颜色
*              	sizey - 字体大小
* @retval:     	void
* @details:    	在LCD上显示格式化的浮点数，支持设置整数位数、小数位数、字体颜色、背景颜色和字体大小。
************************************************************************
**/
void LCD_ShowFloatNum(uint16_t x, uint16_t y, float num, uint8_t len, uint8_t decimal, uint16_t fc, uint16_t bc, uint8_t sizey)
{
	int16_t num_int;
	uint8_t t, temp, sizex;
	sizex = sizey / 2;
	num_int = num * mypow(10, decimal);

	if (num < 0)
	{
		LCD_ShowChar(x, y, '-', fc, bc, sizey, 0);
		num_int = -num_int;
		x += sizex;
		len++;
	}
	else
	{
		LCD_ShowChar(x, y, ' ', fc, bc, sizey, 0);
		num_int = num_int;
		x += sizex;
		len++;
	}

	// 在更新数字时刷新显示的位置
	LCD_Fill(x, y, x + len * sizex + decimal + 1, y + sizey + 1, bc);

	for (t = 0; t < len; t++)
	{
		if (t == (len - decimal))
		{
			LCD_ShowChar(x + (len - decimal) * sizex, y, '.', fc, bc, sizey, 0);
			t++;
			len += 1;
		}
		temp = ((num_int / mypow(10, len - t - 1)) % 10) + '0';
		LCD_ShowChar(x + t * sizex, y, temp, fc, bc, sizey, 0);
	}
}

/**
************************************************************************
* @brief:      	LCD_ShowFloatNum: 在LCD上显示格式化的浮点数，不支持负数
* @param:      	x - x坐标
*              	y - y坐标
*              	num - 要显示的浮点数
*              	len - 整数位数
*              	decimal - 小数位数
*              	fc - 字的颜色
*              	bc - 背景颜色
*              	sizey - 字体大小
* @retval:     	void
* @details:    	在LCD上显示格式化的浮点数，支持设置整数位数、小数位数、字体颜色、背景颜色和字体大小。
************************************************************************
**/
void LCD_ShowFloatNum1(uint16_t x, uint16_t y, float num, uint8_t len, uint8_t decimal, uint16_t fc, uint16_t bc, uint8_t sizey)
{
	int16_t num_int;
	uint8_t t, temp, sizex;
	sizex = sizey / 2;
	num_int = num * mypow(10, decimal);


	num_int = num_int;
	x += sizex;
	len++;

	// 在更新数字时刷新显示的位置
	LCD_Fill(x, y, x + len * sizex + decimal + 1, y + sizey + 1, bc);

	for (t = 0; t < len; t++)
	{
		if (t == (len - decimal))
		{
			LCD_ShowChar(x + (len - decimal) * sizex, y, '.', fc, bc, sizey, 0);
			t++;
			len += 1;
		}
		temp = ((num_int / mypow(10, len - t - 1)) % 10) + '0';
		LCD_ShowChar(x + t * sizex, y, temp, fc, bc, sizey, 0);
	}
}


/******************************************************************************
      函数说明：显示图片
      入口数据：x,y起点坐标
                length 图片长度
                width  图片宽度
                pic[]  图片数组    
      返回值：  无
******************************************************************************/
void LCD_ShowPicture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[])
{
	uint16_t i,j;
	uint32_t k=0;
	LCD_Address_Set(x,y,x+length-1,y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			LCD_WR_DATA8(pic[k*2]);
			LCD_WR_DATA8(pic[k*2+1]);
			k++;
		}
	}			
}

/*
*********************************************************************************************************
*	函 数 名: LCD_Demo
*	功能说明: LCD演示函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_Demo(void)
{
	if (Foc.is_armed == !TRUE)
	{
		LCD_ShowString(10, 5,(uint8_t *)"[ERROR]", BRRED, BLACK, 32, 0);
	}
	else
	{
		LCD_ShowString(10, 5,(uint8_t *)"[READY]", GREEN, BLACK, 32, 0);
	}
	
	
	LCD_ShowString(130, 5,(uint8_t *)"Vbus:", WHITE, BLACK, 16, 0);
	LCD_ShowFloatNum(170, 5, 24.12, 3, 2, GREEN, BLACK, 16);
	LCD_ShowString(220, 5, (uint8_t *)"V", WHITE, BLACK, 16, 0);
	
	LCD_ShowString(130, 21,(uint8_t *)"Ibus:", WHITE, BLACK, 16, 0);
	LCD_ShowFloatNum(170, 21, 00.00, 3, 2, GREEN, BLACK, 16);
	LCD_ShowString(220, 21, (uint8_t *)"A", WHITE, BLACK, 16, 0);
	
	LCD_ShowString(130, 37,(uint8_t *)"Tmos:", WHITE, BLACK, 16, 0);
	LCD_ShowFloatNum(170, 37, 31.27, 3, 2, GREEN, BLACK, 16);
	LCD_ShowString(220, 37, (uint8_t *)"C", WHITE, BLACK, 16, 0);
	
	LCD_DrawLine(10, 60, 10, 135, WHITE);
	LCD_DrawLine(230, 60, 230, 135, WHITE);
	
	LCD_ShowString(20, 40, (uint8_t *)"Mode :", WHITE, BLACK, 16, 0);
	LCD_ShowString(20, 60, (uint8_t *)"Iq.  :", WHITE, BLACK, 16, 0);
	LCD_ShowString(20, 80, (uint8_t *)"Vel. :", WHITE, BLACK, 16, 0);
	LCD_ShowString(20, 100,(uint8_t *)"Pos. :", WHITE, BLACK, 16, 0);
	LCD_ShowString(20, 120,(uint8_t *)"Lim. :", WHITE, BLACK, 16, 0);
	
	switch (UsrConfig.control_mode)
	{
		case CONTROL_MODE_TORQUE_RAMP:
			LCD_ShowString(76, 40, (uint8_t *)"[Tor.]", WHITE, BLACK, 16, 0);
		break;
		case CONTROL_MODE_VELOCITY_RAMP:
			LCD_ShowString(76, 40, (uint8_t *)"[Vel.]", WHITE, BLACK, 16, 0);
		break;
		case CONTROL_MODE_POSITION_RAMP:
			LCD_ShowString(76, 40, (uint8_t *)"[Pos.]", WHITE, BLACK, 16, 0);
		break;
		case CONTROL_MODE_POSITION_PROFILE:
			LCD_ShowString(76, 40, (uint8_t *)"[PV.]", WHITE, BLACK, 16, 0);
		break;
		default:
			break;
	}

	/* Iq */
	LCD_ShowFloatNum(68, 60, Foc.i_q_filt, 3, 2, GREEN, BLACK, 16);
	LCD_ShowString(124, 60, (uint8_t *)"->", WHITE, BLACK, 16, 0);
	if (UsrConfig.control_mode == CONTROL_MODE_TORQUE_RAMP)
	{
		LCD_ShowFloatNum(140, 60, Foc.target_i_q, 3, 2, BRRED, BLACK, 16);
	}
	else
	{
		LCD_ShowFloatNum(140, 60, Foc.target_i_q, 3, 2, GREEN, BLACK, 16);
	}
	LCD_ShowString(196, 60, (uint8_t *)"A", WHITE, BLACK, 16, 0);
	/* Vel */
	LCD_ShowFloatNum(68, 80, Foc.velocity_filt, 4, 0, GREEN, BLACK, 16);
	LCD_ShowString(124, 80, (uint8_t *)"->", WHITE, BLACK, 16, 0);
	if (UsrConfig.control_mode == CONTROL_MODE_VELOCITY_RAMP)
	{
		LCD_ShowFloatNum(140, 80, Foc.target_velocity, 4, 0, BRRED, BLACK, 16);
	}
	else
	{
		LCD_ShowFloatNum(140, 80, Foc.target_velocity, 4, 0, GREEN, BLACK, 16);
	}
	LCD_ShowString(196, 80, (uint8_t *)"r/s", WHITE, BLACK, 16, 0);
	/* Pos */
	LCD_ShowFloatNum(68, 100, Foc.position, 4, 0, GREEN, BLACK, 16);
	LCD_ShowString(124, 100, (uint8_t *)"->", WHITE, BLACK, 16, 0);
	if (UsrConfig.control_mode == CONTROL_MODE_POSITION_RAMP || UsrConfig.control_mode == CONTROL_MODE_POSITION_PROFILE)
	{
		LCD_ShowFloatNum(140, 100, Foc.target_position, 4, 0, BRRED, BLACK, 16);
	}
	else
	{
		LCD_ShowFloatNum(140, 100, Foc.target_position, 4, 0, GREEN, BLACK, 16);
	}
	LCD_ShowString(196, 100, (uint8_t *)"rad", WHITE, BLACK, 16, 0);
	/* Lim */
	switch (UsrConfig.control_mode)
	{
		case CONTROL_MODE_TORQUE_RAMP:
			LCD_ShowFloatNum(68, 120, Foc.pid_i_q.output_prev, 3, 2, GREEN, BLACK, 16);
			LCD_ShowString(124, 120, (uint8_t *)"->", WHITE, BLACK, 16, 0);
			LCD_ShowFloatNum(140, 120, Foc.pid_i_q.limit, 3, 2, GREEN, BLACK, 16);
			LCD_ShowString(196, 120, (uint8_t *)"V", WHITE, BLACK, 16, 0);
		break;
		case CONTROL_MODE_VELOCITY_RAMP:
			LCD_ShowFloatNum(68, 120, Foc.i_q_filt, 3, 2, GREEN, BLACK, 16);
			LCD_ShowString(124, 120, (uint8_t *)"->", WHITE, BLACK, 16, 0);
			LCD_ShowFloatNum(140, 120, Foc.pid_velocity.limit, 3, 2, GREEN, BLACK, 16);
			LCD_ShowString(196, 120, (uint8_t *)"A", WHITE, BLACK, 16, 0);
		break;
		case CONTROL_MODE_POSITION_RAMP:
			LCD_ShowFloatNum(68, 120, Foc.i_q_filt, 3, 2, GREEN, BLACK, 16);
			LCD_ShowString(124, 120, (uint8_t *)"->", WHITE, BLACK, 16, 0);
			LCD_ShowFloatNum(140, 120, Foc.pid_position.limit, 3, 2, GREEN, BLACK, 16);
			LCD_ShowString(196, 120, (uint8_t *)"A", WHITE, BLACK, 16, 0);
		break;
		case CONTROL_MODE_POSITION_PROFILE:
			LCD_ShowFloatNum(68, 120, Foc.i_q_filt, 3, 2, GREEN, BLACK, 16);
			LCD_ShowString(124, 120, (uint8_t *)"->", WHITE, BLACK, 16, 0);
			LCD_ShowFloatNum(140, 120, Foc.pid_velocity.limit, 3, 2, GREEN, BLACK, 16);
			LCD_ShowString(196, 120, (uint8_t *)"A", WHITE, BLACK, 16, 0);
		break;
		default:
			break;
	}
}


/*
*********************************************************************************************************
*	函 数 名: LCD_Disable
*	功能说明: 关闭LCD(转换到Ecat通信模式)
*	形 参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_Disable(void)
{
	LCD_ShowString(10, 5,(uint8_t *)"[EtherCAT]", GREEN, BLACK, 32, 0);
}


/*
*********************************************************************************************************
*	函 数 名: LCD_Draw_FOC_Calibration
*	功能说明: 显示FOC校准
*	形 参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_Draw_FOC_Calibration(void)
{
	LCD_ShowString(10, 5,(uint8_t *)"Calibration...", GREEN, BLACK, 32, 0);
}
