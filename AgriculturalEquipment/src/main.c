/* ****************************
 * �汾��V1.1
 * ˵����1.���4G��λ����
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

//struct GPS			 		 //����ṹ�塪��GPS�ṹ��
//{
//	char time[10];		 	 //���������������ʱ��ֵ
//	int hour;			 	 //���������������ʱ��Сʱ
//	int minute;			 	 //���������������ʱ�����
//	int second;			 	 //���������������ʱ����
//	char N[20];			 	 //�������顪����γ����
//	char E[20];			 	 //�������顪����������
//	char status;			 //�����־λ����GPS��Ϣ�Ƿ�ˢ��
//	char time_new_ststus;	 //timeʱ��ˢ�±�־λ
//	char gps_new_status;	 //GPSˢ�±�־λ
//} GPS_message = { .time = { 0 }, .hour = 0, .minute = 0, .second = 0, .N = { 0 }, .E = { 0 },
//        .status = 0 };

static char begin_mark = 0;														//�����־λ������ͷ$
static char GNRMC_count = 1;						//��������������ڼ���Ƿ��յ�����GNRMC��5����֮���Ϊ1
static char douhao_count = 0;												//�������������������
static char variable_i = 0;													//����������������ֽ�ʹ��
char CANerr = 0;														//�����������CAN�쳣
char GPSerr = 0;														//�����������GPS�쳣
int time_count = 0;														//�������������ʱ�������룩
#define CAN_er_time 2													//�궨�塪��CAN��ʱʱ���趨
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

char receives[15][10][100]; //��֧��19��ָ��(1~19)[0��������],ÿ��ָ��ɽ���5��������Ϣ(0~4),ÿ��������Ϣ�����98(0~97)[99��Ϊ������Ϣ�������ݼ�¼λ]
uint8_t x_axis = 0, y_axis = 0, cmd_axis = 0;

int count = 0;

GPS_DATA GPSDATA = { .ATW = false };
GPS_INIT GPSINIT = { .matchCount = 0, .cmdHead = false, .match[0]='G', .match[1]='N', .match[2]='R',
        .match[3]='M', .match[4]='C', .splitTime = 0, .dataCount = 0 };

void anaGPS();
void IWDG_Config(uint8_t prv, uint16_t rlv)
{
	// ʹ�� Ԥ��Ƶ�Ĵ���PR����װ�ؼĴ���RLR��д
	IWDG_WriteAccessCmd( IWDG_WriteAccess_Enable);

	// ����Ԥ��Ƶ��ֵ
	IWDG_SetPrescaler(prv);

	// ������װ�ؼĴ���ֵ
	IWDG_SetReload(rlv);

	// ����װ�ؼĴ�����ֵ�ŵ���������
	IWDG_ReloadCounter();

	// ʹ�� IWDG
	IWDG_Enable();
}

void IWDG_Feed(void)
{
	// ����װ�ؼĴ�����ֵ�ŵ��������У�ι������ֹIWDG��λ
	// ����������ֵ����0��ʱ������ϵͳ��λ
	IWDG_ReloadCounter();
}

int gps_count = 0, can_count = 0;
void time_break_function()	//RTC������
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
	digitalWriteB(GPIO_Pin_5, LOW);
	pinModeC(GPIO_Pin_14, OUTPUT);
	digitalWriteC(GPIO_Pin_14, HIGH);

	delay_us(12000000);	//15s
	usart_1_init(115200);
//	printf("ok");
//	IWDG_Config(IWDG_Prescaler_256, 4095);
	BC28_Init();
	CAN_INIT();	//��ʼ��CAN����
	CAN_IT_INIT();	//��ʼ��CAN�����ж�
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

			CANerr = 0;				//CAN�޴��󣬸�λ��־λ����λʱ�����

		}
		anaGPS();
		while (GPSDATA.gps_new_status == 0)
		{
			anaGPS();
			IWDG_Feed();
			if (gps_count > 2 + CAN_er_time)		//GPS���󣬴򿪴����־λ����λʱ�����
			{
				gps_count = 0;
				GPSerr = 1;
				break;
			}

			GPSerr = 0;						//GPS�޴��󣬸�λ��־λ����λʱ�����
		}
		can_count = 0;
		gps_count = 0;

		pack_to_aliyun();

		IWDG_Feed();
//		delay_us(1000000);
	}
}

void USART1_IRQHandler(void)
{
	char temp;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	//���յ�����
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE); //��������жϱ�־
		temp = USART_ReceiveData(USART1); //���մ���1���ݵ�buff������
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
	if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET) //ע�⣡����ʹ��if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)���ж�
	{
		USART_ReceiveData(USART3);
	}
	//���ڴ�����һ��ͨѶ�����յ����ݲ�����
	if (GPSINIT.ATR == true)
	{
		USART_ReceiveData(USART3);
		USART_ClearITPendingBit(USART3, USART_IT_RXNE); //��������жϱ�־
		return;
	} else if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //���յ�����
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
				USART_ClearITPendingBit(USART3, USART_IT_RXNE); //��������жϱ�־
				GPSINIT.ATR = true;
				GPSINIT.splitTime = 0;
				return;
			} else
			{
				datas[counts] = GpstempChar;
				counts++;
			}

		}
		USART_ClearITPendingBit(USART3, USART_IT_RXNE); //��������жϱ�־
	}
}

void anaGPS()
{
	static int num = 0;				//�����ж�GPS��ֵ�Ƿ�ˢ�����
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
							GPSDATA.time_new_status = 1;				//ʱ�����ˢ�����
						} else
						{
							GPSDATA.time_new_status = 0;				//ʱ�����ˢ�����
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
							num = 0;								//��־λ����
							GPSDATA.gps_new_status = 1;				//GPS����ˢ�����
						} else
						{
							num = 0;								//��־λ����
							GPSDATA.gps_new_status = 0;				//GPS����ˢ�����
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

