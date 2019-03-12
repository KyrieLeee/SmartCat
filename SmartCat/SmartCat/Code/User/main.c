#include "system.h"
#include "led.h"
#include "includes.h"
#include "tftlcd.h"
#include "SysTick.h"
#include "usart.h"		
#include "esp8266_drive.h"
#include "sta_tcpclent_test.h"
#include "oled.h"

//�������ȼ�
#define START_TASK_PRIO		3
//�����ջ��С	
#define START_STK_SIZE 		128
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);

//�������ȼ�
#define LED1_TASK_PRIO		6
//�����ջ��С	
#define LED1_STK_SIZE 		96
//������ƿ�
OS_TCB Led1TaskTCB;
//�����ջ	
CPU_STK LED1_TASK_STK[LED1_STK_SIZE];
void led1_task(void *p_arg);

//�������ȼ�
#define TFTLCD_TASK_PRIO		4
//�����ջ��С	
#define TFTLCD_STK_SIZE 		128
//������ƿ�
OS_TCB TFTLCDTaskTCB;
//�����ջ	
CPU_STK TFTLCD_TASK_STK[TFTLCD_STK_SIZE];
void  tftlcd_task(void *p_arg);

//�������ȼ�
#define ESP8266_TASK_PRIO		5
//�����ջ��С	
#define ESP8266_STK_SIZE 		384
//������ƿ�
OS_TCB ESP8266TaskTCB;
//�����ջ	
CPU_STK ESP8266_TASK_STK[ESP8266_STK_SIZE];
void  esp8266_task(void *p_arg);
int main()
{  	
	OS_ERR err;
//	Systick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	LED_Init();
//	USART1_Init(9600);
	
	OSInit(&err);		//��ʼ��UCOSIII
	//������ʼ����
	
	
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ 
	OSStart(&err);  //����UCOSIII
	while(1);
}

//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	
	CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
	
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
	
	cpu_clk_freq = BSP_CPU_ClkFreq();                           /* Determine SysTick reference freq.                    */
    cnts = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        /* Determine nbr SysTick increments                     */
    OS_CPU_SysTickInit(cnts);                                   /* Init uC/OS periodic time src (SysTick).              */

    Mem_Init();       
	
	
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//�����ٽ���
	//����LED0����
		OSTaskCreate((OS_TCB 	* )&Led1TaskTCB,		
				 (CPU_CHAR	* )"led1 task", 		
                 (OS_TASK_PTR )led1_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )LED1_TASK_PRIO,     
                 (CPU_STK   * )&LED1_TASK_STK[0],	
                 (CPU_STK_SIZE)LED1_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED1_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);		
								 
	OSTaskCreate((OS_TCB 	* )&TFTLCDTaskTCB,		
					(CPU_CHAR	* )"tftlcd task", 		
                 (OS_TASK_PTR )tftlcd_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )TFTLCD_TASK_PRIO,     
                 (CPU_STK   * )&TFTLCD_TASK_STK[0],	
                 (CPU_STK_SIZE)TFTLCD_STK_SIZE/10,	
                 (CPU_STK_SIZE)TFTLCD_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);	
								 
	OSTaskCreate((OS_TCB 	* )&ESP8266TaskTCB,		
					(CPU_CHAR	* )"esp8266 task", 		
                 (OS_TASK_PTR )esp8266_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )ESP8266_TASK_PRIO,     
                 (CPU_STK   * )&ESP8266_TASK_STK[0],	
                 (CPU_STK_SIZE)ESP8266_STK_SIZE/10,	
                 (CPU_STK_SIZE)ESP8266_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);								 

	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//����ʼ����			 
	OS_CRITICAL_EXIT();	//�����ٽ���
}

//led1������
void led1_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
		led1=!led1;
		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ200ms
	}
}

void  tftlcd_task(void *p_arg)
{
	unsigned	char * sttr;
	unsigned int count;
	OS_ERR err;
	void *p_msg;
	CPU_TS * ts;
	OS_MSG_SIZE msg_size;
	p_arg = p_arg;
	ts=0;
	count=1;
	
	while(1)
	{
	 
		OLED_Init();			//��ʼ��OLED  
		OLED_Clear(); 
		while(1)
	 	{
			sttr=OSTaskQPend(0, OS_OPT_PEND_BLOCKING, &msg_size, ts, &err);
			RxCount=0;
			OLED_Init();
			OLED_Clear();
			OLED_ShowString(0,0,sttr);
//			OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); 
		}
	}
}

void esp8266_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	USART1_Init(115200);
	ESP8266_Init(115200);
	while(1)
	{
	ESP8266_STA_TCPClient_Test();
	OSTimeDlyHMSM(0,0,0,400,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}



