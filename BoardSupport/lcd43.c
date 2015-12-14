/*******************************************************************************
 * °ÙÎª¼¼ÊõÓÐÏÞ¹«Ë¾ 		Copyright(C) 2013, baiweijishu 
 * ÏîÄ¿Ãû³Æ(Project):			
 * ÎÄ¼þÃû(File):				lcd.c
 *
 * ×÷Õß(Author):				ÕÅÎýÁ¼ (xi_iang)
 * µçÓÊ(Email):					zhang.xiliang@163.com
 * ¼¼ÊõÖ§³ÖÂÛÌ³(Forum)£ºwww.stm32bbs.com
 * ÌÔ±¦¹Ù·½Íøµê(Shop):  http://baiweijishu.taobao.com
 * ÐÞ¸ÄÈÕÆÚ(Date): 			2013-11-29		
 *******************************************************************************/
#include "lpc177x_8x_lcd.h"
#include "lpc177x_8x_pinsel.h"
#include "lcd43.h"
#include "sdram.h"


/*******************************************************************************
 * º¯ÊýÃû(Function):	lpc1788_Lcd_Init
 * ²ÎÊý(Param):				ÎÞ 							
 * ·µ»ØÖµ(Return):		ÎÞ
 * ÃèÊö(Description):	LCD³õÊ¼»¯º¯Êý
 *******************************************************************************/
void lpc1788_Lcd_Init(void)
{
	LCD_Config_Type lcd_config;

	/* Ú˜Ò•LCDà ˜×† */
	LCD_Enable (FALSE);
 
	/* ³õÊ¼»¯SDRAM */
	lpc1788_SDRAM_Init();

 lpc1788_SDRAM_Clean();
// lpc1788_SDRAM_Test();


 	/* ÅäÖÃLCD²ÎÊý */
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

	/* LCDÊ¹ÄÜ */
	LCD_Enable (TRUE);


}

/*******************************************************************************
 * º¯ÊýÃû(Function):	LCD_Clear
 * ²ÎÊý(Param):				panel
 *                    Color							
 * ·µ»ØÖµ(Return):		ÎÞ
 * ÃèÊö(Description):	LCDÇåÆÁ
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
 * º¯ÊýÃû(Function):	LCD_SetPixel
 * ²ÎÊý(Param):				panel
 *                    x
 *                    y	
 *                    Color							
 * ·µ»ØÖµ(Return):		ÎÞ
 * ÃèÊö(Description):	LCD»­µãº¯Êý
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
 * º¯ÊýÃû(Function):	LCD_GetPixel
 * ²ÎÊý(Param):				panel
 *                    x
 *                    y	
 *                    Color							
 * ·µ»ØÖµ(Return):		¶Á»ØµÄÏñËØÖµ
 * ÃèÊö(Description):	LCD»­µãº¯Êý
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
 * º¯ÊýÃû(Function):	LCD_test
 * ²ÎÊý(Param):				ÎÞ 							
 * ·µ»ØÖµ(Return):		ÎÞ
 * ÃèÊö(Description):	LCD²âÊÔ
 *******************************************************************************/
void LCD_test(void)
{
	int x, y;
	/* ³õÊ¼»¯LCD */
	lpc1788_Lcd_Init();

//	/* LCDÇåÆÁ */
//	LCD_Clear(LCD_PANEL_UPPER, WHITE);
//	LCD_Clear(LCD_PANEL_LOWER, WHITE);

//	/* Ìî³ä¾ØÐÎ */
//	LCD_FillRectangle (LCD_PANEL_UPPER, 10, 100, 10, 100, GREEN);
//	LCD_FillRectangle (LCD_PANEL_UPPER, 100, 200, 100, 200, RED);
//	LCD_FillRectangle (LCD_PANEL_UPPER, 200, 300, 100, 200, BLUE);

//	for(x=350;x<400;x++)
//		for(y=100;y<200;y++)
//			LCD_PutPixel(LCD_PANEL_UPPER,x,y,RED);
}

/************************************* End *************************************/
