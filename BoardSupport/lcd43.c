/*******************************************************************************
 * 百为技术有限公司 		Copyright(C) 2013, baiweijishu 
 * 项目名称(Project):			
 * 文件名(File):				lcd.c
 *
 * 作者(Author):				张锡良 (xi_iang)
 * 电邮(Email):					zhang.xiliang@163.com
 * 技术支持论坛(Forum)：www.stm32bbs.com
 * 淘宝官方网店(Shop):  http://baiweijishu.taobao.com
 * 修改日期(Date): 			2013-11-29		
 *******************************************************************************/
#include "lpc177x_8x_lcd.h"
#include "lpc177x_8x_pinsel.h"
#include "lcd43.h"
#include "sdram.h"


/*******************************************************************************
 * 函数名(Function):	lpc1788_Lcd_Init
 * 参数(Param):				无 							
 * 返回值(Return):		无
 * 描述(Description):	LCD初始化函数
 *******************************************************************************/
void lpc1788_Lcd_Init(void)
{
	LCD_Config_Type lcd_config;

	/* 初始化SDRAM */
	lpc1788_SDRAM_Init();
  lpc1788_SDRAM_Clean();

	/* 关闭LCD控制 */
	LCD_Enable (FALSE);

 	/* 配置LCD参数 */
  lcd_config.big_endian_byte = 0;
  lcd_config.big_endian_pixel = 0;
  lcd_config.hConfig.hbp = LCD_H_BACK_PORCH;
  lcd_config.hConfig.hfp = LCD_H_FRONT_PORCH;
  lcd_config.hConfig.hsw = LCD_H_PULSE;
  lcd_config.hConfig.ppl = LCD_H_SIZE;
  lcd_config.vConfig.lpp = LCD_V_SIZE;
  lcd_config.vConfig.vbp = LCD_V_BACK_PORCH;
  lcd_config.vConfig.vfp = LCD_V_FRONT_PORCH;
  lcd_config.vConfig.vsw = LCD_V_PULSE;
  lcd_config.panel_clk   = LCD_PIX_CLK;
  lcd_config.polarity.active_high = 1;
  lcd_config.polarity.cpl = LCD_H_SIZE;
  lcd_config.polarity.invert_hsync = 1;
  lcd_config.polarity.invert_vsync = 1;
  lcd_config.polarity.invert_panel_clock = 0;
  lcd_config.lcd_panel_upper =  LCD_VRAM_BASE_ADDR_UPPER;
  lcd_config.lcd_panel_lower =  LCD_VRAM_BASE_ADDR_LOWER;
  lcd_config.lcd_bpp = LCD_BPP_16_565Mode;
  lcd_config.lcd_type = LCD_TFT;
	lcd_config.lcd_palette = NULL;
  lcd_config.lcd_bgr = FALSE;
	LCD_Initialize (&lcd_config);

//  LCD_SetImage(LCD_PANEL_UPPER, NULL);
//  LCD_SetImage(LCD_PANEL_LOWER, NULL);

	/* LCD使能 */
	LCD_Enable (TRUE);


}

/*******************************************************************************
 * 函数名(Function):	LCD_Clear
 * 参数(Param):				panel
 *                    Color							
 * 返回值(Return):		无
 * 描述(Description):	LCD清屏
 *******************************************************************************/
void LCD_Clear(LCD_PANEL panel, int Color)
{
	int i,j;
	uint16_t * pBuffer;
	if(panel == LCD_PANEL_UPPER)
		pBuffer = (uint16_t *)LCD_VRAM_BASE_ADDR_UPPER;
// 	else 
// 		pBuffer = (uint16_t *)LCD_VRAM_BASE_ADDR_LOWER;	 
	for(i = 0; i < LCD_H_SIZE; i++)
	{
		for(j = 0;j < LCD_V_SIZE; j++)
		{
			pBuffer[j*LCD_H_SIZE + i] = Color; 
		}
	}
}

/*******************************************************************************
 * 函数名(Function):	LCD_SetPixel
 * 参数(Param):				panel
 *                    x
 *                    y	
 *                    Color							
 * 返回值(Return):		无
 * 描述(Description):	LCD画点函数
 *******************************************************************************/
void LCD_SetPixel(LCD_PANEL panel, int x, int y, int Color)
{
	uint16_t * pBuffer;
	if(panel == LCD_PANEL_UPPER)
		pBuffer = (uint16_t *)LCD_VRAM_BASE_ADDR_UPPER;
// 	else 
// 		pBuffer = (uint16_t *)LCD_VRAM_BASE_ADDR_LOWER;	 

	pBuffer[y*LCD_H_SIZE + x] = Color; 

}

/*******************************************************************************
 * 函数名(Function):	LCD_GetPixel
 * 参数(Param):				panel
 *                    x
 *                    y	
 *                    Color							
 * 返回值(Return):		读回的像素值
 * 描述(Description):	LCD画点函数
 *******************************************************************************/
uint16_t LCD_GetPixel(LCD_PANEL panel, int x, int y)
{
	uint16_t * pBuffer;
	if(panel == LCD_PANEL_UPPER)
		pBuffer = (uint16_t *)LCD_VRAM_BASE_ADDR_UPPER;
// 	else 
// 		pBuffer = (uint16_t *)LCD_VRAM_BASE_ADDR_LOWER;	 

	return pBuffer[y*LCD_H_SIZE + x]; 

}


/*******************************************************************************
 * 函数名(Function):	LCD_test
 * 参数(Param):				无 							
 * 返回值(Return):		无
 * 描述(Description):	LCD测试
 *******************************************************************************/
void LCD_test(void)
{
	int x, y;
	/* 初始化LCD */
	lpc1788_Lcd_Init();

//	/* LCD清屏 */
//	LCD_Clear(LCD_PANEL_UPPER, WHITE);
//	LCD_Clear(LCD_PANEL_LOWER, WHITE);

//	/* 填充矩形 */
//	LCD_FillRectangle (LCD_PANEL_UPPER, 10, 100, 10, 100, GREEN);
//	LCD_FillRectangle (LCD_PANEL_UPPER, 100, 200, 100, 200, RED);
//	LCD_FillRectangle (LCD_PANEL_UPPER, 200, 300, 100, 200, BLUE);

//	for(x=350;x<400;x++)
//		for(y=100;y<200;y++)
//			LCD_PutPixel(LCD_PANEL_UPPER,x,y,RED);
}

/************************************* End *************************************/
