// iMobotController_WindowsDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "gait.h"
#include <mobot.h>
#include "afxcmn.h"
#include <math.h>

#define IDT_TIMER1 100

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef enum button_e
{
  B_PLAY, // Play Gait
  B_M1F, // Motor forward
  B_M2F,
  B_M3F,
  B_M4F,
  B_M1S, // Motor stop
  B_M2S,
  B_M3S,
  B_M4S,
  B_M1B, // Motor Backward
  B_M2B,
  B_M3B,
  B_M4B,
  B_FORWARD, // Roll commands
  B_STOP,
  B_LEFT,
  B_RIGHT,
  B_BACK,
  S_M1S, // Sliders, motor 1 speed
  S_M2S,
  S_M3S,
  S_M4S,
  S_M1P, // Motor position sliders
  S_M2P, 
  S_M3P, 
  S_M4P, 
  B_SETPOS,
  B_MOVE,
  B_SETSPD,
  B_MOVETOZERO,
  B_NUMBUTTONS
} buttonId_t;

// CiMobotController_WindowsDlg dialog
class CiMobotController_WindowsDlg : public CDialog
{
  DECLARE_DYNAMIC(CiMobotController_WindowsDlg)

public:
// Construction
	CiMobotController_WindowsDlg(CWnd* pParent = NULL);	// motionStandard constructor

// Dialog Data
	enum { IDD = IDD_IMOBOTCONTROLLER_WINDOWS_DIALOG };

  CMobot iMobotComms;
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	bool isConnected;
  Gait* m_gaits[50];
  int m_numGaits;
public:
  int m_positions[4]; // Store last known slider position
  int m_speeds[4];    // Store last known slider position
	void InitIcons();
  void InitGaits();
  void InitSliders();
  void UpdateSliders();
  void UpdateSpeedSliders(int i, double speed);
  int addGait(Gait* gait);
  int poseJoints(const double *angles, unsigned char motorMask);
  int moveJoints(const double *angles, unsigned char motorMask);
  
	afx_msg void OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	CButton m_button_Motor1Forward;
	CButton m_button_Motor2Forward;
	CButton m_button_Motor3Forward;
	CButton m_button_Motor4Forward;
	CButton m_button_Motor1Stop;
	CButton m_button_Motor2Stop;
	CButton m_button_Motor3Stop;
	CButton m_button_Motor4Stop;
	CButton m_button_Motor1Backward;
	CButton m_button_Motor2Backward;
	CButton m_button_Motor3Backward;
	CButton m_button_Motor4Backward;
	CListBox m_list_gaits;
	CEdit m_edit_Motor1Position;
	CEdit m_edit_Motor2Position;
	CEdit m_edit_Motor3Position;
	CEdit m_edit_Motor4Position;
public:
	CEdit *m_edit_MotorPositions[4];
	CEdit *m_edit_MotorSpeeds[4];
	CEdit m_edit_Address;
	CEdit m_edit_Channel;
	CSliderCtrl m_slider_Speed1;
	CSliderCtrl m_slider_Speed2;
	CSliderCtrl m_slider_Speed3;
	CSliderCtrl m_slider_Speed4;
	CSliderCtrl m_slider_Position1;
	CSliderCtrl m_slider_Position2;
	CSliderCtrl m_slider_Position3;
	CSliderCtrl m_slider_Position4;
	CSliderCtrl *m_slider_Speeds[4];
	CSliderCtrl *m_slider_Positions[4];
	afx_msg void OnBnClickedButtonplay();
  void handlerPlay(void);
	afx_msg void OnBnClickedButtonconnect();
	afx_msg void OnLbnSelchangeListgaits();
	afx_msg void OnEnChangeEditposition4();
	// Picture of the iMobot joints
	CStatic m_staticicon_iMobotPicture;
	afx_msg void OnStnClickedStaticImobotPicture();
	CButton m_button_rollForward;
	CButton m_button_rollStop;
	CButton m_button_rollBack;
	CButton m_button_rollLeft;
	CButton m_button_rollRight;
	afx_msg void OnBnClickedButtonMotor1forward();
	afx_msg void OnBnClickedButtonMotor2forward();
	afx_msg void OnBnClickedButtonMotor3forward();
	afx_msg void OnBnClickedButtonMotor4forward();
  void handlerM1F();
  void handlerM2F();
  void handlerM3F();
  void handlerM4F();
	afx_msg void OnBnClickedButtonMotor1stop();
	afx_msg void OnBnClickedButtonMotor2stop();
	afx_msg void OnBnClickedButtonMotor3stop();
	afx_msg void OnBnClickedButtonMotor4stop();
  void handlerM1S();
  void handlerM2S();
  void handlerM3S();
  void handlerM4S();
	afx_msg void OnBnClickedButtonMotor1backward();
	afx_msg void OnBnClickedButtonMotor2backward();
	afx_msg void OnBnClickedButtonMotor3backward();
	afx_msg void OnBnClickedButtonMotor4backward();
  void handlerM1B();
  void handlerM2B();
  void handlerM3B();
  void handlerM4B();
	afx_msg void OnBnClickedButtonrollforward();
	afx_msg void OnBnClickedButtonrollstop();
	afx_msg void OnBnClickedButtonrollleft();
	afx_msg void OnBnClickedButtonrollright();
	afx_msg void OnBnClickedButtonrollback();
  void handlerFORWARD();
  void handlerSTOP();
  void handlerLEFT();
  void handlerRIGHT();
  void handlerBACK();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void OnOK();


// Implementation
//protected:

public:
	afx_msg void OnNMCustomdrawSliderposition2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTRBNThumbPosChangingSliderposition2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTRBNThumbPosChangingSliderposition1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTRBNThumbPosChangingSliderposition3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTRBNThumbPosChangingSliderposition4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRobotConfiguremobotbluetooth();
	CEdit m_edit_MotorSpeed1;
	CEdit m_edit_MotorSpeed2;
	CEdit m_edit_MotorSpeed3;
	CEdit m_edit_MotorSpeed4;
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEditsetpos1();
	CEdit m_edit_setpos1;
	CEdit m_edit_setpos2;
	CEdit m_edit_setpos3;
	CEdit m_edit_setpos4;
	afx_msg void OnBnClickedButtonGopos();
	void handlerSETPOS();
	afx_msg void OnRobotConnecttoamobot();
	afx_msg void OnConnectDisconnectfrommobot();
	afx_msg void OnHelpHelp();
	afx_msg void OnHelpAboutmobotcontroller();
	afx_msg void OnFileExit();
	afx_msg void OnBnClickedButtonMovetozero();
  void handlerMOVETOZERO();
  afx_msg void OnBnClickedButtonSetspd();
  void handlerSETSPD();
  CEdit m_edit_setspd1;
  CEdit m_edit_setspd2;
  CEdit m_edit_setspd3;
  CEdit m_edit_setspd4;
  afx_msg void OnBnClickedButtonMove();
  void handlerMOVE();
  afx_msg void OnHelpDemos();
  afx_msg void OnTeachingmodeTeachingmodedialog();
};

DWORD WINAPI HandlerThread(void*);

typedef struct buttonState_s
{
  int clicked;
  void (CiMobotController_WindowsDlg::*handlerFunc)(void);
} buttonState_t;

extern buttonState_t g_buttonState[B_NUMBUTTONS];

