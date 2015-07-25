#include "MainTask.h"
#include "51t9py_indexa.h"
#include "string.h"
#include "math.h"
#include "Drawinformation.h"


/* external variables */
extern boat mothership;

unsigned char strBuf[20];
unsigned char* pStrBuf = strBuf;
char du[5];
char fen[6];


float getDistance(const long lt, const long lg)
{
	
}

long strtoi(const char*str)
{
	int val = 0;
	short i = 0;
	
	while('\0' != str[i])
	{
		val  = val*10+str[i]-'0';
		i++;
	}
	return val;
}
void showname (WM_MESSAGE* pMsg,const char* name)
{
		WM_HWIN hWin = pMsg->hWin;
		char str[20] = {0};
		unsigned int AmountT9PY = sizeof(t9PY_index2)/sizeof(t9PY_index2[0]);//索引表的总数
		unsigned int PY_index;		  //用于对结构体数组进行索引
		unsigned int shipNameLength;  //字符串船名的长度
		unsigned int theSame;         //判断是否有对应的汉字
		unsigned int newshipNameIndex;
		unsigned int SplitshipNameIndex;
		unsigned int shipNameIndex=0;
		char shipName[20]={0};            //船名字符串
		char SplitshipName[20];		  //和索引表对比的船名拼音
		
		strcpy(shipName,name);
		shipNameLength = strlen(shipName); //计算船名长度
		/*
		将船名的拼音和所有的拼音表对比,
		没有对应的汉字，舍去船名拼音最后一个，剩下的继续对比；
		有对应汉字，把对应的拼音舍去，剩下的继续。
		*/
		for ( shipNameIndex = shipNameLength; shipNameIndex > 0; shipNameIndex-- )
		{
			for( SplitshipNameIndex=0; SplitshipNameIndex < shipNameIndex; SplitshipNameIndex++ )
				SplitshipName[SplitshipNameIndex] = shipName[SplitshipNameIndex];
			SplitshipName[SplitshipNameIndex] = '\0';
			
			for ( PY_index = 0; PY_index < AmountT9PY; PY_index++ )
			{
				theSame = strcmp ( SplitshipName, t9PY_index2[PY_index].PY );
				if ( theSame == 0 )
					{ 
						strcat (str, t9PY_index2[PY_index].PY_mb);
						SplitshipName[0] = 0;
						for ( newshipNameIndex = 0; shipNameIndex < shipNameLength; shipNameIndex++, newshipNameIndex++ ) //把识别到的部分去掉，剩下的继续拆分
							shipName[newshipNameIndex] = shipName[shipNameIndex];
						shipName[newshipNameIndex] = '\0';
						shipNameIndex = strlen ( shipName ) + 1;
						break;
					}
			}
			//
			//没有对应汉字，并且对比到船名的第一个字符,就显示第一个字符，剩下的字符继续对比
			//
			if ( theSame != 0 )
			{    
				if ( shipNameIndex == 1 && strlen ( shipName ) > 1 )
				{
					strcat (str, SplitshipName);
					for ( newshipNameIndex = 0; shipNameIndex < shipNameLength; shipNameIndex++, newshipNameIndex++ )
						shipName[newshipNameIndex] = shipName[shipNameIndex];

					shipName[newshipNameIndex] = '\0';
					shipNameIndex = strlen ( shipName ) + 1;
				}
			}
		}
		//只有一个字符且没有汉字对应
		strcat (str, SplitshipName);
		TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3),str);
}

void GUI_DispStringExAt(const unsigned char *str,const int x, const int y)
{
	int i  = 0;
	int fontXSize  = 0;
//	const GUI_FONT* font  = NULL;
//	font  = GUI_GetFont();
	fontXSize  = GUI_GetCharDistX('a');
	while(str[i] != '\0')
	{
		GUI_DispCharAt(str[i],x+i*fontXSize,y);
		i++;
	}
}





char * ttoi(long num , char *str)
{
    char *index = "0123456789",temp;
    long unum=num;
    int i=0,j=0,js,s;
    do{
			str[i++]=index[unum%10];
			unum/=10;
    }while(unum);
    str[i]='\0';
    js=(i-1)/2;
    for(j=0;j<=js;j++){
			s=i-1-j;
			temp = str[j];
			str[j]=str[s];
			str[s]=temp;
    }
    return str;
}

void lltostr(long l_o_l,char *str)
{
	char i = 0,j=0;
	int degree  = 0;
	int minute_int  = 0;
	int minute_dec  = 0;
	char tmp[10];
 if( l_o_l==0)
 {
    str[0]  = '?';
    str[1]  = '?';
    str[2]  = '?';
    str[3]  = 176;
    str[4]  = '?';
    str[5]  = '?';
    str[6]  = '.';
    str[7]  = '?';
    str[8]  = '?';
    str[9]  = '?';
    str[10] = '\0';
 }
 else
 {
    degree = l_o_l/(distance<<1);
    minute_int = (l_o_l%(distance<<1))/mul_pow;
    minute_dec = (l_o_l%(distance<<1))-minute_int*mul_pow;
    
   //	ttoi(degree,tmp);
    sprintf(str,"%3d",degree);
    str[3]  = 176;
    sprintf(str+4,"%02d",minute_int);
    str[6] = '.';
    sprintf(str+7,"%03d",minute_dec); 
 }

}
void PaintFrame() 
{
	
	
// 	GUI_SetFont (&GUI_Font16);
// 	GUI_SetColor(GUI_WHITE);
// 	GUI_SetPenSize(2);
// 	GUI_DrawLine(LINE_X,0,LINE_X,455);
// 	GUI_DrawLine(LINE_X,LINE_Y1,800,LINE_Y1);
// 	GUI_DrawLine(LINE_X,LINE_Y2,800,LINE_Y2);
// 	GUI_DrawLine(LINE_X,LINE_Y3,800,LINE_Y3);
// 	GUI_DrawLine(LINE_X,LINE_Y4,800,LINE_Y4);
// 	GUI_DrawLine(0,455,800,455);
// 	GUI_SetColor(GUI_BLACK);
// 	GUI_DispStringAt("AIS目标船数据:", SIZE_X, SIZE_Y0);
// 	GUI_DispStringAt("MMSI:", SIZE_X, SIZE_Y1+10);
// 	GUI_DispStringAt("船名:", SIZE_X, SIZE_Y2+10);
// 	GUI_DispStringAt("北纬:", SIZE_X, SIZE_Y5);
// 	GUI_DispStringAt("东经:", SIZE_X, SIZE_Y6+10);
// 	GUI_DispStringAt("航速:", SIZE_X, SIZE_Y8-3);
// 	GUI_DispStringAt("航向:", SIZE_X2, SIZE_Y8-3);
// 	GUI_DispStringAt("距离:", SIZE_X, SIZE_Y9);
// 	GUI_DispStringAt("方位:", SIZE_X2, SIZE_Y9);
// 	GUI_DispStringAt("监控信息:", SIZE_X,SIZE_Y10+10);
// 	GUI_DispStringAt("网位仪移动报警距离:", SIZE_X, SIZE_Y12-5);
// 	GUI_DispStringAt("其他靠近报警距离:", SIZE_X, SIZE_Y13-3);
// 	GUI_DispStringAt("报警航速设置:", SIZE_X, SIZE_Y14-1);
// 	GUI_DispStringAt("本船位置:", SIZE_X, SIZE_Y15+10);
// 	

// 	GUI_DispStringAt("提示信息:", SIZE_X, SIZE_Y20+5);	
// 	GUI_SetFont(&GUI_Font32_ASCII);
// 	
// 	
// //-Add By XiaoLu at 2015/5/7	

// 		/* 母船纬度显示 */
// 		GUI_DispStringAt("N:", SIZE_X, SIZE_Y16+10);
// 	lol_to_c(mothership.latitude,pStrBuf);
// 	GUI_DispString(pStrBuf);
// 	
// 		/* 母船经度显示 */
// 		GUI_DispStringAt("E:", SIZE_X, SIZE_Y18);
// 	lol_to_c(mothership.longitude ,pStrBuf);
// 	GUI_DispString(pStrBuf);

//-Add end
}

 void Createbutton(WM_MESSAGE* pMsg)
{
	WM_HWIN hWin = pMsg->hWin;
	WM_HWIN _button0;
	WM_HWIN _button1;
	WM_HWIN _button2;
	WM_HWIN _button3;
	BUTTON_CreateEx(0, 50, 170, 50, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON0);
	_button0 = WM_GetDialogItem (pMsg->hWin, GUI_ID_BUTTON0);
	BUTTON_CreateEx(0, 100, 170, 50, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON1);
	_button1 = WM_GetDialogItem (pMsg->hWin, GUI_ID_BUTTON1);
	BUTTON_CreateEx(0, 150, 170, 50, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON2);
	_button2 = WM_GetDialogItem (pMsg->hWin, GUI_ID_BUTTON2);
	BUTTON_CreateEx(0, 200, 170, 50, hWin, WM_CF_SHOW, 0, GUI_ID_BUTTON3);
	_button3 = WM_GetDialogItem (pMsg->hWin, GUI_ID_BUTTON3);
	BUTTON_SetTextColor(_button0,BUTTON_CI_UNPRESSED,GUI_WHITE);
	BUTTON_SetTextColor(_button1,BUTTON_CI_UNPRESSED,GUI_WHITE);
	BUTTON_SetTextColor(_button2,BUTTON_CI_UNPRESSED,GUI_WHITE);
	BUTTON_SetTextColor(_button3,BUTTON_CI_UNPRESSED,GUI_WHITE);
	BUTTON_SetText(_button0,"1:监控列表");
	BUTTON_SetText(_button1,"2:监控设置");
	BUTTON_SetText(_button2,"3:船舶列表");
	BUTTON_SetText(_button3,"4:系统设置");

	BUTTON_SetBkColor(_button0,BUTTON_CI_UNPRESSED,DEEPBLUE);
	BUTTON_SetBkColor(_button1,BUTTON_CI_UNPRESSED,DEEPBLUE);
	BUTTON_SetBkColor(_button2,BUTTON_CI_UNPRESSED,DEEPBLUE);
	BUTTON_SetBkColor(_button3,BUTTON_CI_UNPRESSED,DEEPBLUE);

	WIDGET_SetEffect(_button0,&WIDGET_Effect_Simple);
	WIDGET_SetEffect(_button1,&WIDGET_Effect_Simple);
	WIDGET_SetEffect(_button2,&WIDGET_Effect_Simple);
	WIDGET_SetEffect(_button3,&WIDGET_Effect_Simple);

// 	TEXT_CreateEx(SIZE_X+150,SIZE_Y0,220,43,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT0,NULL); //船数
// 	TEXT_CreateEx(SIZE_X1,SIZE_Y1+10,220,43,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT1,NULL); //MMSI
// 	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT1),&GUI_FontComic18B_1);	
// 	TEXT_CreateEx(SIZE_X1,SIZE_Y2+10,220,43,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT2,NULL); //船名
// 	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT2),&GUI_Font20_1);	
// 	TEXT_CreateEx(SIZE_X1,SIZE_Y3+10,220,43,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT3,NULL); //船名中文
// 	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT3),&GUI_Font16);	
// 	TEXT_CreateEx(SIZE_X1,SIZE_Y4+10,50,43,hWin,WM_CF_SHOW,TEXT_CF_RIGHT,GUI_ID_TEXT4,NULL);  //东经, 度
// 	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT4),&GUI_Font32_ASCII);	
// 	TEXT_CreateEx(SIZE_X1,SIZE_Y4+10,60,30,hWin,WM_CF_SHOW,TEXT_CF_RIGHT,GUI_ID_TEXT5,NULL);  //北纬 "°"
// 	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT5),&GUI_Font32B_1);
//  	TEXT_CreateEx(SIZE_X1+62,SIZE_Y4+10,100,43,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT6,NULL);//分,秒
// 	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT6),&GUI_Font32_ASCII);	
// 	TEXT_CreateEx(SIZE_X1,SIZE_Y6,50,43,hWin,WM_CF_SHOW,TEXT_CF_RIGHT,GUI_ID_TEXT7,NULL);  //北纬,度
// 	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT7),&GUI_Font32_ASCII);	
// 	TEXT_CreateEx(SIZE_X1,SIZE_Y6,60,30,hWin,WM_CF_SHOW,TEXT_CF_RIGHT,GUI_ID_TEXT8,NULL);  //东经 "°"
// 	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT8),&GUI_Font32B_1);	
//  	TEXT_CreateEx(SIZE_X1+62,SIZE_Y6,100,43,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT9,NULL);//分，秒
// 	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT9),&GUI_Font32_ASCII);	
// 	TEXT_CreateEx(SIZE_X1,SIZE_Y7+15,220,43,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT10,NULL);     //航速
// 	TEXT_CreateEx(SIZE_X3,SIZE_Y7+15,220,43,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT11,NULL);     //航向
// 	TEXT_CreateEx(SIZE_X1,SIZE_Y8+15,220,43,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT12,NULL);     //距离
// 	TEXT_CreateEx(SIZE_X3,SIZE_Y8+15,220,43,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT13,NULL);     //方位
// 	TEXT_CreateEx(SIZE_X4,SIZE_Y11+15,220,43,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT14,NULL);    //网位仪移动报警距离
// 	TEXT_CreateEx(SIZE_X4-10,SIZE_Y12+15,220,43,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT15,NULL); //其他靠近报警距离
// 	TEXT_CreateEx(SIZE_X2+15,SIZE_Y13+15,220,43,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT16,NULL); //报警航速
// 	TEXT_CreateEx(SIZE_X,SIZE_Y16+10,160,30,hWin,WM_CF_SHOW,TEXT_CF_RIGHT,GUI_ID_TEXT17,NULL); //本船位置：N
// 	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT17),&GUI_Font32_ASCII);
// 	TEXT_CreateEx(SIZE_X,SIZE_Y18,160,43,hWin,WM_CF_SHOW,TEXT_CF_RIGHT,GUI_ID_TEXT18,NULL);  //本船位置：E
// 	TEXT_SetFont(WM_GetDialogItem(hWin, GUI_ID_TEXT18),&GUI_Font32_ASCII);
// 	TEXT_CreateEx(SIZE_X,SIZE_Y21+7,200,18,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT19,"可按监控键对该船进行监控");//提示信息
// 	
// 	TEXT_CreateEx(180,SIZE_Y23,100,20,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT20,"渔区图/列表");
// 	TEXT_CreateEx(480,SIZE_Y23,150,20,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT21,"所有船舶/监控船舶");

}

/*************************************
	Add by XiaoLu at 2015/5/7

	Function:       CleanText()
	Description:    Clarn boat text information
	Calls:          
	Called By:      _cbWindowAllFishMap()
	Table Accessed: NULL
	Table Updated:  NULL
	Input:          A message pointer
	Output:         void
	Return:         void
	Others:         

***************************************/
void CleanText(WM_MESSAGE* pMsg)
{
	
	WM_HWIN hWin = pMsg->hWin;
	
	/* Set TEXT1 NULL string */
	TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT1),"");
	TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT2),"");
	TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT3),"");
	TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT4),"");
	TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT5),"");
	TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT6),"");
	TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT7),"");
	TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT8),"");
	TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT9),"");
}

void InitText(WM_MESSAGE* pMsg,long user_id,short SOG,short COG,short true_heading,long longitude,long latitude,char *name,int type_of_electronic_position_fixing_device)
{
	char user_id_c[10],lol_c[11];
	WM_HWIN hWin = pMsg->hWin;
	
	ttoi(user_id,user_id_c);
	TEXT_SetText(WM_GetDialogItem(hWin,GUI_ID_TEXT1),user_id_c);
	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2),name);
	showname(pMsg,name);
	
	lltostr(longitude,lol_c);
	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT4),du);
	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT5),"°");
	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT6),fen);
	
	lltostr(latitude,lol_c);
	lltostr(WM_GetDialogItem(hWin, GUI_ID_TEXT7),du);
	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT8),"°");
	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT9),fen);

// 	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT0),"[7:00]");
// 	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT1),"411371444[A]");
// 	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT2),"MAN DE LI 66");
// 	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT3),"曼德力66");
// 	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT4),"24 36.690N");
// 	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT5),"118 54.209E");
// 	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT6),"7.2");
// 	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT7),"7.2");
// 	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT8),"2.2mm");
//  TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT9),"195°");
// 	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT10),"1.1海里");
// 	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT11),"25.0海里");
// 	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT12),"2节");
// 	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT13),"24 41.184");
// 	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT14),"118 41.491");
// 	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT15),"渔区图/列表");
// 	TEXT_SetText(WM_GetDialogItem(hWin, GUI_ID_TEXT16),"所有船舶/监控船舶");
}

void Draw_ScaleRuler(int x0, int y0, long scaleVal)
{
	
	GUI_DrawLine(x0,y0,x0+52,y0);
	GUI_DrawLine(x0,y0,x0,y0-5),
	GUI_DrawLine(x0+52,y0,x0+52,y0-5);
	
	
	
	if(3000>scaleVal)
	{
		GUI_GotoXY(x0+2,y0-20);
		GUI_DispFloatMin(scaleVal/1000.0,2 );
		GUI_DispChar('n');
		GUI_DispChar('m');
	}
	else
	{
		GUI_GotoXY(x0+10,y0-20);
		GUI_DispDecMin(scaleVal/1000);
		GUI_DispChar('n');
		GUI_DispChar('m');
	}
}


void strcpyEx( unsigned char* dest,  const unsigned char* src)
{
	int i  = 0;
	while(src[i])
	{
		*(dest+i) = *(src+i);
	}
}

int GUI__strlenEx(const unsigned char GUI_UNI_PTR * s) {
 
  int r = -1;
  if (s != '\0') {
 
    do {
 
      r++;
     
} while (*s++);
   
}
  return r;
 
}

int GUI__SetTextEx(GUI_HMEM* phText, const unsigned char* s)
{
 
  int r = 0;
 
    GUI_HMEM hMem;
    hMem = GUI_ALLOC_AllocNoInit(GUI__strlenEx(s) + 1);
    if (hMem)
		{
 
      unsigned char* pMem;
      pMem = (unsigned char*) GUI_ALLOC_h2p(hMem);
      strcpyEx(pMem, s);
      GUI_ALLOC_FreePtr(phText);
      *phText = hMem;
      r = 1;
		}
  return r;
 
}


typedef struct {
  WIDGET Widget;
  WM_HMEM hpText;
  const GUI_FONT GUI_UNI_PTR * pFont;
  I16 Align;
  GUI_COLOR TextColor;
  GUI_COLOR BkColor;
  #if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
    int DebugId;
  #endif  
} TEXT_Obj;
#define TEXT_H2P(h) (TEXT_Obj*) GUI_ALLOC_h2p(h)


void TEXT_SetTextEx(TEXT_Handle hObj, const unsigned char* s) {
 
  if (hObj) {
 
    TEXT_Obj* pObj;
    WM_LOCK();
    pObj = TEXT_H2P(hObj);
    if (GUI__SetTextEx(&pObj->hpText, s)) {
 
      WM_Invalidate(hObj);
     
}
    WM_UNLOCK();
   
}
 
}
 
