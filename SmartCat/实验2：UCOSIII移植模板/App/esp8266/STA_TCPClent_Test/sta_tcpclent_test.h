#ifndef _sta_tcpclent_test_H
#define _sta_tcpclent_test_H


#include "system.h"


#define User_ESP8266_SSID	  "TP-LINK_5FD8"	      //要连接的热点的名称
#define User_ESP8266_PWD	  "zxmlyt159753"	  //要连接的热点的密码

#define User_ESP8266_TCPServer_IP	  "192.168.0.100"	  //要连接的服务器的IP
#define User_ESP8266_TCPServer_PORT	  "8080"	  //要连接的服务器的端口


extern volatile uint8_t TcpClosedFlag;  //定义一个全局变量


void ESP8266_STA_TCPClient_Test(void);



#endif
