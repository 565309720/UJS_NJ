/*
 * Servers.c
 *
 *  Created on: 2020Äê4ÔÂ26ÈÕ
 *      Author: WangXiang
 */
#include "Servers.h"
#include "UserConfig.h"
#include <stdio.h>
//text
extern GPS_DATA GPSDATA;
extern Cach1 Pack1;
extern Cach2 Pack2;
extern Cach3 Pack3;
extern Cach4 Pack4;
extern Cach5 Pack5;
extern Cach6 Pack6;
extern Cach7 Pack7;
extern char CANerr;
extern char GPSerr;
ID_INFO ID_INFOs;

extern char receives[15][10][100]; //��֧��19��ָ��(1~19)[0��������],ÿ��ָ��ɽ���5��������Ϣ(0~4),ÿ��������Ϣ�����98(0~97)[99��Ϊ������Ϣ�������ݼ�¼λ]
extern uint8_t x_axis, y_axis, cmd_axis;
//static int time_count = 0;
extern int count;

/*
 * ����CAN�������ݰ���ID�����ݸ�ʽ
 */
void Analysys(CanRxMsg CANRX)
{
	switch (CANRX.ExtId)
	{
		case P1ID:
			Analysys_ID(1, CANRX), Analysys_DATA(get_pack_currentPack(CANRX), CANRX);
			break;
		case P2ID:
			Analysys_ID(2, CANRX), Analysys_DATA(get_pack_currentPack(CANRX), CANRX);
			break;
		case P3ID:
			Analysys_ID(3, CANRX), Analysys_DATA(get_pack_currentPack(CANRX), CANRX);
			break;
		case P4ID:
			Analysys_ID(4, CANRX), Analysys_DATA(get_pack_currentPack(CANRX), CANRX);
			break;
//		default:
	}
}

/*
 * �������ݰ�ID�е�Pri
 */
uint8_t set_Pri(uint32_t id)
{
	id = (id & 0xFF000000) >> 24;
	switch (id)
	{
		case 0x0C:
			return 3;
		case 0X10:
			return 4;
		default:
			return 5;
	}
}

/*
 * �������ݰ���ID
 * Pack_No:	�����,��Ӧ�ڼ������ݰ�,�������������ݽ������ڼ����ṹ����,��ӦEXCEL�����������(1~7)
 * CANRX:	���յ�CAN�������ݽṹ��
 */
void Analysys_ID(uint8_t Pack_NO, CanRxMsg CANRX)
{
	ID_INFOs.Pri = set_Pri(CANRX.ExtId);
	ID_INFOs.PGN = (CANRX.ExtId & 0x00FFFF00) >> 8;
	ID_INFOs.PF = (CANRX.ExtId & 0x00FF0000) >> 16;
	ID_INFOs.PSGE = (CANRX.ExtId & 0x0000FF00) >> 8;
	ID_INFOs.SA = (CANRX.ExtId & 0x000000FF);
	ID_INFOs.DLC = CANRX.DLC;
	if (Pack_NO == 1)
		Pack1.ID_INFO = ID_INFOs, Pack1.ID = CANRX.ExtId;
	else if (Pack_NO == 2)
		Pack2.ID_INFO = ID_INFOs, Pack2.ID = CANRX.ExtId;
	else if (Pack_NO == 3)
		Pack3.ID_INFO = ID_INFOs, Pack3.ID = CANRX.ExtId;
	else if (Pack_NO == 4)
		Pack4.ID_INFO = ID_INFOs, Pack4.ID = CANRX.ExtId;
	else if (Pack_NO == 5)
		Pack5.ID_INFO = ID_INFOs, Pack5.ID = CANRX.ExtId;
	else if (Pack_NO == 6)
		Pack6.ID_INFO = ID_INFOs, Pack6.ID = CANRX.ExtId;
	else if (Pack_NO == 7)
		Pack7.ID_INFO = ID_INFOs, Pack7.ID = CANRX.ExtId;
}

/*
 * ����EXCEL���������˳�򷵻����ǵڼ�������,����ʹ�ò�ͬ�������н�������
 */
uint8_t get_pack_currentPack(CanRxMsg CANRX)
{
	if (CANRX.ExtId == P1ID)
		return (CANRX.Data[0] & 0x0F);
	else
	{
		if (CANRX.ExtId == P2ID)
			return 5;
		if (CANRX.ExtId == P3ID)
			return 6;
		if (CANRX.ExtId == P4ID)
			return 7;
	}
	return 0;
}

/*
 * ��������
 * Pack_No:	ʹ�õڼ�����ʽ���,��ͬ��ŵ����ݰ���Ӧ��ͬ�����ݽ�����ʽ
 * CANRX:	���յ�CAN�������ݰ�
 *
 * !!ʹ��ǰ��Ҫ�Ƚ�Mail_Box����,����ATW��true!!
 */
void Analysys_DATA(uint8_t Pack_NO, CanRxMsg CANRX)
{
	int mail_box = 0;

	if (Pack_NO == 1)
	{
		if (Pack1.Mail_Box[0].ATW)
			mail_box = 0;
		else if (Pack1.Mail_Box[1].ATW)
			mail_box = 1;
		else if (Pack1.Mail_Box[2].ATW)
			mail_box = 2;
		Pack1.Mail_Box[mail_box].total_pack = (CANRX.Data[0] & 0xF0) >> 4;
		Pack1.Mail_Box[mail_box].current_pack = (CANRX.Data[0] & 0x0F);
		Pack1.Mail_Box[mail_box].whell_speed = (CANRX.Data[1] & 0xFE) >> 1;
		Pack1.Mail_Box[mail_box].is_on_work = (CANRX.Data[1] & 0x01);
		Pack1.Mail_Box[mail_box].ultrasonic_sensor = (CANRX.Data[3] << 8) | CANRX.Data[2];
		Pack1.Mail_Box[mail_box].rotating_speed = CANRX.Data[4];
		Pack1.Mail_Box[mail_box].drive_speed = (CANRX.Data[6] << 8) | CANRX.Data[5];
		Pack1.Mail_Box[mail_box].car_speed = CANRX.Data[7];
		Pack1.Mail_Box[mail_box].ATW = false;
	} else if (Pack_NO == 2)
	{
		if (Pack2.Mail_Box[0].ATW)
			mail_box = 0;
		else if (Pack2.Mail_Box[1].ATW)
			mail_box = 1;
		else if (Pack2.Mail_Box[2].ATW)
			mail_box = 2;
		Pack2.Mail_Box[mail_box].total_pack = (CANRX.Data[0] & 0xF0) >> 4;
		Pack2.Mail_Box[mail_box].current_pack = (CANRX.Data[0] & 0x0F);
		Pack2.Mail_Box[mail_box].roller_speed = (CANRX.Data[2] << 8) | CANRX.Data[1];
		Pack2.Mail_Box[mail_box].Yroller_speed = (CANRX.Data[4] << 8) | CANRX.Data[3];
		Pack2.Mail_Box[mail_box].wind_speed = (CANRX.Data[6] << 8) | CANRX.Data[6];
		Pack2.Mail_Box[mail_box].driver_speed = CANRX.Data[7];
		Pack2.Mail_Box[mail_box].ATW = false;
	} else if (Pack_NO == 3)
	{
		if (Pack3.Mail_Box[0].ATW)
			mail_box = 0;
		else if (Pack3.Mail_Box[1].ATW)
			mail_box = 1;
		else if (Pack3.Mail_Box[2].ATW)
			mail_box = 2;
		Pack3.Mail_Box[mail_box].total_pack = (CANRX.Data[0] & 0xF0) >> 4;
		Pack3.Mail_Box[mail_box].current_pack = (CANRX.Data[0] & 0x0F);
		Pack3.Mail_Box[mail_box].shock_speed = (CANRX.Data[2] << 8) | CANRX.Data[1];
		Pack3.Mail_Box[mail_box].Xroller_speed = (CANRX.Data[4] << 8) | CANRX.Data[3];
		Pack3.Mail_Box[mail_box].Xrest_speed = (CANRX.Data[6] << 8) | CANRX.Data[5];
		Pack3.Mail_Box[mail_box].keep = CANRX.Data[7];
		Pack3.Mail_Box[mail_box].ATW = false;
	} else if (Pack_NO == 4)
	{
		if (Pack4.Mail_Box[0].ATW)
			mail_box = 0;
		else if (Pack4.Mail_Box[1].ATW)
			mail_box = 1;
		else if (Pack4.Mail_Box[2].ATW)
			mail_box = 2;
		Pack4.Mail_Box[mail_box].total_pack = (CANRX.Data[0] & 0xF0) >> 4;
		Pack4.Mail_Box[mail_box].current_pack = (CANRX.Data[0] & 0x0F);
		Pack4.Mail_Box[mail_box].high = (CANRX.Data[2] << 8) | CANRX.Data[1];
		Pack4.Mail_Box[mail_box].ATW = false;
	} else if (Pack_NO == 5)
	{
		if (Pack5.Mail_Box[0].ATW)
			mail_box = 0;
		else if (Pack5.Mail_Box[1].ATW)
			mail_box = 1;
		else if (Pack5.Mail_Box[2].ATW)
			mail_box = 2;
		Pack5.Mail_Box[mail_box].cleanlost_sensor = (CANRX.Data[1] << 8) | CANRX.Data[0];
		Pack5.Mail_Box[mail_box].cliplost_sensor = (CANRX.Data[3] << 8) | CANRX.Data[2];
		Pack5.Mail_Box[mail_box].angle = CANRX.Data[4];
		Pack5.Mail_Box[mail_box].ATW = false;
	} else if (Pack_NO == 6)
	{
		if (Pack6.Mail_Box[0].ATW)
			mail_box = 0;
		else if (Pack6.Mail_Box[1].ATW)
			mail_box = 1;
		else if (Pack6.Mail_Box[2].ATW)
			mail_box = 2;
		Pack6.Mail_Box[mail_box].pure_value = CANRX.Data[0];
		Pack6.Mail_Box[mail_box].break_value = CANRX.Data[1];
		Pack6.Mail_Box[mail_box].ATW = false;
	} else if (Pack_NO == 7)
	{
		if (Pack7.Mail_Box[0].ATW)
			mail_box = 0;
		else if (Pack7.Mail_Box[1].ATW)
			mail_box = 1;
		else if (Pack7.Mail_Box[2].ATW)
			mail_box = 2;
		Pack7.Mail_Box[mail_box].float_value = CANRX.Data[0];
		Pack7.Mail_Box[mail_box].ATW = false;
	}
}

bool iscontants(char *str, char *cmd)
{
	int i = 0, start = 0, count = 0;
	uint8_t lens = strlen(cmd);
	for (i = 0; i < strlen(str); i++)
	{
		if (*(str + i) == *cmd)
		{
			if (*(str + i + 1) == *(cmd + 1))
			{
				start = i;
				break;
			}
		}
	}

	for (i = 0; i < lens; i++)
	{
		if (*(str + start + i) == *(cmd + i))
			count++;
		else
			break;
	}

	if (count == lens)
		return true;
	else
		return false;

}

void send_cmd(char *str)
{
	printf(str);
	cmd_axis++;
	x_axis = 0;
	y_axis = 0;
}

bool check_receives(uint8_t cmd_number, char *cmd)
{
	int i = 0, j = 0;
	char str[20] = { '\0' };
	for (i = 0; i < y_axis; i++)
	{
		for (j = 0; j < receives[cmd_number][i][99] - 2; j++)
		{
			str[j] = receives[cmd_number][i][j];
		}
		if (str[0] == '\0' || str[0] == '\r' || str[0] == '\n')
		{
			continue;
		} else if (iscontants(str, cmd))
			return true;
	}
	return false;
}

void BC28_RESET()
{
	digitalWriteB(GPIO_Pin_5, HIGH);
	delay_us(3000000);
	digitalWriteB(GPIO_Pin_5, LOW);
	NVIC_SystemReset();
}

void BC28_Init()
{
	count = 0;
	y_axis = 0;
	x_axis = 0;
	cmd_axis = 0;
	send_cmd("ATE0 \r\n");	//�ڶ���ָ��
	delay_us(1000000);
	while (!check_receives(1, "OK"))	//���ָ��2���ص��������Ƿ����"ok"
	{
		;
	}

	send_cmd(
	        "AT+QMTCFG=\"aliauth\",0,\"a1f2CH9BSx7\",\"ZRH_4G\",\"TEnbrWdkBXfLkca73A9Nhyzqe9o19HM6\" \r\n");//�ڶ���ָ��
	delay_us(1000000);	//1s

	send_cmd("AT+QMTOPEN=0,\"iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883 \r\n");	//�ڶ���ָ��
	delay_us(1000000);	//1s

	if (check_receives(3, "ERROR"))
	{
		BC28_RESET();
	}
	delay_us(200000);	//1s
	if (check_receives(3, "+QMTOPEN: 0,-1"))
	{
		BC28_RESET();
	}
	if (check_receives(3, "+QMTOPEN: 0,3"))
	{
		BC28_RESET();
	}
	if (check_receives(3, "+QMTOPEN: 0,2"))
	{
		BC28_RESET();
	}
//	delay_us(1000000);	//1s

	send_cmd("AT+QMTCONN=0,\"ZRH_4G\" \r\n");	//�ڶ���ָ��
	delay_us(5000000);	//1s
	if (check_receives(4, "ERROR"))	//���ָ��2���ص��������Ƿ����"ok"
	{
		BC28_RESET();
	}
	if (check_receives(4, "+QMTCONN: 0,1"))
	{
		BC28_RESET();
	}
	cmd_axis = 12;	//ʹ��12���������Ƿ��ͳɹ���
	printf("AT+QMTPUB=0,0,0,1,\"/a1f2CH9BSx7/ZRH_4G/user/put\" \r\n");
	delay_us(100000);	//0.1s
	x_axis = 0;
	y_axis = 0;
	printf("~ \r\n");

	delay_us(500000);	//0.5s
	while (!check_receives(12, "+QMTPUB: 0,0,0"))
	{
		if (check_receives(12, "ERROR"))	//���ָ��2���ص��������Ƿ����"ok"
		{
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 99; j++)
					receives[12][i][j] = '\0';
			}
			BC28_Init();
			break;
		}
	}
	digitalWriteC(GPIO_Pin_14, LOW);
	delay_us(1000000);	//1s

}

void pack_to_aliyun()
{
	printf("AT+QMTPUB=0,0,0,1,\"/a1f2CH9BSx7/ZRH_4G/user/put\" \r\n");
	delay_us(100000);	//0.1s
	x_axis = 0;
	y_axis = 0;
	printf(
	        "{\"Mark\":\"A1001\",\"Time\":\"%s\",\"N\":\"%s\",\"E\":\"%s\",\"Bohelun\":\"%d\",\"Zuoye\":\"%d\",\"Fukuan\":\"%d\",\"Getai\":\"%d\",\"Shusongzhou\":\"%d\",\"Chesu\":\"%d\",\"QieLTL\":\"%d\",\"ZongZTL\":\"%d\",\"FongJZS\":\"%d\",\"QuDL\":\"%d\",\"ZhengDS\":\"%d\",\"LiZSP\":\"%d\",\"ZaYSP\":\"%d\",\"GeCGD\":\"%d\",\"QinXSS\":\"%d\",\"JiaDSS\":\"%d\",\"YuLSD\":\"%d\",\"HanZL\":\"%d\",\"PoSL\":\"%d\",\"LiZLL\":\"%d\",\"CANerr\":\"%d\",\"GPSerr\":\"%d\"} \r\n",
	        GPSDATA.UTCtime, GPSDATA.latitude, GPSDATA.longitude, Pack1.Mail_Box[1].whell_speed,
	        Pack1.Mail_Box[1].is_on_work, Pack1.Mail_Box[1].ultrasonic_sensor,
	        Pack1.Mail_Box[1].rotating_speed, Pack1.Mail_Box[1].drive_speed,
	        Pack1.Mail_Box[1].car_speed, Pack2.Mail_Box[1].roller_speed,
	        Pack2.Mail_Box[1].Yroller_speed, Pack2.Mail_Box[1].wind_speed,
	        Pack2.Mail_Box[1].driver_speed, Pack3.Mail_Box[1].shock_speed,
	        Pack3.Mail_Box[1].Xroller_speed, Pack3.Mail_Box[1].Xrest_speed, Pack4.Mail_Box[1].high,
	        Pack5.Mail_Box[1].cleanlost_sensor, Pack5.Mail_Box[1].cliplost_sensor,
	        Pack5.Mail_Box[1].angle, Pack6.Mail_Box[1].pure_value, Pack6.Mail_Box[1].break_value,
	        Pack7.Mail_Box[1].float_value, CANerr, GPSerr);

	delay_us(500000);	//0.5s
	while (!check_receives(12, "+QMTPUB: 0,0,0"))
	{
		if (check_receives(12, "ERROR"))	//���ָ��2���ص��������Ƿ����"ok"
		{
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 99; j++)
					receives[12][i][j] = '\0';
			}
			BC28_Init();
			break;
		}
	}
}

void writeUTC(char *time, char *data)
{
	time[0] = data[0];
	time[1] = data[1];
	time[2] = ':';
	time[3] = data[2];
	time[4] = data[3];
	time[5] = ':';
	time[6] = data[4];
	time[7] = data[5];
}

void writeL(char *lan, char *data)
{
	int i = 0;
	for (; data[i] != '\0'; i++)
	{
		lan[i] = data[i];
	}
}

void clearStr(char *str, uint8_t i)
{
	uint8_t j = 0;
	for (; j < i; j++)
	{
		str[j] = '\0';
	}
}
