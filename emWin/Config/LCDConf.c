/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2012  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.20 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : LCDConf.c
Purpose     : Display controller configuration (single layer)
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"
#include <stddef.h>

#ifndef _WINDOWS
  #include "GUIDRV_Lin.h"
  #include "LPC177x_8x.h"
  #include "system_LPC177x_8x.h"
  #include "LCDConf.h"
#endif

/*********************************************************************
*
*       Layer configuration (to be modified)
*
**********************************************************************
*/

//
// Display framebuffer size
// These values can be reduced if using a smaller display and might have
// to be adjusted if using virtual screens. The values will be checked
// during emWin init.
//
#define FB_XSIZE  800
#define FB_YSIZE  480

//
// Physical display size
//#define USE_LCD_800_480
#define USE_LCD_480_272
//#define USE_LCD_240_320
//

#ifdef USE_LCD_480_272
  #define XSIZE_PHYS  800
  #define YSIZE_PHYS  480
#else
  #define XSIZE_PHYS  FB_XSIZE
  #define YSIZE_PHYS  FB_YSIZE
#endif

//
// Virtual display size
//
#define VXSIZE_PHYS (XSIZE_PHYS)
#define VYSIZE_PHYS (YSIZE_PHYS)

//
// Color conversion
//
#define COLOR_CONVERSION  GUICC_M565

//
// Pixel width in bytes
//
#define PIXEL_WIDTH  2

//
// Display driver
//
#define DISPLAY_DRIVER_TRULY  &GUIDRV_Lin_OSX_16_API
#define DISPLAY_DRIVER_OTHER  &GUIDRV_Lin_16_API

//
// Video RAM address
//
#define VRAM_ADDR_PHYS  (U32)&_aVRAM[0]


/*********************************************************************
*
*       Configuration checking
*
**********************************************************************
*/
#ifndef   VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
#endif
#ifndef   VRAM_ADDR_PHYS
  #define VRAM_ADDR_PHYS  0
#endif
#ifndef   VRAM_ADDR_VIRT
  #define VRAM_ADDR_VIRT  0
#endif

#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   COLOR_CONVERSION
  #error Color conversion not defined!
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

//
// LCD types
//
enum {
  DISPLAY_TRULY_240_320,
  DISPLAY_BOARD_480_272,
  DISPLAY_BOARD_800_480
};

#ifdef USE_LCD_480_272


#define HBP_480_272			46       // Horizontal back porch in clocks
#define HFP_480_272			80        // Horizontal front porch in clocks
#define HSW_480_272			1        // HSYNC pulse width in clocks
#define PPL_480_272			800      // Pixels per line
#define VBP_480_272			21        // Vertical back porch in clocks
#define VFP_480_272			21        // Vertical front porch in clocks
#define VSW_480_272			2        // VSYNC pulse width in clocks
#define LPP_480_272			480      // Lines per panel
#define IOE_480_272			0        // Invert output enable, 1 = invert
#define IPC_480_272			0        // Invert panel clock, 1 = invert
#define IHS_480_272			1        // Invert HSYNC, 1 = invert
#define IVS_480_272			1        // Invert VSYNC, 1 = invert
#define ACB_480_272			1        // AC bias frequency in clocks (not used)
#define BPP_480_272			16       // Bits per pixel b110 = 16 bpp 5:6:5 mode
#define CLK_480_272			33000000  // Optimal clock rate (Hz) between 1-8.22 MHz according to SSD1289 datasheet
#define LCD_480_272			1        // Panel type; 1: LCD TFT panel
#define DUAL_480_272		0        // No dual panel
#endif


//
// LCDC
//
#define LCDC_BPP_16_565    6     // Bits per pixel b110 = 16 bpp 5:6:5 mode
#define LCDC_TFT           1     // TFT value for LCDC

#define BGR                1     // Swap red and blue

#define BCD_BIT            26    // Bypass pixel clock divider
#define LCD_BACKLIGHT_BIT  1    // P2.1
// #define LCD_REG_BIT        19    // P0.19

// #define LCD_DATA()         (LPC_GPIO0->SET = (1 << LCD_REG_BIT))
// #define LCD_CMD()          (LPC_GPIO0->CLR = (1 << LCD_REG_BIT))

//
// I2C
//
#define I2C0_BASE_ADDR  0x4001C000

//
// PCA9532 (I2C)
//
#define PCA9532_I2C_ADDR         0x64  // Address of the PCA9532 on the display PCB
#define PCA9532_DISP_3V3_EN_BIT  0
#define PCA9532_DISP_5V_EN_BIT   1
#define PCA9532_DISP_EN_BIT      4
#define PCA9532_DISP_BL_BIT      8

//
// EEPROM (I2C)
//
#define EEPROM_I2C_ADDR         0x56  // Address of the EEPROM that holds the display configurations
#define EEPROM_MAGIC_NUMBER     0xEA01CDAE
#define EEPROM_LCD_CONFIG_OFFS  64
#define EEPROM_MAX_SEQ_VER      1

//
// SSP
//
#define SSPSR_TNF      (1 << 1)  // Transmit FIFO not full
#define SSPSR_RNE      (1 << 2)  // Receive FIFO not empty
#define SSPSR_BSY      (1 << 4)  // Busy
#define SSP_CLOCK      500000
#define SSP_FIFO_SIZE  8

#define SSP_MODE_LCD   0
#define SSP_MODE_TS    1

//
// Touch screen
//
#define TOUCH_CS_BIT                   23
#define TS_CS_SET()                    LPC_GPIO2->CLR = (1 << TOUCH_CS_BIT)
#define TS_CS_CLR()                    LPC_GPIO2->SET = (1 << TOUCH_CS_BIT)

#define ADS_START                      (1 << 7)
#define ADS_A2A1A0_d_y                 (1 << 4)  // Differential
#define ADS_A2A1A0_d_z1                (3 << 4)  // Differential
#define ADS_A2A1A0_d_z2                (4 << 4)  // Differential
#define ADS_A2A1A0_d_x                 (5 << 4)  // Differential
#define ADS_A2A1A0_vaux                (6 << 4)  // Non-differential
#define ADS_12_BIT                     (0 << 3)
#define ADS_SER                        (1 << 2)  // Non-differential
#define ADS_DFR                        (0 << 2)  // Differential
#define ADS_PD10_ADC_ON                (1 << 0)  // ADC on
#define ADS_PD10_REF_ON                (2 << 0)  // vREF on + penirq
#define ADS_PD10_ALL_ON                (3 << 0)  // ADC + vREF on

#define READ_12BIT_DFR(D, ADC, VREF)  (ADS_START                    | \
                                       D                            | \
                                       ADS_12_BIT                   | \
                                       ADS_DFR                      | \
                                       (ADC  ? ADS_PD10_ADC_ON : 0) | \
                                       (VREF ? ADS_PD10_REF_ON : 0))


#define READ_12BIT_SER(x)              (ADS_START  | /* Single-ended samples need to power   */ \
                                        x          | /* up reference voltage first; therefor */ \
                                        ADS_12_BIT | /* we leave both VREF and ADC powered.  */ \
                                        ADS_SER)

#define REF_ON                         (READ_12BIT_DFR(ADS_A2A1A0_d_x,  1, 1))
#define READ_Y(VREF)                   (READ_12BIT_DFR(ADS_A2A1A0_d_y,  1, VREF))
#define READ_Z1(VREF)                  (READ_12BIT_DFR(ADS_A2A1A0_d_z1, 1, VREF))
#define READ_Z2(VREF)                  (READ_12BIT_DFR(ADS_A2A1A0_d_z2, 1, VREF))
#define READ_X(VREF)                   (READ_12BIT_DFR(ADS_A2A1A0_d_x,  1, VREF))
#define PWRDOWN                        (READ_12BIT_DFR(ADS_A2A1A0_d_y,  0, 0))

#define TS_DEBOUNCE_MAX                1
#define TS_DEBOUNCE_TOL                3

#ifndef   ABS
  #define ABS(x)                       (((int32_t)(x)) < 0 ? (-x) : (x))
#endif

/*********************************************************************
*
*       Defines, sfrs
*
**********************************************************************
*/

#define MATRIX_ARB  (*(volatile U32*)(0x400FC188))

/*********************************************************************
*
*       Types
*
**********************************************************************
*/

typedef struct {
  U8  HBP;   // Horizontal back porch in clocks
  U8  HFP;   // Horizontal front porch in clocks
  U8  HSW;   // HSYNC pulse width in clocks
  U16 PPL;   // Pixels per line
  U8  VBP;   // Vertical back porch in clocks
  U8  VFP;   // Vertical front porch in clocks
  U8  VSW;   // VSYNC pulse width in clocks
  U16 LPP;   // Lines per panel
  U8  IOE;   // Invert output enable, 1 = invert
  U8  IPC;   // Invert panel clock, 1 = invert
  U8  IHS;   // Invert HSYNC, 1 = invert
  U8  IVS;   // Invert VSYNC, 1 = invert
  U8  ACB;   // AC bias frequency in clocks (not used)
  U8  BPP;   // Maximum bits per pixel the display supports
  U32 Clk;   // Optimal clock rate (Hz)
  U8  LCD;   // LCD panel type
  U8  Dual;  // Dual panel, 1 = dual panel display
} LCD_PARAM;

typedef struct {
  U16 LcdParams;
  U16 LcdInit;
  U16 PowerDown;
  U16 Touch;
  U16 End;
} EEPROM_CONFIG_OFFS;

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

//
// LCD
//
//static EEPROM_CONFIG_OFFS _EepromConfigOffs;
static LCD_PARAM          _LcdParams;
//static U8                 _acBuffer[256];  // Buffer to read EEPROM initstring
static U8                 _Display;

//
// Video RAM
//
#ifdef __ICCARM__
  #pragma data_alignment=8  // 8 byte align frame buffer
  #pragma location="VRAM"
  static __no_init U32 _aVRAM[FB_XSIZE * FB_YSIZE / (4 / PIXEL_WIDTH)];
#endif
#ifdef __CC_ARM
  __align(8) static U32 _aVRAM[FB_XSIZE * FB_YSIZE / (4 / PIXEL_WIDTH)] __attribute__ ((section ("VRAM"), zero_init));
#endif
#ifdef __GNUC__
  U32 static _aVRAM[FB_XSIZE * FB_YSIZE / (4 / PIXEL_WIDTH)]  __attribute__ ((section(".VRAM"))) = { 0 };
#endif


/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/

/*********************************************************************
*
*       _SetDisplayOrigin()
*/
#ifndef _WINDOWS
static void _SetDisplayOrigin(int x, int y) {
  (void)x;
  //
  // Set start address for display data and enable LCD controller
  //
  LPC_LCD->UPBASE = VRAM_ADDR_PHYS + (y * YSIZE_PHYS * PIXEL_WIDTH);  // Needs to be set, before LCDC is enabled
}
#endif

/*********************************************************************
*
*       _FindClockDivisor
*
* Function description
*   Find closest clock divider to get the desired clock rate.
*/
#ifndef _WINDOWS
static U32 _FindClockDivisor(U32 TargetClock) {
  U32 Divider;
  U32 r;

  Divider = 1;
  while (((SystemCoreClock / Divider) > TargetClock) && (Divider <= 0x3F)) {
    Divider++;
  }
  if (Divider <= 1) {
    r = (1 << BCD_BIT);  // Skip divider logic if clock divider is 1
  } else {
    //
    // Use found divider
    //
    Divider -= 2;
    r = 0
        | (((Divider >> 0) & 0x1F)
        | (((Divider >> 5) & 0x1F) << 27))
        ;
  }
  return r;
}
#endif

/*********************************************************************
*
*       _InitLCDC
*
* Function description
*   Inits the LCD controller, backlight and sets the frame buffer.
*/
#ifndef _WINDOWS
static void _InitLCDC(void) {
  U32 i;

  //
  // Translate and check params from EEPROM values to device values
  //
  if (_LcdParams.BPP == 16) {
    _LcdParams.BPP = LCDC_BPP_16_565;
  } else {
    while (1);  // Error, display mode not yet supported
  }
//  if (_LcdParams.LCD == 0) {
//    _LcdParams.LCD = LCDC_TFT;
//  } else {
//    while (1);  // Error, display type not yet supported
//  }
  //
  // Init LCDC
  //
  LPC_SC->PCONP |=  (1UL << 0);  // Power the LCDC
  LPC_LCD->CTRL &= ~(1UL << 0);  // Disable the LCDC
  LPC_LCD->TIMH  = 0             // Configure horizontal axis
                   | ((((U32)_LcdParams.PPL / 16) - 1) <<  2)
                   |  (((U32)_LcdParams.HSW - 1)       <<  8)
                   |  (((U32)_LcdParams.HFP - 1)       << 16)
                   |  (((U32)_LcdParams.HBP - 1)       << 24)
                   ;
  LPC_LCD->TIMV  = 0             // Configure vertical axis
                   | (((U32)_LcdParams.LPP - 1) <<  0)
                   | (((U32)_LcdParams.VSW - 1) << 10)
                   | (((U32)_LcdParams.VFP)     << 16)
                   | (((U32)_LcdParams.VBP)     << 24)
                   ;
  LPC_LCD->POL   = 0             // Configure clock and signal polarity
                   | (_FindClockDivisor(_LcdParams.Clk) <<  0)
                   | (((U32)_LcdParams.ACB - 1)         <<  6)
                   | (((U32)_LcdParams.IVS)             << 11)
                   | (((U32)_LcdParams.IHS)             << 12)
                   | (((U32)_LcdParams.IPC)             << 13)
                   | (((U32)_LcdParams.PPL - 1)         << 16)
                   ;
  LPC_LCD->CTRL  = 0             // Configure operating mode and panel parameters
                   | ((U32)_LcdParams.BPP << 1)
                   | ((U32)BGR            << 8)
                   | ((U32)_LcdParams.LCD << 5)
                   ;
  for (i = 0; i < GUI_COUNTOF(LPC_LCD->PAL); i++) {
    LPC_LCD->PAL[i] = 0;  // Clear the color palette with black
  }
  LPC_SC->LCD_CFG = 0x0;  // No panel clock prescaler
  //
  // Enable LCDC
  //
  LPC_LCD->UPBASE  = VRAM_ADDR_PHYS;
  LPC_LCD->CTRL   |= (1 <<  0);                 // Enable LCD signals
  LPC_LCD->CTRL   |= (1 << 11);                 // Enable LCD power
  LPC_GPIO2->SET  |= (1 << LCD_BACKLIGHT_BIT);  // Set backlight to on
}
#endif

/*********************************************************************
*
*       _InitLCD
*
* Function description
*   Initializes the port pins as needed for the LCD and selects
*   an available LCD.
*/
#ifndef _WINDOWS
static void _InitLCD(void) {
//  _InitLCDPorts();
  //
  // Setup BUS priorities
  //
  MATRIX_ARB = 0            // Set AHB Matrix priorities [0..3] with 3 being highest priority
               | (1 <<  0)  // PRI_ICODE : I-Code bus priority. Should be lower than PRI_DCODE for proper operation.
               | (2 <<  2)  // PRI_DCODE : D-Code bus priority.
               | (0 <<  4)  // PRI_SYS   : System bus priority.
               | (0 <<  6)  // PRI_GPDMA : General Purpose DMA controller priority.
               | (0 <<  8)  // PRI_ETH   : Ethernet: DMA priority.
               | (3 << 10)  // PRI_LCD   : LCD DMA priority.
               | (0 << 12)  // PRI_USB   : USB DMA priority.
               ; //可以不要

	_InitLCDC(); //保留,否则底色变红

}
#endif

/*********************************************************************
*
*       _InitController
*
* Function description
*   Initializes the LCD controller and touch screen
*/
#ifndef _WINDOWS
static void _InitController(unsigned LayerIndex) {
  _InitLCD();  //注释掉不影响功能
  //
  // Set display size and video-RAM address
  //
  LCD_SetSizeEx (XSIZE_PHYS, YSIZE_PHYS, LayerIndex);   //这三句不能删
  LCD_SetVSizeEx(VXSIZE_PHYS, VYSIZE_PHYS, LayerIndex);
  LCD_SetVRAMAddrEx(LayerIndex, (void*)VRAM_ADDR_PHYS);

}
#endif

/*********************************************************************
*
*       _UpdateDisplayConfig
*
* Function description
*   Checks for attached display and updates the LCDC parameters.
*/
#ifndef _WINDOWS
static void _UpdateDisplayConfig(void) {

#ifdef USE_LCD_480_272
		_Display = DISPLAY_BOARD_480_272;
    //
    // Setup display settings for Truly display manually as there is no EEPROM on this display module
    //
    _LcdParams.HBP  = HBP_480_272;
    _LcdParams.HFP  = HFP_480_272;
    _LcdParams.HSW  = HSW_480_272;
    _LcdParams.PPL  = PPL_480_272;
    _LcdParams.VBP  = VBP_480_272;
    _LcdParams.VFP  = VFP_480_272;
    _LcdParams.VSW  = VSW_480_272;
    _LcdParams.LPP  = LPP_480_272;
    _LcdParams.IOE  = IOE_480_272;
    _LcdParams.IPC  = IPC_480_272;
    _LcdParams.IHS  = IHS_480_272;
    _LcdParams.IVS  = IVS_480_272;
    _LcdParams.ACB  = ACB_480_272;
    _LcdParams.BPP  = BPP_480_272;
    _LcdParams.Clk  = CLK_480_272;
    _LcdParams.LCD  = LCD_480_272;
    _LcdParams.Dual = DUAL_480_272;

#endif

}
#endif

/*********************************************************************
*
*       Global functions for display init
*
**********************************************************************
*/

/*********************************************************************
*
*       LCD_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*/
void LCD_X_Config(void) {
  const GUI_DEVICE_API * pDriver;

  #ifndef _WINDOWS
    _UpdateDisplayConfig();
  #endif
  //
  // Check framebuffer size
  //
  #ifndef _WINDOWS
    if ((FB_XSIZE * FB_YSIZE) < (VXSIZE_PHYS * VYSIZE_PHYS)) {
      while (1);  // Error, framebuffer too small
    }
  #endif
  //
  // Initialize multibuffering.
  //
//  GUI_MULTIBUF_Config(3);
  //
  // Set display driver and color conversion for 1st layer
  //
  #ifndef _WINDOWS
  if (_Display == DISPLAY_TRULY_240_320) {
    pDriver = DISPLAY_DRIVER_TRULY;
  } else {
    pDriver = DISPLAY_DRIVER_OTHER;
  }
  #else
  pDriver = GUIDRV_WIN32;
  #endif
  GUI_DEVICE_CreateAndLink(pDriver, COLOR_CONVERSION, 0, 0);
  //
  // Display driver configuration, required for Lin-driver
  //
  LCD_SetPosEx(0, 0, 0);
  if (LCD_GetSwapXYEx(0)) {
    LCD_SetSizeEx  (0, YSIZE_PHYS , XSIZE_PHYS);
    LCD_SetVSizeEx (0, VYSIZE_PHYS, VXSIZE_PHYS);
  } else {
    LCD_SetSizeEx  (0, XSIZE_PHYS , YSIZE_PHYS);
    LCD_SetVSizeEx (0, VXSIZE_PHYS, VYSIZE_PHYS);
  }
  LCD_SetVRAMAddrEx(0, (void*)VRAM_ADDR_VIRT);
  //
  // Set user palette data (only required if no fixed palette is used)
  //
  #if defined(PALETTE)
    LCD_SetLUTEx(0, PALETTE);
  #endif
}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Purpose:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
  #ifndef _WINDOWS
  LCD_X_SETORG_INFO * pSetOrg;
  #endif
  int r;

  (void) LayerIndex;

  switch (Cmd) {
  //
  // Required
  //
  case LCD_X_INITCONTROLLER:
    //
    // Called during the initialization process in order to set up the
    // display controller and put it into operation. If the display
    // controller is not initialized by any external routine this needs
    // to be adapted by the customer...
    //
    // ...
    #ifndef _WINDOWS
      _InitController(0);
    #endif
    return 0;
  case LCD_X_SETORG:
    //
    // Required for setting the display origin which is passed in the 'xPos' and 'yPos' element of p
    //
    #ifndef _WINDOWS
      pSetOrg = (LCD_X_SETORG_INFO *)pData;
      _SetDisplayOrigin(pSetOrg->xPos, pSetOrg->yPos);  //可以删
    #endif
    return 0;
  default:
    r = -1;
  }
  return r;
}


/*************************** End of file ****************************/
