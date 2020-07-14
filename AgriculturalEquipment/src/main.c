/* ****************************
 * 锟芥本锟斤拷V1.1
 * 说锟斤拷锟斤拷1.锟斤拷锟�4G锟斤拷位锟斤拷锟斤拷
 *
 * ***************************/

#include <stddef.h>
#include <stdio.h>
#include "UserConfig.h"
#include "stm32f10x.h"

//#include "FreeRTOS.h"
//#include "task.h"
//#include "queue.h"
//#include "timers.h"

//struct GPS			 		 //锟斤拷锟斤拷峁癸拷濉拷锟紾PS锟结构锟斤拷
//{
//	char time[10];		 	 //锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟绞憋拷锟街�
//	int hour;			 	 //锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟绞憋拷锟叫∈�
//	int minute;			 	 //锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟绞憋拷锟斤拷锟斤拷
//	int second;			 	 //锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟绞憋拷锟斤拷锟�
//	char N[20];			 	 //锟斤拷锟斤拷锟斤拷锟介—锟斤拷锟斤拷纬锟斤拷锟斤拷
//	char E[20];			 	 //锟斤拷锟斤拷锟斤拷锟介—锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
//	char status;			 //锟斤拷锟斤拷锟街疚伙拷锟斤拷锟紾PS锟斤拷息锟角凤拷刷锟斤拷
//	char time_new_ststus;	 //time时锟斤拷刷锟铰憋拷志位
//	char gps_new_status;	 //GPS刷锟铰憋拷志位
//} GPS_message = { .time = { 0 }, .hour = 0, .minute = 0, .second = 0, .N = { 0 }, .E = { 0 },
//        .status = 0 };

static char begin_mark = 0;														//锟斤拷锟斤拷锟街疚伙拷锟斤拷锟斤拷锟酵�$
static char GNRMC_count = 1;						//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷诩锟斤拷锟角凤拷锟秸碉拷锟斤拷锟斤拷GNRMC锟斤拷5锟斤拷锟斤拷之锟斤拷锟轿�1
static char douhao_count = 0;												//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟�
static char variable_i = 0;													//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷纸锟绞癸拷锟�
char CANerr = 0;														//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟紺AN锟届常
char GPSerr = 0;														//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟紾PS锟届常
int time_count = 0;														//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟绞憋拷锟斤拷锟斤拷锟斤拷耄�
#define CAN_er_time 2													//锟疥定锟藉—锟斤拷CAN锟斤拷时时锟斤拷锟借定
typedef enum
{
	ERR = 0, OK, ONLINE

} NB_STAT;

Cach1 Pack1;
Cach2 Pack2;
Cach3 Pack3;
Cach4 Pack4;
Cach5 Pack5;
Cach6 Pack6;
Cach7 Pack7;

char receives[15][10][100]; //锟斤拷支锟斤拷19锟斤拷指锟斤拷(1~19)[0锟斤拷锟斤拷锟斤拷锟斤拷],每锟斤拷指锟斤拷山锟斤拷锟�5锟斤拷锟斤拷锟斤拷锟斤拷息(0~4),每锟斤拷锟斤拷锟斤拷锟斤拷息锟斤拷锟斤拷锟筋长98(0~97)[99锟斤拷为锟斤拷锟斤拷锟斤拷息锟斤拷锟斤拷锟斤拷锟捷硷拷录位]
uint8_t x_axis = 0, y_axis = 0, cmd_axis = 0;

int count = 0;

GPS_DATA GPSDATA = { .ATW = false };
GPS_INIT GPSINIT = { .matchCount = 0, .cmdHead = false, .match[0]='G', .match[1]='N', .match[2]='R',
        .match[3]='M', .match[4]='C', .splitTime = 0, .dataCount = 0 };

void anaGPS();
void IWDG_Config(uint8_t prv, uint16_t rlv)
{
	// 使锟斤拷 预锟斤拷频锟侥达拷锟斤拷PR锟斤拷锟斤拷装锟截寄达拷锟斤拷RLR锟斤拷写
	IWDG_WriteAccessCmd( IWDG_WriteAccess_Enable);

	// 锟斤拷锟斤拷预锟斤拷频锟斤拷值
	IWDG_SetPrescaler(prv);

	// 锟斤拷锟斤拷锟斤拷装锟截寄达拷锟斤拷值
	IWDG_SetReload(rlv);

	// 锟斤拷锟斤拷装锟截寄达拷锟斤拷锟斤拷值锟脚碉拷锟斤拷锟斤拷锟斤拷锟斤拷
	IWDG_ReloadCounter();

	// 使锟斤拷 IWDG
	IWDG_Enable();
}

void IWDG_Feed(void)
{
	// 锟斤拷锟斤拷装锟截寄达拷锟斤拷锟斤拷值锟脚碉拷锟斤拷锟斤拷锟斤拷锟叫ｏ拷喂锟斤拷锟斤拷锟斤拷止IWDG锟斤拷位
	// 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷值锟斤拷锟斤拷0锟斤拷时锟斤拷锟斤拷锟斤拷系统锟斤拷位
	IWDG_ReloadCounter();
}

int gps_count = 0, can_count = 0;
void time_break_function()	//RTC锟斤拷锟斤拷锟斤拷
{
	time_count++;
	gps_count++;
	can_count++;
}

int main(void)
{
	nvic_init();
	GPSINIT.ATR = false;

	pinModeB(GPIO_Pin_5, OUTPUT);
	digitalWriteB(GPIO_Pin_5, HIGH);
	pinModeC(GPIO_Pin_14, OUTPUT);
	digitalWriteC(GPIO_Pin_14, HIGH);

	delay_us(12000000);	//15s
	usart_1_init(115200);
//	printf("ok");
//	IWDG_Config(IWDG_Prescaler_256, 4095);
	BC28_Init();
	CAN_INIT();	//锟斤拷始锟斤拷CAN锟斤拷锟斤拷
	CAN_IT_INIT();	//锟斤拷始锟斤拷CAN锟斤拷锟斤拷锟叫讹拷
	usart_3_init(9600, 0);
//	USART_ITConfig(USART3, USART_IT_ERR, ENABLE);
	IWDG_Feed();
	RTC_1s_it_init();
	RTC_Handler(time_break_function);
	RTC_IT_ON;
	while (1)
	{
		Pack1.Mail_Box[1].ATW = true;
		Pack2.Mail_Box[1].ATW = true;
		Pack3.Mail_Box[1].ATW = true;
		Pack4.Mail_Box[1].ATW = true;
		Pack5.Mail_Box[1].ATW = true;
		Pack6.Mail_Box[1].ATW = true;
		Pack7.Mail_Box[1].ATW = true;
		IWDG_Feed();
		GPSDATA.ATW = true;

		while (!(Pack1.Mail_Box[1].ATW == false || Pack2.Mail_Box[1].ATW == false
		        || Pack3.Mail_Box[1].ATW == false || Pack4.Mail_Box[1].ATW == false
		        || Pack5.Mail_Box[1].ATW == false || Pack6.Mail_Box[1].ATW == false
		        || Pack7.Mail_Box[1].ATW == false))
		{
			if (can_count > CAN_er_time)
			{
				can_count = 0;
				CANerr = 1;
				break;
			}
			IWDG_Feed();

			CANerr = 0;				//CAN锟睫达拷锟襟，革拷位锟斤拷志位锟斤拷锟斤拷位时锟斤拷锟斤拷锟�

		}
		anaGPS();
		while (GPSDATA.gps_new_status == 0)
		{
			anaGPS();
			IWDG_Feed();
			if (gps_count > 2 + CAN_er_time)		//GPS锟斤拷锟襟，打开达拷锟斤拷锟街疚伙拷锟斤拷锟轿皇憋拷锟斤拷锟斤拷
			{
				gps_count = 0;
				GPSerr = 1;
				break;
			}

			GPSerr = 0;						//GPS锟睫达拷锟襟，革拷位锟斤拷志位锟斤拷锟斤拷位时锟斤拷锟斤拷锟�
		}
		can_count = 0;
		gps_count = 0;

		pack_to_aliyun();

		IWDG_Feed();
		for(int i=0;i<3600;i++)
		{
			delay_us(1000000);
			IWDG_Feed();
		}
	}
}

void USART1_IRQHandler(void)
{
	char temp;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	//锟斤拷锟秸碉拷锟斤拷锟斤拷
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE); //锟斤拷锟斤拷锟斤拷锟斤拷卸媳锟街�
		temp = USART_ReceiveData(USART1); //锟斤拷锟秸达拷锟斤拷1锟斤拷锟捷碉拷buff锟斤拷锟斤拷锟斤拷
		receives[cmd_axis][y_axis][x_axis] = temp;
		x_axis++;
		if (temp == '\n')
		{
			receives[cmd_axis][y_axis][99] = x_axis;
			y_axis++;
			x_axis = 0;
		}
	}
}

char GpstempChar;
char GpsCharToConvert[20];
char datas[100];
int counts = 0;
bool reachD = false;
void USART3_IRQHandler(void)
{
//	GPSINIT.ATR=false;
	if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET) //注锟解！锟斤拷锟斤拷使锟斤拷if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)锟斤拷锟叫讹拷
	{
		USART_ReceiveData(USART3);
	}
	//锟斤拷锟节达拷锟斤拷锟斤拷一锟斤拷通讯锟斤拷锟斤拷锟秸碉拷锟斤拷锟捷诧拷锟斤拷锟斤拷
	if (GPSINIT.ATR == true)
	{
		USART_ReceiveData(USART3);
		USART_ClearITPendingBit(USART3, USART_IT_RXNE); //锟斤拷锟斤拷锟斤拷锟斤拷卸媳锟街�
		return;
	} else if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //锟斤拷锟秸碉拷锟斤拷锟斤拷
	{
		GpstempChar = USART_ReceiveData(USART3);
		if (GpstempChar == '$')
		{
			datas[0] = '$';
			reachD = true;
			counts = 1;
			return;
		}
		if (reachD)
		{

			if (GpstempChar == '\n')
			{
				counts = 0;
				reachD = false;
				USART_ClearITPendingBit(USART3, USART_IT_RXNE); //锟斤拷锟斤拷锟斤拷锟斤拷卸媳锟街�
				GPSINIT.ATR = true;
				GPSINIT.splitTime = 0;
				return;
			} else
			{
				datas[counts] = GpstempChar;
				counts++;
			}

		}
		USART_ClearITPendingBit(USART3, USART_IT_RXNE); //锟斤拷锟斤拷锟斤拷锟斤拷卸媳锟街�
	}
}

void anaGPS()
{
	static int num = 0;				//锟斤拷锟斤拷锟叫讹拷GPS锟斤拷值锟角凤拷刷锟斤拷锟斤拷锟�
	int x, y;
	GPSDATA.time_new_status = 0;
	GPSDATA.gps_new_status = 0;
	if (GPSDATA.ATW == false)
	{
		return;
	} else if (GPSINIT.ATR)
	{
		GPSINIT.splitTime = 0;
		clearStr(GpsCharToConvert, 20);
		GPSINIT.dataCount = 0;

		for (GPSINIT.matchCount = 1; GPSINIT.matchCount <= 5; GPSINIT.matchCount++)
		{
			if (datas[GPSINIT.matchCount] != GPSINIT.match[GPSINIT.matchCount - 1])
			{
				clearStr(datas, 100);
				GPSINIT.ATR = false;
				GPSINIT.splitTime = 0;
				return;
			}
		}

		for (x = 0, y = 0; datas[x] != '\r'; x++)
		{
			if (datas[x] != ',')
			{
				GpsCharToConvert[y] = datas[x];
				y++;
			} else
			{
				y = 0;
				GPSINIT.splitTime++;
				switch (GPSINIT.splitTime)
				{
					case 2:
					{
						writeUTC(GPSDATA.UTCtime, GpsCharToConvert);
						if (GpsCharToConvert[0] != '\0')
						{
							GPSDATA.time_new_status = 1;				//时锟斤拷锟斤拷锟剿拷锟斤拷锟斤拷
						} else
						{
							GPSDATA.time_new_status = 0;				//时锟斤拷锟斤拷锟剿拷锟斤拷锟斤拷
						}
						clearStr(GpsCharToConvert, 20);
						GPSINIT.dataCount = 0;
						GPSDATA.ATW = false;
						continue;
					}

					case 3:
						GPSDATA.AorP = GpsCharToConvert[0], clearStr(GpsCharToConvert, 20), GPSINIT.dataCount =
						        0;
						continue;
					case 4:
					{
						writeL(GPSDATA.latitude, GpsCharToConvert);
						if (GpsCharToConvert[0] != '\0')
						{
							num++;
						}
						clearStr(GpsCharToConvert, 20), GPSINIT.dataCount = 0;
						continue;
					}

					case 5:
						GPSDATA.NorS = GpsCharToConvert[0], clearStr(GpsCharToConvert, 20), GPSINIT.dataCount =
						        0;
						continue;
					case 6:
						writeL(GPSDATA.longitude, GpsCharToConvert);
						if (GpsCharToConvert[0] != '\0' && num == 1)
						{
							num = 0;								//锟斤拷志位锟斤拷锟斤拷
							GPSDATA.gps_new_status = 1;				//GPS锟斤拷锟斤拷刷锟斤拷锟斤拷锟�
						} else
						{
							num = 0;								//锟斤拷志位锟斤拷锟斤拷
							GPSDATA.gps_new_status = 0;				//GPS锟斤拷锟斤拷刷锟斤拷锟斤拷锟�
						}
						clearStr(GpsCharToConvert, 20), GPSINIT.dataCount = 0;
						continue;
					case 7:
						GPSDATA.EorW = GpsCharToConvert[0], clearStr(GpsCharToConvert, 20), GPSINIT.dataCount =
						        0;
						continue;
					case 8:
						GPSDATA.ATW = false, GPSINIT.ATR = false, GPSINIT.splitTime = 0;

						return;
				}
				clearStr(GpsCharToConvert, 20);
				GPSINIT.dataCount = 0;
				continue;
			}

		}
	}

}

