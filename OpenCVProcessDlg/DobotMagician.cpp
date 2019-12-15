#include "StdAfx.h"
#include "DobotMagician.h"
#include "AlarmInfo.h"
#include <conio.h>
#pragma comment(lib, "./DobotDll/x64/DobotDll.lib")
#include "./DobotDll/DobotDll.h"



CDobotMagician::CDobotMagician(void)
{
	m_bIsConnected = false;
	m_bIsSetUserDefinedPose = false;
	m_bIsGotoHome = false;
	memset(m_dWarpMatrix,0,sizeof(double)*6);
	
	for (int i = 0; i<8; i++)
		m_bIsJogMsg[i] = false;
}
CDobotMagician::~CDobotMagician(void)
{
	
}
void CDobotMagician::InitDobot(void)
{
	try
	{
		// Command timeout
		SetCmdTimeout(3000);
		// Clear old commands and set the queued command running
		SetQueuedCmdClear();
		SetQueuedCmdStartExec();

		// Device SN
		char deviceSN[64];
		GetDeviceSN(deviceSN, sizeof(deviceSN));
		_cprintf("Device SN:%s\r\n", deviceSN);

		// Device Name
		char deviceName[64];
		GetDeviceName(deviceName, sizeof(deviceName));
		_cprintf("Device Name:%s\r\n", deviceName);

		// Device version information
		uint8_t majorVersion, minorVersion, revision;
		GetDeviceVersion(&majorVersion, &minorVersion, &revision);
		_cprintf("Device information:V%d.%d.%d\r\n", majorVersion, minorVersion, revision);

		// Set the end effector parameters
		EndEffectorParams endEffectorParams;
		memset(&endEffectorParams, 0, sizeof(EndEffectorParams));
		endEffectorParams.xBias = 71.6f;
		SetEndEffectorParams(&endEffectorParams, false, NULL);

		// 1. Set the JOG parameters
		JOGJointParams jogJointParams;
		for (uint32_t i = 0; i < 4; i++) {
			jogJointParams.velocity[i] = 200;
			jogJointParams.acceleration[i] = 200;
		}
		SetJOGJointParams(&jogJointParams, false, NULL);

		JOGCoordinateParams jogCoordinateParams;
		for (uint32_t i = 0; i < 4; i++) {
			jogCoordinateParams.velocity[i] = 100;
			jogCoordinateParams.acceleration[i] = 100;
		}
		SetJOGCoordinateParams(&jogCoordinateParams, false, NULL);

		JOGCommonParams jogCommonParams;
		jogCommonParams.velocityRatio = 50;
		jogCommonParams.accelerationRatio = 50;
		SetJOGCommonParams(&jogCommonParams, false, NULL);

		// 2. Set the PTP parameters
		PTPJointParams ptpJointParams;
		for (uint32_t i = 0; i < 4; i++) {
			ptpJointParams.velocity[i] = 500;
			ptpJointParams.acceleration[i] = 500;
		}
		SetPTPJointParams(&ptpJointParams, false, NULL);

		PTPCoordinateParams ptpCoordinateParams;

		if (GetPTPCoordinateParams(&ptpCoordinateParams) == DobotCommunicate_NoError)
		{


		}

		ptpCoordinateParams.xyzVelocity = 500;
		ptpCoordinateParams.xyzAcceleration = 500;
		ptpCoordinateParams.rVelocity = 500;
		ptpCoordinateParams.rAcceleration = 500;
		SetPTPCoordinateParams(&ptpCoordinateParams, false, NULL);

		//CPParams cpParams;
		//GetCPParams(&cpParams);
		//cpParams.


		PTPJumpParams ptpJumpParams;
		ptpJumpParams.jumpHeight = 10;
		ptpJumpParams.zLimit = 150;
		SetPTPJumpParams(&ptpJumpParams, false, NULL);


		HOMEParams homeParams;
		GetHOMEParams(&homeParams);
		homeParams.x = 250.0;
		homeParams.y = 0.0;
		homeParams.z = 0.0;
		homeParams.r = 0.0;
		SetHOMEParams(&homeParams, false, NULL);
	}
	catch (...)
	{

	}
}
bool CDobotMagician::Connect()
{
	try
	{
		if (!m_bIsConnected)
		{
			if (ConnectDobot(0, 115200, 0, 0) != DobotConnect_NoError) {
				::AfxMessageBox(L"连接Dobot机械手失败!");
				return false;
			}
			m_bIsConnected = true;
			InitDobot();

			PTPCoordinateParams ptpCoordinateParams;
			GetPTPCoordinateParams(&ptpCoordinateParams);

			PTPJumpParams ptpJumpParams;
			GetPTPJumpParams(&ptpJumpParams);
			ptpJumpParams.jumpHeight = 35;
			SetPTPJumpParams(&ptpJumpParams, false, NULL);


		}
	}
	catch (...)
	{
	
	}
	
	return m_bIsConnected;
}
void CDobotMagician::Disconnect()
{
	try
	{
		if (m_bIsConnected) {
			m_bIsConnected = false;
			DisconnectDobot();

		}
	}
	catch (...)
	{
		
	}
	if (!m_bIsConnected) 
	{
		AfxMessageBox(TEXT("机械手断开！"));
	}
}
bool CDobotMagician::GotoPoint(uint8_t ptpMode, float x, float y, float z, float r, bool waitEnd)
{
	try
	{
		if (!m_bIsConnected)
			return false;
		ClearAllAlarmsState();
		if (IsAlert())
		{
			return false;
		}
		PTPCmd ptpCmd;
		ptpCmd.ptpMode = ptpMode;
		//ptpCmd.ptpMode = PTPMOVJXYZMode;
		ptpCmd.x = x;
		ptpCmd.y = y;
		ptpCmd.z = z;
		ptpCmd.r = r;


		// Send the command. If failed, just resend the command
		uint64_t queuedCmdIndex;
		do {
			int result = SetPTPCmd(&ptpCmd, true, &queuedCmdIndex);
			if (result == DobotCommunicate_NoError) {
				break;
			}
		} while (1);

		// Check whether the command is finished
		do {
			if (waitEnd == false) {
				break;
			}
			uint64_t currentIndex;
			int result = GetQueuedCmdCurrentIndex(&currentIndex);
			if (result == DobotCommunicate_NoError &&
				currentIndex >= queuedCmdIndex) {
				break;
			}
		} while (1);
		
		if (!IsAlert())
		{
			return true;
		}
	}
	catch (...)
	{

	}
	return false;
}
void CDobotMagician::GotoHome(void)
{
	try
	{
		if (!m_bIsConnected)
			return;
		// Send the command. If failed, just resend the command
		uint64_t queuedCmdIndex;
		do {
			HOMECmd homeCmd;
			int result = SetHOMECmd(&homeCmd, true, &queuedCmdIndex);
			if (result == DobotCommunicate_NoError) {
				break;
			}
		} while (1);

		// Check whether the command is finished
		do {
			bool waitEnd = true;
			if (waitEnd == false) {
				break;
			}
			uint64_t currentIndex;
			int result = GetQueuedCmdCurrentIndex(&currentIndex);
			if (result == DobotCommunicate_NoError &&
				currentIndex >= queuedCmdIndex) {
				break;
			}
		} while (1);
		m_bIsSetUserDefinedPose = false;
		if (!m_bIsSetUserDefinedPose)
		{
			//AfxMessageBox(TEXT("用户自定义点已清除，请重新设置！"));
			Pose pose;
			pose.x = 0;
			pose.y = -220;
			pose.z = 0;
			pose.r = 0;
			SetUserDefinedPose(pose);
			CString csText;
			csText.Format(TEXT("自定义点重置为：X:%.1f,Y:%.1f,Z:%.1f,R:%.1f"), pose.x, pose.y, pose.z, pose.r);
			AfxMessageBox(csText);
			m_bIsGotoHome = true;
		}
	}
	catch (...)
	{
	}
	
}
bool CDobotMagician::GetCurrentPose(Pose& pose)
{
	try
	{
		if (!m_bIsConnected)
			return 0;
		//int flag = -1;
		//do {
		SetCmdTimeout(50);
		int flag = GetPose(&pose);
		SetCmdTimeout(5000);
		if (flag != DobotCommunicate_NoError)
		{
			Disconnect();
			//break;
			return 0;
		}
		//} while (0);
		return 1;
	}
	catch (...)
	{

	}
	return 0;
}
void CDobotMagician::PreTranslateMessage(MSG *pMsg,HWND* hWnd)
{
	try
	{
		if (!m_bIsConnected)
			return;
	
		int num = sizeof(hWnd) / sizeof(hWnd[0]);
		num = 8;
		if (pMsg->message == WM_LBUTTONDOWN) {
			for (int i = 0; i < num; i++) {
				if (pMsg->hwnd == hWnd[i]) {					
					TRACE0("LBUTTONDOWN\n");
					
					m_bIsJogMsg[i] = true;
					//jogCmd.isJoint = m_JOGMode.GetCurSel() == 0;
					JOGCmd jogCmd;
					jogCmd.isJoint = 0;
					jogCmd.cmd = i + 1;

					SetJOGCmd(&jogCmd, false, NULL);
					
					break;
				}
			}
		}
		else if (pMsg->message == WM_LBUTTONUP) {
			for (int i = 0; i < num; i++) {
				if (pMsg->hwnd == hWnd[i]) {
					TRACE0("LBUTTONUP\n");
					
					m_bIsJogMsg[i] = false;
					//jogCmd.isJoint = m_JOGMode.GetCurSel() == 0;
					JOGCmd jogCmd;
					jogCmd.isJoint = 0;
					jogCmd.cmd = 0;
					SetJOGCmd(&jogCmd, false, NULL);
					//Sleep(1);
					break;
				}
			}
		}
		else  if (pMsg->message == WM_MOUSEMOVE) 
		{
			TRACE0("MOUSEMOVE\n");
			for (int i = 0; i < num; i++) {
				if (true == m_bIsJogMsg[i]) {
					TRACE0("LBUTTONUP\n");
					m_bIsJogMsg[i] = false;
					JOGCmd jogCmd;
					jogCmd.isJoint = 0;
					jogCmd.cmd = 0;
					SetJOGCmd(&jogCmd, false, NULL);
					//Sleep(1);
					break;
				}
			}
		}
	}
	catch (...)
	{

	}
	
}
void CDobotMagician::SetUserDefinedPose(Pose pose) 
{ 
	m_objUserDefinedPose = pose; 
	m_bIsSetUserDefinedPose = true;
}
bool CDobotMagician::GotoUserDefinedPose(void)
{
	try
	{
		if (!m_bIsSetUserDefinedPose)
			return 0;
		GotoPoint(PTPJUMPXYZMode, m_objUserDefinedPose.x, m_objUserDefinedPose.y, m_objUserDefinedPose.z, m_objUserDefinedPose.r, true);
		return 1;
	}
	catch (...)
	{
	
	}
	return 0;
}


bool CDobotMagician::IsAlert()
{
	try
	{
		uint8_t alarmsState[1024] = { 0 };
		uint32_t len;
		unsigned int maxLen = 1024;
		GetAlarmsState(alarmsState, &len, maxLen);

		UINT index = 0;
		for (int i = 0; i < (int)len; i++)
		{
			for (int j = 7; j >= 0; j--)
			{
				if ((alarmsState[i] >> j) == 1)
				{
					index = i * 8 + j;
					i = len;
					break;
				}
			}
		}
		if (index > 0)
		{
			CAlarmInfo alarmInfo;
			for (int i = 0; i < 128; i++)
			{
				if (index == alarmInfo.index[i])
				{
					AfxMessageBox(alarmInfo.info[i]);
					break;
				}

			}
			return true;
		}
	}
	catch (...)
	{
		

	}
	
	
	return false;
}
