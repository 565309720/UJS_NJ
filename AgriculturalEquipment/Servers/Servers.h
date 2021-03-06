/*
 * Servers.h
 *
 *  Created on: 2020年4月26日
 *      Author: WangXiang
 */

#ifndef SERVERS_H_
#define SERVERS_H_
#include "UserConfig.h"

#define P1ID 0x0CFF1081
#define P2ID 0x10FF1182
#define P3ID 0x10FF1283
#define P4ID 0x10FF1384

/*
 * ID解析之后数据结构体格式
 */
typedef struct
{
	uint8_t Pri;
	uint16_t PGN;
	uint8_t PF;
	uint8_t PSGE;
	uint8_t SA;
	uint8_t DLC;
} ID_INFO;

/*
 * EXCEL表中第1条命令的解析格式
 */
typedef struct
{
	uint8_t total_pack; 			//总包数
	uint8_t current_pack; 			//当前包序号
	uint8_t whell_speed; 			//拨禾轮转速/转速传感器
	bool is_on_work; 				//作业状态
	uint16_t ultrasonic_sensor; 	//幅宽/超声波传感器
	uint8_t rotating_speed; 		//割台搅龙转速/转速传感器
	uint16_t drive_speed; 			//输送槽主动轴转速/转速传感器
	uint8_t car_speed; 				//车速/车速雷达传感器
	bool ATW;
} Mail_Box1;

/*
 * EXCEL表中第2条命令的解析格式
 */
typedef struct
{
	uint8_t total_pack;				//总包数
	uint8_t current_pack;			//当前包序号
	uint16_t roller_speed;			//切流脱粒滚筒/转速传感器
	uint16_t Yroller_speed;			//纵轴流脱粒滚筒/转速传感器
	uint16_t wind_speed;			//风机转速/转速传感器
	uint8_t driver_speed;			//驱动轮转速/转速传感器
	bool ATW;
} Mail_Box2;

/*
 * EXCEL表中第3条命令的解析格式
 */
typedef struct
{
	uint8_t total_pack;				//总包数
	uint8_t current_pack;			//当前包序号
	uint16_t shock_speed;			//振动筛驱动轴转速/转速传感器
	uint16_t Xroller_speed;			//籽粒水平搅龙转速/转速传感器
	uint16_t Xrest_speed;			//杂余水平搅龙转/转速传感器
	uint8_t keep;					//保留
	bool ATW;
} Mail_Box3;

/*
 * EXCEL表中第4条命令的解析格式
 */
typedef struct
{
	uint8_t total_pack;				//总包数
	uint8_t current_pack;			//当前包序号
	uint16_t high;					//割茬高度/割茬高度传感器
	bool ATW;
} Mail_Box4;

/*
 * EXCEL表中第5条命令的解析格式
 */
typedef struct
{
	uint16_t cleanlost_sensor;		//清选损失/清选损失传感器
	uint16_t cliplost_sensor;		//夹带损失/夹带损失传感器
	uint8_t angle;
	bool ATW;
} Mail_Box5;

/*
 * EXCEL表中第6条命令的解析格式
 */
typedef struct
{
	uint8_t pure_value;				//含杂率/含杂率传感器
	uint8_t break_value;			//破碎率/破碎率传感器
	bool ATW;
} Mail_Box6;

/*
 * EXCEL表中第7条命令的解析格式
 */
typedef struct
{
	uint8_t float_value;			//籽粒流量/籽粒流量监测传感器
	bool ATW;
} Mail_Box7;

/*
 * EXCEL表中第1条命令的缓存
 */
typedef struct
{
	ID_INFO ID_INFO;				//ID数据解析结构体
	Mail_Box1 Mail_Box[3];			//数据缓存区
	uint32_t ID;					//完整ID
} Cach1;

/*
 * EXCEL表中第2条命令的缓存
 */
typedef struct
{
	ID_INFO ID_INFO;
	Mail_Box2 Mail_Box[3];
	uint32_t ID;
} Cach2;

/*
 * EXCEL表中第3条命令的缓存
 */
typedef struct
{
	ID_INFO ID_INFO;
	Mail_Box3 Mail_Box[3];
	uint32_t ID;
} Cach3;

/*
 * EXCEL表中第4条命令的缓存
 */
typedef struct
{
	ID_INFO ID_INFO;
	Mail_Box4 Mail_Box[3];
	uint32_t ID;
} Cach4;

/*
 * EXCEL表中第5条命令的缓存
 */
typedef struct
{
	ID_INFO ID_INFO;
	Mail_Box5 Mail_Box[3];
	uint32_t ID;
} Cach5;

/*
 * EXCEL表中第6条命令的缓存
 */
typedef struct
{
	ID_INFO ID_INFO;
	Mail_Box6 Mail_Box[3];
	uint32_t ID;
} Cach6;

/*
 * EXCEL表中第7条命令的缓存
 */
typedef struct
{
	ID_INFO ID_INFO;
	Mail_Box7 Mail_Box[3];
	uint32_t ID;
} Cach7;

typedef struct
{
	char UTCtime[10];
	char latitude[15];
	char longitude[15];					//经度
	char NorS;
	char EorW;
	char AorP;
	bool ATW;
	char time_new_status;	 //time时间刷新标志位。0未刷新，1刷新
	char gps_new_status;	 //GPS刷新标志位。0未刷新，1刷新
} GPS_DATA;

typedef struct
{
	bool cmdHead;
	uint8_t matchCount;
	uint8_t dataCount;
	char match[5];
	uint8_t splitTime;
	uint8_t totalmatch;
	bool ATR;

} GPS_INIT;

void Analysys(CanRxMsg CANRX);
void Analysys_ID(uint8_t Pack_NO, CanRxMsg CANRX);
uint8_t set_Pri(uint32_t id);
void Analysys_DATA(uint8_t Pack_NO, CanRxMsg CANRX);

bool iscontants(char *str, char *cmd);
void send_cmd(char *str);
bool check_receives(uint8_t cmd_number, char *cmd);
void BC28_RESET();
void BC28_Init();
void pack_to_aliyun();

void writeUTC(char *time, char *data);
void writeL(char *lan, char *data);
void clearStr(char *str, uint8_t i);

uint8_t get_pack_currentPack(CanRxMsg CANRX);

#endif /* SERVERS_H_ */
