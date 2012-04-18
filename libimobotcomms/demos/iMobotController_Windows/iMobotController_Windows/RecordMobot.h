#pragma once
#include <mobot.h>

enum motionType_e
{
	MOTION_POS,
	MOTION_SLEEP,
};

struct motion_s
{
	enum motionType_e motionType;
	union data_u {
		double sleepDuration;
		double pos[4];
	} data;
	TCHAR* name;
};

class CRecordMobot :
	public CMobot
{
public:
	CRecordMobot(TCHAR* name);
	~CRecordMobot(void);
	int record();
	int addDelay(double seconds);
	int play(int index);
	int getMotionType(int index);
	int getMotionString(int index, TCHAR* buf);
	const TCHAR* getMotionName(int index);
	int setMotionName(int index, const TCHAR* name);
	int removeMotion(int index);
	int numMotions();
private:
	int _numMotions;
	struct motion_s **_motions;
	int _numMotionsAllocated;
	TCHAR _name[80];
};