#pragma once
#include "./DobotDll/DobotDll.h"
class CDobotMagician
{
public:
	CDobotMagician(void);
	~CDobotMagician(void);
	
protected:
	bool m_bIsConnected;
	void InitDobot();
	Pose m_objUserDefinedPose;
	bool m_bIsSetUserDefinedPose;
	bool m_bIsGotoHome;

	bool m_bIsJogMsg[8];
public:
	bool Connect();
	void Disconnect();

	
	// Wrap of Dobot API
	bool GotoPoint(uint8_t ptpMode, float x, float y, float z, float r, bool waitEnd = false);
	void GotoHome(void);
	bool GotoUserDefinedPose(void);
	bool GetCurrentPose(Pose& pose);
	void PreTranslateMessage(MSG *pMsg,HWND* hWnd);

	bool GetConnectStatus() { return m_bIsConnected; }
	bool GetHomeStatus() { return m_bIsGotoHome; }
	bool GetUserStatus() { return m_bIsSetUserDefinedPose; }
	double m_dWarpMatrix[6];
	void SetUserDefinedPose(Pose pose);
	bool IsAlert();
};

