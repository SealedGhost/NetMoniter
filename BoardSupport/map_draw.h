#ifndef MAPDRAW_H
#define MAPDRAW_H



#define MAX_GEAR  27

#define MAP_LEFT   0
#define MAP_RIGHT  800
#define MAP_TOP    36
#define MAP_BOTTOM 480

#define MAP_LEFT_LG  6720000   /// E =112°
#define MAP_RIGHT_LG 8100000  ///  E = 135°
#define MAP_TOP_LT   2460000 ///   N =35°
#define MAP_BOTTOM_LT 210000  //   N =3.5°

#define ID_TIMER_MAP_REFRESH  0x01


#define MAP_WIDTH (MAP_RIGHT-MAP_LEFT)
#define MAP_HEIGHT (MAP_BOTTOM-MAP_TOP)

//#define map_width 800 //mapwidth
//#define map_height 480	//mapheight
#define num_fish 76		//the fish number array count
#define distance 30000 //used in display latitude and finish number calculate
#define mul_pow 1000   //I use 1000 times of the minute when calculate longtitude and latitude

#define fish_number_x_shift -10;  //the text can be move, tune this number to make in the middle or other place
#define fish_number_y_shift -30;

#define latitude_display_x_shift -80 //the latitude text can be move
#define latitude_display_y_shift 5

#define longitude_display_x_shift 0 //longitude can be move
#define longitude_display_y_shift 0

#define x_shift_10000 -16   //tune of small fish number text
#define y_shift_10000 0

#define small_fish_number 10000  //10 minute, one grid for a small fish
#define big_fish_number 30000   //30minute big

#define center_shift_x_small 0
#define center_shift_y_small 0 //center point(x,y) can be moved for fitting screen

#define flip_speed_long (MAP_WIDTH>>1)	//how much of the page you want to turn
#define flip_speed_lat (MAP_HEIGHT>>1)

#define speed_up_after 5 //200ms for a time count, after this number, speed up the cursor
#define CURSOR_SPEED_FAST 40 //how fast whent cursor run in a high speed
#define CURSOR_SPEED_SLOW 20
#define timer_millisecond 200

//#ifndef KEY_TIMER_MODE
// #define KEY_TIMER_MODE
//#endif

#define tip_width 160
#define tip_height 90

#define if_SM_fsh_num(num_digit) 	\
			if(scale.minute != small_fish_number)\
				GUI_DispDecAt(disp.num,disp.x,disp.y,num_digit);\
			else\
			{for(k=0;k<3;k++)\
				{for(l=0;l<3;l++)\
					{temp_x = disp.x+l*scale.pixel;\
						temp_y = disp.y+k*scale.pixel;\
						if((temp_x>MAP_LEFT)&&(temp_y>MAP_TOP)&&(temp_x<MAP_RIGHT-40)&&(temp_y<MAP_BOTTOM)){\
							GUI_DispDecAt(disp.num,temp_x,temp_y,num_digit);\
							GUI_DispCharAt(0x5f,temp_x + 30,temp_y);\
							GUI_DispDecAt(n,temp_x + 37,temp_y,1);}\
						n++;}\
				}\
				n=1;}

#define KEY_handle(key_num,sca,sig)	\
					__cursor.key = key_num;\
					if(time_delay == 0)\
					{time_delay = 1;\
						__cursor.sca = __cursor.sca sig __cursor.speed;\
						GUI_CURSOR_SetPosition(__cursor.x,__cursor.y);\
						timer_cursor =  WM_CreateTimer(hWin,0,timer_millisecond,0);}\
					else{\
						time_delay = 0;\
						WM_DeleteTimer(timer_cursor);\
						__cursor.speed = 1;}

#define board_handle	\
						__cursor.x = (MAP_LEFT+MAP_RIGHT)/2;\
						__cursor.y = (MAP_TOP+MAP_BOTTOM)/2;\
						GUI_CURSOR_SetPosition(__cursor.x,__cursor.y);\
						WM_InvalidateRect(hWin,pRect);
//						WM_InvalidateRect(hWin, pRect);

//						WM_InvalidateWindow(hWin); 
//						WM_Paint(hWin);
						

#define TIMER_handle 	\
						time_delay = 1;\
						board_handle
						
struct fishing_map{
	 long longitude_fish;
	 long latitude_fish;
	 short fish_number;
	 short exp_num;
};

struct scale_map{
	 short pixel;
	 long minute_km;
	 long scaleVal;
	 long minute;
};

struct display{
	 short x;
	 short y;
	 short num;
};
	
struct mapping{
	 long lgtude;
	 long lttude;
	 short x;
	 short y;
};
struct _cursor{
	short x;
	short y;
	long longitude;
	long latitude;
	short speed;
	char key;
	//struct scale_map scale;
	short wide;
	short height;
};

struct _map_revise{
	short x;
	short y;
};
typedef struct scale_map scale_map;
typedef struct fishing_map fishing_map;
typedef struct display display;
//typedef struct display display; Deleted by Xiaolu at 2015/4/27
typedef struct mapping mapping;
typedef struct _cursor _cursor;
typedef struct _map_revise _map_revise;

typedef short(*operation)(mapping, scale_map) ;

long null_operation(long temp,long flip);

//const static scale_map measuring_scale[5]={{110,30000},{120,10000},{130,3000},{100,106},{80,53}};
const static scale_map measuring_scale[MAX_GEAR+1]={
	                                          {160,   85,    50,   152},
																						{160,   169,   100,  305},
																						{160,   254,   150,  457},
																						{160,   339,   200,  610},
																						
																						{150,   397,   250,  714},
																						{150,   476,   300,  857},
																						{150,   794,   500,  1429},
																						
																						{140,  1188,   800,  2134},
																						{140,  1481,   1000, 2667},
																						{140,  2222,   1500, 4000},
																						
																						{130,  4126,   3000, 7430},
																						{130,  6877,   5000, 12383},
																						{130, 11032,   8000, 19812},
																						
																						{120, 15235,  12000, 27432},
																						{120, 20366,  16000, 36576},
																						{120, 25248,  20000, 45720},
																						
																						{110, 29095,  25000, 52388},
																						{110, 34914,  30000, 62865},
																						{110, 41897,  36000, 75438},
																						
																						{100, 44436,  42000, 80010},
																						{100, 50916,  48000, 91440},
																						{100, 59402,  56000, 106680},
																						
																						{90,  59036,  62000, 106299},
																						{90,  66654,  70000, 120015},
																						{90,  76374,  80000, 137160},
																						
																						{80,  84640,  100000,152400},
																						{80, 126960,  150000,228600},
																						{80, 211600,  250000,381000}
};
const static fishing_map fishing_area[num_fish] = 
{
{7230000,2460000,1,0}, //?????,????,?????
{7200000,2430000,5,0},
{7140000,2400000,10,0},
{7050000,2370000,20,0}, // ?????,????,????
{7050000,2340000,34,0},
{7050000,2310000,47,0},
{7110000,2280000,60,1},
{7110000,2250000,71,2}, //???? , ????,????
{7260000,2220000,77,2},
{7230000,2190000,83,2},
{7170000,2160000,90,2},//?????,?????,????
{7140000,2130000,99,14},
{7140000,2100000,109,14},
{7200000,2070000,119,14},
{7230000,2040000,127,9}, //????,????
{7230000,2010000,136,8},
{7260000,1980000,145,7},
{7290000,1950000,153,9},
{7290000,1920000,160,9}, //?????
{7290000,1890000,167,9},
{7260000,1860000,174,9}, //????
{7260000,1830000,182,9},
{7320000,1800000,190,9},
{7320000,1770000,196,8}, //????
{7290000,1740000,202,8},
{7260000,1710000,209,8},
{7230000,1680000,217,7}, //???? 
{7230000,1650000,226,6},
{7200000,1620000,235,6}, //????
{7170000,1590000,244,4},
{7170000,1560000,254,4}, //???? ,????
{7140000,1530000,263,4},
{7110000,1500000,273,0},
{7080000,1470000,282,0}, //????
{7050000,1440000,291,0},
{6990000,1410000,300,0},
{6900000,1380000,310,0}, //????,??????
{6810000,1350000,324,0},
{7020000,1320000,341,0}, //????,??????
{6480000,1290000,361,0},
{6420000,1260000,386,0},
{6390000,1230000,412,0},
{6360000,1200000,439,0},
{6360000,1170000,463,0},
{6360000,1140000,486,0},
{6390000,1110000,509,0},
{6420000,1080000,533,0},
{6450000,1050000,555,0},
{6540000,1020000,578,0},
{6570000,990000,598,0},
{6600000,960000,617,0},  //??????
{6630000,930000,635,0},
{6630000,900000,652,0},
{6630000,870000,669,0},
{6630000,840000,686,0},
{6630000,810000,703,0},
{6630000,780000,720,0},
{6630000,750000,737,0},
{6630000,720000,753,0},
{6630000,690000,769,0},
{6630000,660000,784,0},
{6630000,630000,799,0},
{6600000,600000,814,0}, // ????/????
{6570000,570000,829,0},
{6570000,540000,844,0},
{6540000,510000,858,0},
{6540000,480000,872,0},
{6540000,450000,885,0},
{6540000,420000,898,0},
{6570000,390000,910,0},
{6600000,360000,920,0},
{6630000,330000,929,0},
{6660000,300000,935,0}, // ??????
{6660000,270000,940,0},
{6690000,240000,944,0},
{6690000,210000,946,0},
};


#endif
