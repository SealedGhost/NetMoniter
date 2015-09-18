typedef struct message_18 message_18;
typedef struct message_24 message_24;
typedef struct message_24_partA message_24_partA;
typedef struct message_24_partB message_24_partB;
typedef struct vender_id vender_id;
typedef struct call_sign call_sign;
typedef struct dimension_of_ship dimension_of_ship;
typedef struct type_of_ship type_of_ship;
struct message_18{
  int message_id;
  int repeat_indicator;
  long user_id;//MMSI
  int spare;
  short SOG;//speed over ground
  int position_accuracy;
  long longitude;//longitude
  long latitude;//latitude
  short COG;//course over ground
  short true_heading;//true heading
  int time_stamp;
  int spare2;
  int class_B_unit_flag;
  int class_B_display_flag;
  int class_B_DSC_flag;
  int class_B_band_flag;
  int class_B_message_22_flag;
  int mode_flag;
  int RAIM_flag;
  int communication_state_selector_flag;
  int communication_state;
};
struct message_24_partA{
  int message_id;
  int repeat_indicator;
  long user_id;
  int part_number;
  char name[20];
};
//struct vender_id{
//  char manufacturer_id[3];
//  int unit_model_code;
//  unsigned long unit_serial_number;
//};
struct call_sign{
	int f;
};
struct dimension_of_ship{
  int A;
  int B;
  int C;
  int D;
};
// struct message_24_partB{
//   int message_id;
//   int repeat_indicator;
//   unsigned long user_id;
//   int part_number;
//   int type_of_ship_and_cargo_type;
//   vender_id vender_id_;
// 	call_sign call_sign_;
//   dimension_of_ship dimension_of_ship_;
//   int type_of_electronic_position_fixing_device;
//   int spare;
// };

struct type_of_ship{
	int message_id;
//	int repeat_indicator;
	unsigned long user_id;
 unsigned char vender_id[7];
//int part_number;
//int type_of_ship_and_cargo_type;
};
