

#include "MainTask.h"

//-Deleted by XiaoLu at 201/5/7
 /* 标示母船的位置 */
  /*
	 mapping mothership_location ={7305545,2030726,0,0};
		*/
//-Deleted end

/* external variables */
extern _boat boat_list[BOAT_LIST_SIZE_MAX];
extern _boat* boat_list_p[BOAT_LIST_SIZE_MAX];
extern boat mothership;
extern short isAllBoatVisible;
extern void Draw_ScaleRuler(int x0, int y0, long scaleVal);

static WM_HTIMER reTimer;
static unsigned int aaa[3]  = {48,176,'\0'};
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
void draw_map_grid(mapping gridAnchor)
{
	short x = 0;
	short y = 0;

	x  = gridAnchor.x;
	y  = gridAnchor.y;
	
	
  GUI_SetColor(GUI_WHITE);
	while(x<MAP_LEFT)
	{
		x  += measuring_scale[scale_choose].pixel;
	}
	while(x<MAP_RIGHT)
	{
		GUI_DrawLine(x, MAP_TOP, x, MAP_BOTTOM);
		x  += measuring_scale[scale_choose].pixel;
	}
	
	while(y<MAP_TOP)
	{
		y  += measuring_scale[scale_choose].pixel;
	}
	while(y<MAP_BOTTOM)
	{
		GUI_DrawLine(MAP_LEFT, y,MAP_RIGHT, y);
		y  += measuring_scale[scale_choose].pixel;
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



void get_diff(mapping * diff)
{
	int i  = 0;
	
	int min_lg_index  = 0;
	int max_lg_index  = 0;
	int min_lt_index  = 0;
	int max_lt_index  = 0;

//	long diff_lg  = 0;
//	long diff_lt  = 0;
	
	
	for(i=0;i<3;i++)
	{
		if(test_p[i]->longitude < test_p[min_lg_index]->longitude)
			min_lg_index  = i;
		if(test_p[i]->longitude > test_p[max_lg_index]->longitude)
			max_lg_index  = i;
		
		if(test_p[i]->latitude < test_p[min_lt_index]->latitude)
			min_lt_index  = i;
		if(test_p[i]->latitude > test_p[max_lt_index]->latitude)
			max_lt_index  = i;		
	}
	
	
	diff->lgtude  = test_p[max_lg_index]->longitude - test_p[min_lg_index]->longitude;
	diff->lttude  = test_p[max_lt_index]->latitude - test_p[min_lt_index]->latitude;
	
	__cursor.longitude  = (test_p[max_lg_index]->longitude + test_p[min_lg_index]->longitude) /2;
	__cursor.latitude   = (test_p[max_lt_index]->latitude + test_p[min_lt_index]->latitude) /2;
	
GUI_DispDecAt(diff->lgtude,20,20,10);
GUI_DispDecAt(diff->lttude,20,40,10);
	
//	diff_lg  = test_p[max_lg_index]->longitude - test_p[min_lg_index]->longitude;
//	diff_lt  = test_p[max_lt_index]->latitude - test_p[min_lt_index]->latitude;


//GUI_DispDecAt(diff_lg,20,20,10);
//GUI_DispDecAt(diff_lt,20,40,10);
	
}

void map_getWraped()
{
	int gear  = 0;
	///表示最上方boat和最下方boat的纬度差
	long diff_lt  = 0;
	///表示最右方boat和最左方boat的经度差	
	long diff_lg  = 0;
	
	/// 在当前档位下屏幕能表示的最大纬度差
	long maxDiff_lt  = 0;
	////在当前档位下屏幕能表示的最大经度差
	long maxDiff_lg  = 0;
	
  mapping diff = {0,0,0,0};
	
	get_diff(&diff);
	gear  = 4;
	while(gear>0)
	{
		///计算当前档位下屏幕所能表示的最大经纬度差
		maxDiff_lt  =  MAP_WIDTH * measuring_scale[gear].minute / measuring_scale[gear].pixel;
		maxDiff_lg  =  MAP_HEIGHT  * measuring_scale[gear].minute / measuring_scale[gear].pixel;	

		
		printf("maxDiff_lt=%ld  maxDiff_lg=%ld\r\n",maxDiff_lt,maxDiff_lg);
		///若屏幕足够显示
//    if(maxDiff_lt>=diff_lt  &&  maxDiff_lg>=diff_lg)
		if(maxDiff_lt >= diff.lttude  &&  maxDiff_lg>= diff.lgtude)
		{
			///返回所选档位
			printf("find gear=%d\r\n",gear);
       break;
		}	
    else
		{
       gear--;
		}			
	}
  
	scale_choose  = gear;
	
	__cursor.x  = (MAP_LEFT+MAP_RIGHT)/2;
	__cursor.y  = (MAP_TOP+MAP_BOTTOM)/2;
	
	WM_InvalidateRect(hDlg_FishMap, pRect);
GUI_DispDecAt(gear,20,60,1);
	
}


void map_draw(long longitude,  long latitude, scale_map scale)
{
	  int i = 0;
	  short count_fr_fsh_ara = 0;
	  short num_fr_fsh_ara   = 0;
	
	 short tmp_x = 0;
	 short tmp_y = 0;
	 
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
				 count_fr_fsh_ara  = (longitude - fishing_area[i].longitude_fish)/measuring_scale[scale_choose].minute;
				 num_fr_fsh_ara    = fishing_area[i].fish_number + count_fr_fsh_ara;
				 break;
			 }
			 else
			 {
				 i++;
			 }
		 }while(i<num_fish-1);
		 
		 anchor.y  = (MAP_TOP+MAP_BOTTOM)/2 - \
		                 (fishing_area[i].latitude_fish-latitude)*measuring_scale[scale_choose].pixel \
		                 / measuring_scale[scale_choose].minute;
		 anchor.x  = (MAP_LEFT+MAP_RIGHT)/2  - \
		                 (longitude-fishing_area[i].longitude_fish)%measuring_scale[scale_choose].minute \
		                 * measuring_scale[scale_choose].pixel / measuring_scale[scale_choose].minute;
		 
		 anchor.lttude  = fishing_area[i].latitude_fish;
		 anchor.lgtude  = fishing_area[i].longitude_fish + count_fr_fsh_ara * measuring_scale[scale_choose].minute;

		 
	   while(anchor.x  >= MAP_LEFT+(measuring_scale[scale_choose].pixel-shift_x))
		 {
			 anchor.x  -= measuring_scale[scale_choose].pixel;
			 anchor.lgtude -= measuring_scale[scale_choose].minute;
		 }
		 while(anchor.y  > MAP_TOP+(measuring_scale[scale_choose].pixel-shift_y))
		 {
			 anchor.y  -= measuring_scale[scale_choose].pixel;
			 anchor.lttude  += measuring_scale[scale_choose].minute;
		 }
		  
		 draw_map_grid(anchor);
		 
		 tmp_x  = anchor.x;
		 tmp_y  = anchor.y;
		 tmp_lgtude  = anchor.lgtude;
		 tmp_lttude  = anchor.lttude;

		 GUI_SetColor (GUI_BLACK);
     for(tmp_y=anchor.y;tmp_y<=MAP_BOTTOM;)			 
		 {
				 
				if(scale_choose <=  1)
				{
					 tmp_x  = anchor.x;
					 tmp_lgtude  = anchor.lgtude ;
					 
					 for(tmp_x=anchor.x; tmp_x<=MAP_RIGHT-(measuring_scale[scale_choose].pixel - shift_x);)
					 {
							 fishingAreaId  = getFishingAreaId(tmp_lgtude, tmp_lttude);
							 
							 if(1 == scale_choose  && (0 != fishingAreaId))
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
						 else if((0 == scale_choose) && (0 != fishingAreaId))
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
						 
						 tmp_x  += measuring_scale[scale_choose].pixel;
						 tmp_lgtude  += measuring_scale[scale_choose].minute;
					 }					 
				 }

	       display_longitude_latitude(tmp_lttude,MAP_RIGHT + latitude_display_x_shift,tmp_y + latitude_display_y_shift);						 
				 
				 tmp_y  += measuring_scale[scale_choose].pixel;
				 tmp_lttude  -= measuring_scale[scale_choose].minute;
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

void _DeleteFrame(void) 
{
	WM_DeleteWindow(_hLastFrame);
	_hLastFrame = 0;
}

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
		  
		
		  reTimer  = WM_CreateTimer(pMsg->hWin, ID_TIMER_MAP_REFRESH,5000, 0);
				break;
		case WM_TIMER:
			 if(ID_TIMER_MAP_REFRESH == WM_GetTimerId(reTimer))
				{
MYDEBUG("entry timer and refresh map");					
						WM_InvalidateRect( hWin,pRect);
					WM_RestartTimer(reTimer, 5000);
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
				
				case GUI_KEY_F1:
//					isAllBoatVisible  = 1;
					Doubleclick = 1;
					WM_BringToTop (hDlg_AllList); //所有船舶列表
					WM_SetFocus (hDlg_AllList);
					GUI_CURSOR_Hide();
				break;

				case GUI_KEY_F2:
					
				  if(isAllBoatVisible)
						isAllBoatVisible  = 0;
					else
						isAllBoatVisible  = 1;
				  

					WM_InvalidateRect(hWin, pRect);
				break;
			
				
				/*----------------------   捕捉到中心按键按下的响应:    -----------------------*/
				/*   中心按键按下后：将本船位置和光标显示到map中心点 */
					
				case GUI_KEY_ENTER:
					
				 /* 光标定位到中心 */
							__cursor.x = (MAP_LEFT+MAP_RIGHT)/2;
							__cursor.y = (MAP_TOP+MAP_BOTTOM)/2;

				/* map中心点经纬度 */
				   mothership.latitude = 1927265;
				   mothership.longitude = 7128660;
				   mothership.true_heading  = 0;
				
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
					WM_BringToTop (hDlg_Menu);
					WM_BringToTop (hDlg_MonitorList);
					WM_SetFocus (WM_GetDialogItem (hDlg_Menu, GUI_ID_BUTTON0));
				break;

			}
		break;
			
		case WM_PAINT: 
MYDEBUG("AllFishmap paint");			
//			GUI_MULTIBUF_Begin();
		    GUI_SetBkColor(GUI_GRAY);
		    GUI_Clear();
		    GUI_SetColor(GUI_WHITE);
		    GUI_SetFont(&GUI_Font28);
		    GUI_SetPenSize(2);
		    GUI_DrawLine(0,MAP_TOP-2,800,MAP_TOP-2);
				GUI_DispStringAt("航速:",340,1);
				GUI_DispStringAt("航向:",490,1);
				
		     
		  
		printf("paint begin\r\n");
			
   _PaintFrame();
		/*
			GUI_RECT r;
			WM_GetClientRect(&r);
			GUI_SetBkColor(GUI_GRAY);
			GUI_SetColor(GUI_WHITE);
		  GUI_SetPenSize(1);
			GUI_SetLineStyle(GUI_LS_DASH);
			//GUI_SetFont(FRAME_FONT);
			GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
			GUI_ClearRectEx(&r);
		*/

		
		
//		 _PaintFrame();
		 GUI_SetColor(GUI_BLACK);
		 GUI_SetLineStyle(GUI_LS_DASH);
		 GUI_SetTextMode(GUI_TM_TRANS);
			map_draw(__cursor.longitude,__cursor.latitude,measuring_scale[scale_choose]);
//		
				center.x=(MAP_LEFT+MAP_RIGHT)/2;
				center.y=(MAP_TOP+MAP_BOTTOM)/2;
		
			center.lgtude = __cursor.longitude;
			center.lttude = __cursor.latitude;
			
			
			GUI_SetLineStyle(GUI_LS_SOLID);
			GUI_SetColor(GUI_BLUE);
			disp_fish_boat(scale_choose,center.lgtude,center.lttude,center.x,center.y,boat_list_p,N_boat);
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
			Draw_ScaleRuler(MAP_RIGHT-200,MAP_BOTTOM-50,measuring_scale[scale_choose].scaleVal);
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


