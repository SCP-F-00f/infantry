/**
 * @Copyright(C),
 * @FileName:.c
 * @Author: HongYuJia 
 * @Teammate��
 * @Version: V3.0
 * @Date:2021.4.13
 * @Description:   ����AC��֮�����Ϣ����
 * @Note:       
 * @Others: 
**/
#include "connect_task.h"
#include "delay.h"
#include "FreeRTOS.h"
#include "task.h"
#include "can.h"
#include "can1_app.h"
#include "can2_app.h"
#include "main.h"
#include "chassis_task.h"


extern CAN_TxHeaderTypeDef can2_tx_header;
extern uint8_t can2_tx_data[CAN_TX_BUF_SIZE];
extern robot_status_t robot_status;
connect_t connect_data;


/**
  * @brief          can2����rc���ݱ���
  * @author         
  * @param[in] 
  * @retval	
  * @note        
  */
void connect_rc_ctrl_process(connect_t *connect_data, uint8_t aData[])
{
	connect_data->can2_rc_ctrl.control_mode = (aData[0]);
	connect_data->can2_rc_ctrl.work_mode = (aData[1]);
	connect_data->can2_rc_ctrl.rc.ch2 = (aData[2]<<8)|aData[3];
	connect_data->can2_rc_ctrl.rc.ch3 = (aData[4]<<8)|aData[5];
	connect_data->can2_rc_ctrl.mouse.key = (aData[6]<<8)|aData[7];
	
	connect_data->can2_rc_ctrl.rc.ch2 -= RC_CHANNEL_VALUE_MIDDLE;
	connect_data->can2_rc_ctrl.rc.ch3 -= RC_CHANNEL_VALUE_MIDDLE;
	connect_data->can2_rc_ctrl.rc.ch2 *= -1;
	connect_data->can2_rc_ctrl.rc.ch3 *= -1;
	connect_data->receive_rc_data_flag = 1;//��ʾ�Ѿ����յ���can2��rc����
}

typedef union float_char_
{
	float f;
	uint8_t c[4];
}float_char;
void connect_gyro_data_process(connect_t *connect_data, uint8_t aData[])
{
	float_char f_c;
	f_c.c[0] = aData[0];
	f_c.c[1] = aData[1];
	f_c.c[2] = aData[2];
	f_c.c[3] = aData[3];
	connect_data->can2_rc_ctrl.gyro.yaw_set = f_c.f/10;
	f_c.c[0] = aData[4];
	f_c.c[1] = aData[5];
	f_c.c[2] = aData[6];
	f_c.c[3] = aData[7];
	connect_data->can2_rc_ctrl.gyro.yaw_fdb = f_c.f/10;
	connect_data->receive_rc_data_flag = 1;//��ʾ�Ѿ����յ���can2��rc����

}





/**
  * @brief          ���ӳ�ʼ��
  * @author         
  * @param[in] 
  * @retval	
  * @note           ����δ���� ������������� ������ٽ��� while(1)
  */
void connect_init(connect_t *connect_data) 
{
	connect_data->cm1_msg = get_cm1_msg_point();
	connect_data->cm2_msg = get_cm2_msg_point();
	connect_data->cm3_msg = get_cm3_msg_point();
	connect_data->cm4_msg = get_cm4_msg_point();
	
//	check_connect(connect_data);
}

/**
  * @brief          ��������
  * @author         
  * @param[in] 
  * @retval	
  * @note           
  */
 
void connect_task(void *argument)
{
	connect_init(&connect_data);
	while(1)
	{
		send_shoot_17mm_data(&robot_status);
		send_shoot_judge_data();
		vTaskDelay(10);
	}
}
/**
  * @brief          
  * @author         
  * @param[in] 
  * @retval	
  * @note           
  */
connect_t *get_connect_data_point(void)
{
	return &connect_data;
}