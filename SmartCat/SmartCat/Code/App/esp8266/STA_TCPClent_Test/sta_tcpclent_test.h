#ifndef _sta_tcpclent_test_H
#define _sta_tcpclent_test_H


#include "system.h"


#define User_ESP8266_SSID	  "TP-LINK_5FD8"	      //Ҫ���ӵ��ȵ������
#define User_ESP8266_PWD	  "zxmlyt159753"	  //Ҫ���ӵ��ȵ������

#define User_ESP8266_TCPServer_IP	  "api.k780.com"	  //Ҫ���ӵķ�������IP
#define User_ESP8266_TCPServer_PORT	  "80"	  //Ҫ���ӵķ������Ķ˿�


extern volatile uint8_t TcpClosedFlag;  //����һ��ȫ�ֱ���


void ESP8266_STA_TCPClient_Test(void);



#endif
