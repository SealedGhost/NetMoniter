

#include "MainTask.h"

//-Deleted by XiaoLu at 201/5/7
 /* 标示母船的位置 */
  /*
	 mapping mothership_location ={7305545,2030726,0,0};
		*/
//-Deleted end

/* external variables */
extern WM_HWIN  menuWin;
extern WM_HWIN  subWins[4];
extern _boat boat_list[BOAT_LIST_SIZE_MAX];
extern _boat* boat_list_p[BOAT_LIST_SIZE_MAX];
extern boat mothership;
extern short isAllBoatVisible;
extern unsigned char * pStrBuf;

extern void Draw_ScaleRuler(int x0, int y0, long scaleVal);

static WM_HTIMER reTimer;
static unsigned int drawMapSwitch  = 1;
unsigned int as[]  = {'1','2',176};

short shift_x  = 0;
short shift_y  = 0;

short (*line_operation)(short,short);

GUI_RECT pRect[]  = {MAP_LEFT,MAP_TOP,MAP_RIGHT,MAP_BOTTOM};

static int v;


volatile _cursor __cursor  = {(MAP_LEFT+MAP_RIGHT)/2, (MAP_TOP+MAP_BOTTOM)/2,7300000, 1740000, 1,0,20,20};
WM_HTIMER timer_cursor;
long temp_lat = 1920726,temp_long = 7305545;
char time_delay = 0;
char scale_choose = 1;
//char move_map_horizontal = 0,move_map_vertical = 0;
//char latitude_fish_last = -1;

//short count_fr_fsh_ara_last=-1;
short x_signal=0;
short y_signal=0;

///tip框的左上点pixel坐标
short tip_loc_x  = 0;
short tip_loc_y  = 0;


WM_HWIN WM_hwnd;
extern WM_HWIN hDlg_MonitorMap;

mapping center;
short i_cursor;

long flip_lttude = 10000*(flip_speed_lat)/120;
long flip_lngtude = 10000*(flip_speed_long)/120;

void map_draw(long longitude,  long latitude, scale_map scale);

WM_MESSAGE * pUpdateMsg;


static void Draw_WinMapTitle()
{
		GUI_SetBkColor (GUI_GRAY);
		GUI_Clear();
	  GUI_SetColor (GUI_WHITE);
		GUI_SetFont (&GUI_Font28);
		GUI_SetPenSize(2);
		GUI_DrawLine(0,35,800,35);
		GUI_DispStringAt ("航速:", 340, 1);
		GUI_DispStringAt ("航向:", 490, 1);
		GUI_PNG_Draw(&acCompass,sizeof (acCompass),350,40);	
}




void display_longitude_latitude(unsigned long l_o_l,short x, short y)
{
  long degree;
  float minute;
  degree = l_o_l/(distance<<1);
  minute = (l_o_l%(distance<<1))/mul_pow;
	GUI_SetFont (&GUI_Font16B_ASCII);
  GUI_DispDecAt(degree,x,y,3);
	GUI_SetFont (&GUI_Font16B_1);
	GUI_DispCharAt(176,x+22,y);
	GUI_SetFont (&GUI_Font16B_ASCII);
  GUI_GotoX(x+30);
	GUI_DispFloat(minute,6);
}




/**@brief 画map中的网格线
*
*
*/
void draw_map_grid(mapping gridAnchor,scale_map scale)
{
	short x = 0;
	short y = 0;

	x  = gridAnchor.x;
	y  = gridAnchor.y;
	
	
  GUI_SetColor(GUI_WHITE);
	while(x<MAP_LEFT)
	{
		x  += scale.pixel;
	}
	while(x<MAP_RIGHT)
	{
		GUI_DrawLine(x, MAP_TOP, x, MAP_BOTTOM);
		x  += scale.pixel;
	}
	
	while(y<MAP_TOP)
	{
		y  += scale.pixel;
	}
	while(y<MAP_BOTTOM)
	{
		GUI_DrawLine(MAP_LEFT, y,MAP_RIGHT, y);
		y  += scale.pixel;
	}
}

unsigned int getFishingAreaId(long longitude, long latitude )
{
	int index  = 0;
	volatile int counterAreaNum  = 0;
	volatile int originalAreaNum  = 0;
	unsigned int fishingAreaId  = 0;
  volatile	unsigned int exp_id  = 0;
		
  volatile int row  = 0;
  volatile int col  = 0;
	
	if(fishing_area[index].latitude_fish < latitude)
		return 0;
	
	for(index=0;index < num_fish;)
	{
		if(fishing_area[index].latitude_fish<latitude)
		{
			index--;
			
			if(longitude < fishing_area[index].longitude_fish)
				return 0;

			///不含拓展渔区的渔区数量,即原有渔区数量
			counterAreaNum  = (longitude-fishing_area[index].longitude_fish ) / distance;
			originalAreaNum  = fishing_area[index+1].fish_number - fishing_area[index].fish_number;
	   
      if(counterAreaNum < originalAreaNum)			
				fishingAreaId = fishing_area[index].fish_number + counterAreaNum;
		  else if(counterAreaNum < originalAreaNum+fishing_area[index].exp_num)
				fishingAreaId  = (fishing_area[index].fish_number+originalAreaNum-1)*10 + counterAreaNum-originalAreaNum+1;
			else
				fishingAreaId  = 0;
			
			if( (scale_choose==1) && (fishingAreaId!=0))
			{
				row  = (fishing_area[index].latitude_fish-latitude)/10000 ;
				col  = (longitude-fishing_area[index].longitude_fish)%30000 /10000 + 1;
//				exp_id  = (latitude-fishing_area[index].latitude_fish)/10000 * 3 + ((longitude-fishing_area[index].longitude_fish)%distance)/10000+1;
				exp_id  = row*3+col;
				exp_id  = exp_id<<(sizeof(int)-4);
				fishingAreaId  = fishingAreaId | exp_id<<(8*sizeof(int)-4);
			}
			return fishingAreaId;

		}
		else
		{
			index++;
		}
	}
	return 0;
	
}

/**    getWrapPara()
*    
*    @Description:  通过查找最大和最小的经纬度，确定船舶分布的区域，再计算所需的scale
*    @inputs     :   halfDiff_lg,分布区域的中点经度；halfDiff_lt，中点纬度；
                     wrap_scale 能够满足屏幕切包含这块区域的scale
*    @outputs    :
*    @return     :
*/
void getWrapPara(long * halfDiff_lg, long * halfDiff_lt, scale_map* wrap_scale)
{
  int i  = 0;
  int min_lg_index  = 0;
  int max_lg_index  = 0;
  int min_lt_index  = 0;
  int max_lt_index  = 0;
  
  long maxDiff_lg  = 0;
  long maxDiff_lt  = 0;
  
  long minute  = 0;
  
  if(N_boat <= 0)
  {
     *halfDiff_lg  = mothership.longitude;
     *halfDiff_lt  = mothership.latitude;
     
     return;
  }
  
  /// 找到第一个经纬度都不为 0 的 boat
  for(i=0;i<N_boat;i++)
  {
     if( (boat_list_p[i]->latitude > 0)  &&  (boat_list_p[i]->longitude > 0) )
     {
        min_lg_index  = i;
        min_lt_index  = i;
        max_lg_index  = i;
        max_lt_index  = i;
        break;
     }
  }
  
  ///分别查找最大最小经纬度的船的索引
  for(;i<N_boat;i++)
  {
     if( (boat_list_p[i]->longitude < boat_list_p[min_lg_index]->longitude)  
         &&  (boat_list_p[i]->longitude > 0)  &&  (boat_list_p[i])->latitude > 0 )
       min_lg_index  = i;
     else if(boat_list_p[i]->longitude > boat_list_p[max_lg_index]->longitude)
       max_lg_index  = i;
     
     if( (boat_list_p[i]->latitude < boat_list_p[min_lt_index]->latitude)  
         &&  (boat_list_p[i]->latitude >0)  &&  (boat_list_p[i]->longitude > 0) )
       min_lt_index  = i;
     else if(boat_list_p[i]->latitude  > boat_list_p[max_lt_index]->latitude)
       max_lt_index  = i;     
  }
  




 ///将母船考虑在内后，计算要适配的区域的三个参数：
 ///   maxDiff_lg:表示适配区域的经度差（宽度）
 ///   maxDiff_lt:  ......      纬度差（高度） 
 ///   halfDiff_lg,halfDiff_lt: 适配区域中心点的经纬度坐标
  if(mothership.longitude > boat_list_p[max_lg_index]->longitude)
  {
     maxDiff_lg  = mothership.longitude - boat_list_p[min_lg_index]->longitude;  
     *halfDiff_lg = mothership.longitude/2 + boat_list_p[min_lg_index]->longitude/2;
   
  }
  else if(mothership.longitude < boat_list_p[min_lg_index]->longitude)
  {
     maxDiff_lg  = boat_list_p[max_lg_index]->longitude - mothership.longitude;
     *halfDiff_lg = mothership.longitude/2 + boat_list_p[max_lg_index]->longitude/2;
   
  }
  else  
  {
     maxDiff_lg  = boat_list_p[max_lg_index]->longitude - boat_list_p[min_lg_index]->longitude;
     *halfDiff_lg = boat_list_p[max_lg_index]->longitude/2 + boat_list_p[min_lg_index]->longitude/2;
     
  }
     
     
  if(mothership.latitude > boat_list_p[max_lt_index]->latitude)
  {
     maxDiff_lt  = mothership.latitude - boat_list_p[min_lt_index]->latitude;
     *halfDiff_lt = mothership.latitude/2 + boat_list_p[min_lt_index]->latitude/2;
    
  }
     
  else if(mothership.latitude < boat_list_p[min_lt_index]->latitude)
  {
     maxDiff_lt  = boat_list_p[max_lt_index]->latitude - mothership.latitude;
     *halfDiff_lt = mothership.latitude/2 + boat_list_p[max_lt_index]->latitude/2;
    
  }
     
  else
  {
     maxDiff_lt  = boat_list_p[max_lt_index]->latitude  - boat_list_p[min_lt_index]->latitude;
     *halfDiff_lt = boat_list_p[max_lt_index]->latitude/2 + boat_list_p[min_lt_index]->latitude/2;
    
  }
  
   
   ///map的宽为800pix，高为400pix，判断在适配上述区域时以map的宽来适配还是高来适配
   
   ///若适配区域的宽度大于高度的两倍，则以map的宽来适配
  if(( maxDiff_lg/2) > maxDiff_lt)
  {
     wrap_scale->minute  = ( maxDiff_lg/(8*100) + 1)*100;  ///这种写法保证所得到的scale.minute为100的整数倍
  }
  ///否则以map的高来适配
  else
  {
     wrap_scale->minute  = ( maxDiff_lt/(4*100) + 1)*100;
  }
}

/**    SetWrapedView()
*
*   @Description:  得到适配渔区的参数后，以所得scale显示
*
*/
void setWrapedView()
{
   ///若没有boat，则以母船为中心，用最小scale来显示
   long lg  = mothership.longitude;
   long lt  = mothership.latitude;
   
   scale_map wrapScale ={100,100};
   
   if(N_boat > 0)
      getWrapPara(&lg, &lt, &wrapScale);
   

   
   map_draw(lg, lt, wrapScale);
   
   center.x=(MAP_LEFT+MAP_RIGHT)/2;
	 center.y=(MAP_TOP+MAP_BOTTOM)/2;
						
	 GUI_SetLineStyle(GUI_LS_SOLID);
	 GUI_SetColor(GUI_BLUE);
   
   disp_fish_boat(&wrapScale, lg, lt, MAP_LEFT/2+MAP_RIGHT/2, MAP_TOP/2+MAP_BOTTOM/2, boat_list_p, N_boat);
}



void map_draw(long longitude,  long latitude, scale_map scale)
{
	  int i = 0;
	  short count_fr_fsh_ara = 0;
	  short num_fr_fsh_ara   = 0;
	
	 short tmp_x = 0;
	 short tmp_y = 0;
//   short tmp_user  = 0;
	 
	 long tmp_lttude  = 0;
	 long tmp_lgtude  = 0;
	
	unsigned int fishingAreaId  = 0;
	
	///画map的锚点
	 mapping  anchor;
	
	 display disp_fsh_ara; 
	 
	 shift_x  = (scale.pixel>>1) - 16;
	 shift_y  = (scale.pixel>>1)- 8;
	 
	 anchor.lgtude  = longitude;
	 anchor.lttude  = latitude;
	
	 
		 do
		 {
			 if(fishing_area[i+1].latitude_fish<latitude && fishing_area[i].latitude_fish>=latitude)
			 {
				 count_fr_fsh_ara  = (longitude - fishing_area[i].longitude_fish)/scale.minute;
				 num_fr_fsh_ara    = fishing_area[i].fish_number + count_fr_fsh_ara;
				 break;
			 }
			 else
			 {
				 i++;
			 }
		 }while(i<num_fish-1);
		 
		 anchor.y  = (MAP_TOP+MAP_BOTTOM)/2 - \
		                 (fishing_area[i].latitude_fish-latitude)*scale.pixel \
		                 / scale.minute;
		 anchor.x  = (MAP_LEFT+MAP_RIGHT)/2  - \
		                 (longitude-fishing_area[i].longitude_fish)%scale.minute \
		                 * scale.pixel / scale.minute;
		 
		 anchor.lttude  = fishing_area[i].latitude_fish;
		 anchor.lgtude  = fishing_area[i].longitude_fish + count_fr_fsh_ara * scale.minute;

		 
	   while(anchor.x  >= MAP_LEFT+(scale.pixel-shift_x))
		 {
			 anchor.x  -= scale.pixel;
			 anchor.lgtude -= scale.minute;
		 }
		 while(anchor.y  > MAP_TOP+(scale.pixel-shift_y))
		 {
			 anchor.y  -= scale.pixel;
			 anchor.lttude  += scale.minute;
		 }
		  
		 draw_map_grid(anchor, scale);
		 
		 tmp_x  = anchor.x;
		 tmp_y  = anchor.y;
		 tmp_lgtude  = anchor.lgtude;
		 tmp_lttude  = anchor.lttude;

		 GUI_SetColor (GUI_BLACK);
     GUI_SetFont(&GUI_Font16B_1);
     for(tmp_y=anchor.y;tmp_y<=MAP_BOTTOM;)			 
		 {
				 
				if(scale_choose <=  1)
				{
					 tmp_x  = anchor.x;
					 tmp_lgtude  = anchor.lgtude ;
					 
					 for(tmp_x=anchor.x; tmp_x<=MAP_RIGHT-(scale.pixel - shift_x);)
					 {
							 fishingAreaId  = getFishingAreaId(tmp_lgtude, tmp_lttude);
							 
							 if(measuring_scale[1].minute == scale.minute  && (0 != fishingAreaId))
							 {
									 if( 1000 > (fishingAreaId&0x0fffffff))
									 {
										 GUI_DispDecAt(fishingAreaId & 0x0fffffff, tmp_x+shift_x, tmp_y+shift_y, 3);							 
									 }
									 else
									 {
										 GUI_DispDecAt(fishingAreaId & 0x0fffffff, tmp_x+shift_x, tmp_y+shift_y, 4);
									 }
									 
									GUI_DispChar('_' );
									GUI_DispDec(fishingAreaId>>(8*sizeof(int)-4), 1);							 
							 
						 }
						 else if((measuring_scale[0].minute == scale.minute) && (0 != fishingAreaId))
						 {
								 if(1000 > fishingAreaId)
								 {
									 GUI_DispDecAt(fishingAreaId, tmp_x+shift_x+10, tmp_y+shift_y, 3);							 
								 }
								 else
								 {
									 GUI_DispDecAt(fishingAreaId, tmp_x+shift_x+10, tmp_y+shift_y, 4);
								 }

						 }
						 
						 tmp_x  += scale.pixel;
						 tmp_lgtude  += scale.minute;
					 }					 
				 }
         if( (tmp_y+latitude_display_y_shift) >= MAP_TOP )
         {
           lltostr(tmp_lttude,pStrBuf);
           GUI_DispStringExAt(pStrBuf, MAP_RIGHT+latitude_display_x_shift,tmp_y+latitude_display_y_shift);         
         }

//	         display_longitude_latitude(tmp_lttude,MAP_RIGHT + latitude_display_x_shift,tmp_y + latitude_display_y_shift);						 
				 
				 tmp_y  += scale.pixel;
				 tmp_lttude  -= scale.minute;
			 }
}


static void _PaintFrame(void) 
{
//	GUI_RECT r;
//	WM_GetClientRect(&r);
	GUI_SetBkColor(GUI_GRAY);
	GUI_SetColor(GUI_WHITE);
	GUI_SetLineStyle(GUI_LS_DASH);
	GUI_SetPenSize(1);
	//GUI_SetFont(GUI_Font16);
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
//	GUI_ClearRectEx(&r);
	GUI_ClearRectEx(pRect);
//  	GUI_CURSOR_Select(&GUI_CursorCrossS);
//  	GUI_CURSOR_Show();

}

//void _DeleteFrame(void) 
//{
//	WM_DeleteWindow(_hLastFrame);
//	_hLastFrame = 0;
//}

void _CB_WM(WM_MESSAGE* pMsg)
{
	GUI_RECT Rect;
	switch(pMsg->MsgId)
	{
	case WM_PAINT:
		WM_GetInsideRect(&Rect);
		GUI_SetBkColor(GUI_GREEN);
		GUI_SetColor(GUI_YELLOW);
	 	GUI_SetFont (&GUI_Font24_ASCII);
		GUI_ClearRectEx(&Rect);
		GUI_DrawRectEx(&Rect);
	  GUI_DispStringAt(test_p[i_cursor]->name, 8,2);
			display_longitude_latitude(test_p[i_cursor]->longitude,8,26);
			display_longitude_latitude(test_p[i_cursor]->latitude,10,50);
		break;
	default:
		WM_DefaultProc(pMsg);
	}
}

/*****************************************************************
*  @brief   根据当前光标位置计算Tip框的坐标
* 
*  @details Function:       updateTipLoc()
*           Description:   根据当前光标位置计算Tip框的坐标
*           Calls:         
*           CalledBy:       _cbWindowAllFishMap()
*           Table Accessed:
*           Table Updated:
*           Input:          x(x location of cursor)
*                           y(y location of cursor)
*           Output:         void
*           Return:         void
*           Others:
*
****************************************************************/

static void updateTipLoc(short x, short y)
{
	if(x > MAP_RIGHT-tip_width)
	{
		tip_loc_x  = x-tip_width;
	}
	else if(x > MAP_LEFT)
	{
		tip_loc_x  = x;
	}
	
	if(y > MAP_BOTTOM-tip_height)
	{
		tip_loc_y  = y-tip_height;
	}
	else if(y > MAP_TOP)
	{
		tip_loc_y  = y;
	}
}



void _cbWindowAllFishMap(WM_MESSAGE* pMsg) 
{	
	WM_HWIN hWin = pMsg->hWin;

	short i = 0;

	
	temp_lat  = __cursor.latitude;
	temp_long  = __cursor.longitude ;

	switch (pMsg->MsgId) 
	{
		case WM_CREATE:
				WM_SetFocus(hWin);
				GUI_CURSOR_Select(&GUI_CursorCrossS);
				GUI_CURSOR_Show();
		
				TEXT_CreateEx(5,3,150,30,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT0,"N32°07.295"); //经度
				TEXT_CreateEx(160,3,160,30,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT0,"E118°48.600"); //纬度
				TEXT_CreateEx(420,3,60,30,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT0,"12km"); //航速
				TEXT_CreateEx(570,3,60,30,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT0,"123°"); //航向
				TEXT_CreateEx(650,3,180,30,hWin,WM_CF_SHOW,TEXT_CF_LEFT,GUI_ID_TEXT0,"06/04/4:55"); //时间
		  
		
		  reTimer  = WM_CreateTimer(pMsg->hWin, ID_TIMER_MAP_REFRESH,3000, 0);
				break;
		case WM_TIMER:
			 if(ID_TIMER_MAP_REFRESH == WM_GetTimerId(reTimer))
				{		    
						WM_InvalidateRect( hWin,pRect);
					WM_RestartTimer(reTimer, 3000);
				}
				break;
		
		    #ifndef KEY_TIMER_MODE
		             break;
		    #endif
		/* 按键长按响应 */
					if(time_delay)
					{
								time_delay++;
								if(time_delay == speed_up_after)
									
								/* 长按  光标加速移动 */
									  __cursor.speed = CURSOR_SPEED_FAST;
								

								if(WM_hwnd)
								{
//											if(   (test_p[i_cursor]->x  <=  __cursor.x+__cursor.wide)
//															&&(test_p[i_cursor]->x  >=  __cursor.x-__cursor.wide)
//															&&(test_p[i_cursor]->y  <=  __cursor.y+__cursor.wide)
//															&&(test_p[i_cursor]->y  >=  __cursor.y-__cursor.wide)
//																																																																		  	)
//												{
//												}
//												/* 若超出tip区域，删除tip */
//												else
//												{
//													   CleanText(pMsg);
//																WM_DeleteWindow(WM_hwnd);
//																WM_hwnd=0;
//																//WM_Invalidate(WM_HBKWIN);
//																//GUI_Exec();
//												}
								}
								
						switch(__cursor.key)
						{
						case 1:
							if(__cursor.y <= MAP_TOP+20)
						 {				
                    temp_lat = temp_lat + flip_lttude;	          							 
										if(temp_lat>=MAP_TOP_LT)
										{
											time_delay = 0;	
											break;
										}else
										{
											UART_SendByte(UART_0,'0');
											__cursor.latitude = temp_lat;
										}
										TIMER_handle
				// 						time_delay = 1;
				// 						__cursor.x = map_width/2;
				// 						__cursor.y = map_height/2;
				// 						GUI_CURSOR_SetPosition(__cursor.x,__cursor.y);
				// 						WM_InvalidateRect( hWin,pRect);
										//WM_InvalidateWindow(hWin);
							}
							else
							{
										//UART_SendByte(UART_0,'1');
										__cursor.y = __cursor.y - __cursor.speed;
							}
							break;
						case 2:
							if(__cursor.y >= MAP_BOTTOM-40)
								{

											temp_lat = temp_lat - flip_lttude;
											if(temp_lat<=MAP_BOTTOM_LT)
											{
														time_delay = 0;
														break;
											}
											else
												__cursor.latitude = temp_lat;
											
											TIMER_handle
							}
							else
										__cursor.y = __cursor.y + __cursor.speed;
							break;
							
						case 3:
							if(__cursor.x <= MAP_LEFT+20)
							{
										temp_long = temp_long - flip_lngtude;
								
										if(temp_long<=MAP_LEFT_LG)
										{
													time_delay = 0;
													break;
										}
										else
											__cursor.longitude = temp_long;
										
										
										TIMER_handle
							}
							else
										__cursor.x = __cursor.x - __cursor.speed;
							break;
							
						case 4:
							if(__cursor.x >= MAP_RIGHT-20)
							{
										temp_long = temp_long + flip_lngtude;
							
										if(temp_long>=MAP_RIGHT_LG)
										{
													time_delay = 0;
													break;
										}
										else
											__cursor.longitude = temp_long;
										
										TIMER_handle
							}
							else
										__cursor.x = __cursor.x + __cursor.speed;
							break;
						}
						GUI_CURSOR_SetPosition(__cursor.x,__cursor.y);
						WM_RestartTimer(timer_cursor,timer_millisecond);
			}
			break;
			
		case WM_KEY:
			switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
			{
				
//				case GUI_KEY_F1:
//					isAllBoatVisible  = 1;
//					Doubleclick = 1;
//					WM_BringToTop (hDlg_AllList); //所有船舶列表
//					WM_SetFocus (hDlg_AllList);
//					GUI_CURSOR_Hide();
//				break;

//				case GUI_KEY_F2:
//					
//				  if(isAllBoatVisible)
//						isAllBoatVisible  = 0;
//					else
//						isAllBoatVisible  = 1;
//				  

//					WM_InvalidateRect(hWin, pRect);
//				break;
//			
				
				/*----------------------   捕捉到中心按键按下的响应:    -----------------------*/
				/*   中心按键按下后：将本船位置和光标显示到map中心点 */
					
				case GUI_KEY_ENTER:
					
				 /* 光标定位到中心 */
							__cursor.x = (MAP_LEFT+MAP_RIGHT)/2;
							__cursor.y = (MAP_TOP+MAP_BOTTOM)/2;

				/* map中心点经纬度 */
//         mothership.latitude = 1927265;
//         mothership.longitude = 7128660;
//         mothership.true_heading  = 0;
				
				/* 光标代表的经纬度设置为母船的经纬度 */
				   __cursor.latitude = mothership.latitude;
				   __cursor.longitude = mothership.longitude;
  
				
				
				/* 判断是否离开tip区域，若离开则删除tip */
//					if(WM_hwnd)
//					{
//									if(   (test_p[i_cursor]->x  <=  __cursor.x+__cursor.wide)
//													 &&(test_p[i_cursor]->x  >=  __cursor.x-__cursor.wide)
//														&&(test_p[i_cursor]->y  <=  __cursor.y+__cursor.wide)
//														&&(test_p[i_cursor]->y  >=  __cursor.y-__cursor.wide)
//																																									)
//									{ ;// NULL operation
//									}
//									/* 若超出tip区域，删除tip */
//									else
//									{
//														CleanText(pMsg);
//														WM_DeleteWindow(WM_hwnd);
//														WM_hwnd=0;
//													//WM_Invalidate(WM_HBKWIN);
//													//GUI_Exec();
//									}
//					}

//map_getWraped();
					 board_handle					
				break;
				
				case GUI_KEY_UP:
					//KEY_handle(1,y,-)
					if(__cursor.y <= MAP_TOP+20)
					{
									temp_lat = temp_lat + flip_lttude;
									if(temp_lat>=MAP_TOP_LT)
									{
										time_delay = 0;	
										break;
									}
									else													
										__cursor.latitude = temp_lat;
									
									board_handle

					}
					if(temp_lat<MAP_TOP_LT)
					{
					//	KEY_handle(1,y,-)
						__cursor.key = 1;
						if(time_delay == 0)
						{
							time_delay = 1;
							__cursor.y = __cursor.y - __cursor.speed;
							GUI_CURSOR_SetPosition(__cursor.x,__cursor.y);
		
						#ifdef KEY_TIMER_MODE
							timer_cursor =  WM_CreateTimer(hWin,0,timer_millisecond,0);
						#endif
						}
						else
						{
								time_delay = 0;
								WM_DeleteTimer(timer_cursor);
								__cursor.speed = CURSOR_SPEED_SLOW;
//								if(!WM_hwnd)
//								{
//								    for(i_cursor = 0;i_cursor<N_boat;i_cursor ++)
//										{
//											if(isAllBoatVisible || test_p[i_cursor]->isVisible)
//											{
//													if(  (test_p[i_cursor]->x  <= __cursor.x+__cursor.wide)
//																&&(test_p[i_cursor]->x  >= __cursor.x-__cursor.wide)
//																&&(test_p[i_cursor]->y  <= __cursor.y+__cursor.wide)
//																&&(test_p[i_cursor]->y  >= __cursor.y-__cursor.wide)
//																																										)
//													{

//														 updateTipLoc(__cursor.x, __cursor.y);
//														 WM_hwnd  = WM_CreateWindow(tip_loc_x,tip_loc_y,tip_width,tip_height,WM_CF_SHOW,_CB_WM,0); 
//						
//														 InitText(pMsg,test_p[i_cursor]->user_id,test_p[i_cursor]->SOG,test_p[i_cursor]->COG,test_p[i_cursor]->true_heading,test_p[i_cursor]->longitude,test_p[i_cursor]->latitude,test_p[i_cursor]->name,test_p[i_cursor]->type_of_electronic_position_fixing_device);
//														 break;
//													}
//											}
//										}										
//								}
							 
//							 else
//							 {
//										if(  (test_p[i_cursor]->x  <= __cursor.x+__cursor.wide)
//											  &&(test_p[i_cursor]->x  >= __cursor.x-__cursor.wide)
//										   &&(test_p[i_cursor]->y  <= __cursor.y+__cursor.wide)
//										   &&(test_p[i_cursor]->y  >= __cursor.y-__cursor.wide)
//										                                                        )
//										{
//										}
//										else
//										{
//											  CleanText(pMsg);
//													WM_DeleteWindow(WM_hwnd);
//													WM_hwnd=0;
//			// 									WM_Invalidate(WM_HBKWIN);
//			// 									GUI_Exec();
//								  }
//							 }
						}
					}
					break;					
				case GUI_KEY_DOWN:

 					if(__cursor.y >= MAP_BOTTOM-40){
						temp_lat = temp_lat - flip_lttude;
						if(temp_lat<=MAP_BOTTOM_LT)
						{
							time_delay = 0;
							//temp_lat = __cursor.latitude;
							break;
						}
						else
							__cursor.latitude = temp_lat;
						
						board_handle
					}

					if(temp_lat>MAP_BOTTOM_LT){
						__cursor.key = 2;
						if(time_delay == 0)
						{
							time_delay = 1;
							__cursor.y = __cursor.y + __cursor.speed;
							GUI_CURSOR_SetPosition(__cursor.x,__cursor.y);
							
							#ifdef KEY_TIMER_MODE
							 timer_cursor =  WM_CreateTimer(hWin,0,timer_millisecond,0);
							#endif
						}
						else{
							time_delay = 0;
							
							#ifdef  KEY_TIMER_MODE
							 WM_DeleteTimer(timer_cursor);
							#endif
							
							__cursor.speed = CURSOR_SPEED_SLOW;
//							if(!WM_hwnd){
//								for(i_cursor = 0;i_cursor<N_boat;i_cursor ++)
//										{
//											if(isAllBoatVisible || test_p[i_cursor]->isVisible)
//											{
//													if(  (test_p[i_cursor]->x  <= __cursor.x+__cursor.wide)
//																&&(test_p[i_cursor]->x  >= __cursor.x-__cursor.wide)
//																&&(test_p[i_cursor]->y  <= __cursor.y+__cursor.wide)
//																&&(test_p[i_cursor]->y  >= __cursor.y-__cursor.wide)
//																																										)
//													{
//														 updateTipLoc(__cursor.x, __cursor.y);
//														 WM_hwnd  = WM_CreateWindow(tip_loc_x,tip_loc_y,tip_width,tip_height,WM_CF_SHOW,_CB_WM,0); 																
//														 InitText(pMsg,test_p[i_cursor]->user_id,test_p[i_cursor]->SOG,test_p[i_cursor]->COG,test_p[i_cursor]->true_heading,test_p[i_cursor]->longitude,test_p[i_cursor]->latitude,test_p[i_cursor]->name,test_p[i_cursor]->type_of_electronic_position_fixing_device);
//													   break;
//													}
//											}
//										}			
//							}
//							else
//							{
//									if(  (test_p[i_cursor]->x <= __cursor.x+__cursor.wide)
//												&&(test_p[i_cursor]->x >= __cursor.x-__cursor.wide)
//												&&(test_p[i_cursor]->y <= __cursor.y+__cursor.wide)
//												&&(test_p[i_cursor]->y >= __cursor.y-__cursor.wide)
//																																																																)
//									{
//									}
//									else
//									{
//										 CleanText(pMsg);
//											WM_DeleteWindow(WM_hwnd);
//											WM_hwnd=0;
//	// 									WM_Invalidate(WM_HBKWIN);
//	// 									GUI_Exec();
//								 }
//							}
						}
					}

					break;
				case GUI_KEY_LEFT:
				if(__cursor.x <= MAP_LEFT+20){
						temp_long = temp_long - flip_lngtude;
						if(__cursor.longitude<=MAP_LEFT_LG)
						{
							time_delay = 0;
							break;
						}
						else
							__cursor.longitude = temp_long;
						
						board_handle
					}

					if(temp_long>MAP_LEFT_LG){
						//KEY_handle(3,x,-)
						__cursor.key = 3;
						if(time_delay == 0)
						{
							time_delay = 1;
							__cursor.x = __cursor.x - __cursor.speed;
							GUI_CURSOR_SetPosition(__cursor.x,__cursor.y);
							
							#ifdef KEY_TIMER_MODE
							 timer_cursor =  WM_CreateTimer(hWin,0,timer_millisecond,0);
							#endif
							
						}
						else{
							time_delay = 0;
							
							#ifdef KEY_TIMER_MODE
							 WM_DeleteTimer(timer_cursor);
							#endif
							
							__cursor.speed = CURSOR_SPEED_SLOW;
//							if(!WM_hwnd){
//								for(i_cursor = 0;i_cursor<N_boat;i_cursor ++)
//										{
//											if(isAllBoatVisible || test_p[i_cursor]->isVisible)
//											{
//													if(  (test_p[i_cursor]->x  <= __cursor.x+__cursor.wide)
//																&&(test_p[i_cursor]->x  >= __cursor.x-__cursor.wide)
//																&&(test_p[i_cursor]->y  <= __cursor.y+__cursor.wide)
//																&&(test_p[i_cursor]->y  >= __cursor.y-__cursor.wide)
//																																										)
//													{
//														 updateTipLoc(__cursor.x, __cursor.y);
//														 WM_hwnd  = WM_CreateWindow(tip_loc_x,tip_loc_y,tip_width,tip_height,WM_CF_SHOW,_CB_WM,0); 																
//														 InitText(pMsg,test_p[i_cursor]->user_id,test_p[i_cursor]->SOG,test_p[i_cursor]->COG,test_p[i_cursor]->true_heading,test_p[i_cursor]->longitude,test_p[i_cursor]->latitude,test_p[i_cursor]->name,test_p[i_cursor]->type_of_electronic_position_fixing_device);
//														 break;
//													}
//											}
//										}			
//							}
//							else
//							{
//								if((test_p[i_cursor]->x<=__cursor.x+__cursor.wide)&&(test_p[i_cursor]->x>=__cursor.x-__cursor.wide)&&(test_p[i_cursor]->y<=__cursor.y+__cursor.wide)&&(test_p[i_cursor]->y>=__cursor.y-__cursor.wide)){
//							 }
//							else
//							{
//									 CleanText(pMsg);
// 									WM_DeleteWindow(WM_hwnd);
//								 	WM_hwnd=0;
//// 									WM_Invalidate(WM_HBKWIN);
//// 									GUI_Exec();
//							}
//							}
						}
					}
					break;
				case GUI_KEY_RIGHT:
				if(__cursor.x >= MAP_RIGHT-20){
							temp_long = temp_long + flip_lngtude;					
					
						if(__cursor.longitude>=MAP_RIGHT_LG)
						{
							time_delay = 0;
							break;
						}
						else
							__cursor.longitude = temp_long;
						board_handle
					}

					if(temp_long<MAP_RIGHT_LG){
						//KEY_handle(4,x,+)
						__cursor.key = 4;
						if(time_delay == 0)
						{
							time_delay = 1;
							__cursor.x = __cursor.x + __cursor.speed;
							GUI_CURSOR_SetPosition(__cursor.x,__cursor.y);
							
							#ifdef KEY_TIMER_MODE
							 timer_cursor =  WM_CreateTimer(hWin,0,timer_millisecond,0);
							#endif
							
						}
						else{
							time_delay = 0;
							
							#ifdef KEY_TIMER_MODE
							 WM_DeleteTimer(timer_cursor);
							#endif
							
							__cursor.speed = CURSOR_SPEED_SLOW;
//							if(!WM_hwnd){
//								for(i_cursor = 0;i_cursor<N_boat;i_cursor ++)
//										{
//											if(isAllBoatVisible || test_p[i_cursor]->isVisible)
//											{
//													if(  (test_p[i_cursor]->x  <= __cursor.x+__cursor.wide)
//																&&(test_p[i_cursor]->x  >= __cursor.x-__cursor.wide)
//																&&(test_p[i_cursor]->y  <= __cursor.y+__cursor.wide)
//																&&(test_p[i_cursor]->y  >= __cursor.y-__cursor.wide)
//																																										)
//													{
//														 updateTipLoc(__cursor.x, __cursor.y);
//														 WM_hwnd  = WM_CreateWindow(tip_loc_x,tip_loc_y,tip_width,tip_height,WM_CF_SHOW,_CB_WM,0); 																
//														 InitText(pMsg,test_p[i_cursor]->user_id,test_p[i_cursor]->SOG,test_p[i_cursor]->COG,test_p[i_cursor]->true_heading,test_p[i_cursor]->longitude,test_p[i_cursor]->latitude,test_p[i_cursor]->name,test_p[i_cursor]->type_of_electronic_position_fixing_device);
//														 break;
//													}
//											}
//										}			
//							}else{
//								if((test_p[i_cursor]->x<=__cursor.x+__cursor.wide)&&(test_p[i_cursor]->x>=__cursor.x-__cursor.wide)&&(test_p[i_cursor]->y<=__cursor.y+__cursor.wide)&&(test_p[i_cursor]->y>=__cursor.y-__cursor.wide)){
//								}else
//								{
//									 CleanText(pMsg);
// 									WM_DeleteWindow(WM_hwnd);
//									 WM_hwnd=0;
//// 									WM_Invalidate(WM_HBKWIN);
//// 									GUI_Exec();
//								}
//							}
						}
					}

					break;
				case GUI_KEY_LARGE:
					if(scale_choose <MAX_GEAR)
						scale_choose++;
					WM_InvalidateRect( hWin,pRect);//WM_Paint(hWin);
					flip_lttude = measuring_scale[scale_choose].minute*((flip_speed_lat)/measuring_scale[scale_choose].pixel);
					flip_lngtude = measuring_scale[scale_choose].minute*((flip_speed_long)/measuring_scale[scale_choose].pixel);
					break;
				case GUI_KEY_REDUCE:
					if(scale_choose >0)
						scale_choose--;
					WM_InvalidateRect( hWin,pRect);//WM_Paint(hWin);
					flip_lttude = measuring_scale[scale_choose].minute*(flip_speed_lat)/measuring_scale[scale_choose].pixel;
					flip_lngtude = measuring_scale[scale_choose].minute*(flip_speed_long)/measuring_scale[scale_choose].pixel;
					break;
					
				case GUI_KEY_MENU:	         
          GUI_CURSOR_Hide();
					WM_BringToTop (menuWin);
					WM_ShowWindow(subWins[0]);
          WM_ShowWindow(subWins[1]);
          WM_ShowWindow(subWins[2]);
          WM_ShowWindow(subWins[3]);
          WM_SetFocus(menuWin);
//					WM_SetFocus (WM_GetDialogItem (menuWin, GUI_ID_BUTTON0));
				break;

			}
		break;
			
		case WM_PAINT: 

//			GUI_MULTIBUF_Begin();
		    GUI_SetBkColor(GUI_GRAY);
		    GUI_Clear();
		    GUI_SetColor(GUI_WHITE);
		    GUI_SetFont(&GUI_Font28);
		    GUI_SetPenSize(2);
		    GUI_DrawLine(0,MAP_TOP-2,800,MAP_TOP-2);
				GUI_DispStringAt("航速:",340,1);
				GUI_DispStringAt("航向:",490,1);
							
   _PaintFrame();



		 GUI_SetColor(GUI_BLACK);
		 GUI_SetLineStyle(GUI_LS_DASH);
		 GUI_SetTextMode(GUI_TM_TRANS);
     
     
     /// drawMapSwitch 置 1，不适配渔区，只会动态显示船舶
     /// drawMapSwitch 置 0, 动态适配渔区，（后台经纬度数据会出错，运行一会儿GG）
     drawMapSwitch  = 0;
     
     if(drawMapSwitch)
     {
			 map_draw(__cursor.longitude,__cursor.latitude,measuring_scale[scale_choose]);

				center.x=(MAP_LEFT+MAP_RIGHT)/2;
				center.y=(MAP_TOP+MAP_BOTTOM)/2;
		
			center.lgtude = __cursor.longitude;
			center.lttude = __cursor.latitude;
			
			
			GUI_SetLineStyle(GUI_LS_SOLID);
			GUI_SetColor(GUI_BLUE);
			disp_fish_boat(&measuring_scale[scale_choose],center.lgtude,center.lttude,center.x,center.y,boat_list_p,N_boat);          
     }
     else
     {
        setWrapedView();
     }

      
      
			GUI_SetFont (&GUI_Font16B_1);
			GUI_PNG_Draw(&acCompass,sizeof(acCompass),350,40);
			
//-Add by XiaoLu at 2015/5/7
		
		/* 重绘AllFishMap时判断tip框是否为存在状态,若为存在状态将tip重绘 
		   防止窗口切换后本来存在的tip框没有显示
		 */
		 if(WM_hwnd)
			{
				 WM_BringToTop(WM_hwnd);
			}
//-Add end		 
//			PaintFrame();
			//InitText (pMsg);
		////////////
			GUI_SetFont (&GUI_Font16_ASCII);
//			GUI_MULTIBUF_End();
//			v = GUI_ALLOC_GetNumFreeBytes();
//			GUI_DispDecAt (v , 150, 5, 10);
//			v = GUI_ALLOC_GetNumUsedBytes();
//			GUI_DispDecAt (v, 250, 5, 10);
//			Draw_ScaleRuler(MAP_RIGHT-200,MAP_BOTTOM-50,measuring_scale[scale_choose].scaleVal);
		//////////////////
		break;
// 		case WM_NOTIFY_PARENT:
// 			if (pMsg->Data.v == WM_NOTIFICATION_RELEASED) 
// 			{
// 				int Id = WM_GetId(pMsg->hWinSrc);
// 				switch (Id) 
// 				{
// 					case GUI_ID_BUTTON0:
// 						_Language = 0;
// 						break;
// 					case GUI_ID_BUTTON1:
// 						_Language = 1;
// 						break;
// 				}
// 				_hTitle = TEXT_CreateEx(0, 0, LCD_GetXSize(), 32, WM_HBKWIN, WM_CF_SHOW, 0, GUI_ID_TEXT0, _GetLang(TEXT_ID_GELDAUTOMAT));
// 				TEXT_SetTextAlign(_hTitle, GUI_TA_HCENTER);
// 				TEXT_SetFont(_hTitle, MAIN_FONT);
// 				_DeleteFrame();
// 				_CreateFrame(&_cbInsertCard);
// 			}
// 			break;
		default:
			WM_DefaultProc(pMsg);
	}
}


