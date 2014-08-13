/*
   Copyright 2013 Barobo, Inc.

   This file is part of libbarobo.

   BaroboLink is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   BaroboLink is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with BaroboLink.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <string.h>
#include "mobot.h"
#include "mobot_internal.h"

#define DEPRECATED(from, to) \
  fprintf(stderr, "Warning: The function \"%s()\" is deprecated. Please use \"%s()\"\n" , from, to)

CMobotGroup::CMobotGroup()
{
  _numRobots = 0;
  _motionInProgress = 0;
  _thread = (THREAD_T*)malloc(sizeof(THREAD_T));
  _numAllocated = 0;
  _robots = NULL;
}

CMobotGroup::~CMobotGroup()
{
}

int CMobotGroup::addRobot(CMobot& robot)
{
  int allocStep = 64;
  /* See if we need to allocate more robots */
  if(_numRobots >= _numAllocated) {
    /* Allocate more */
    CMobot** tmp;
    tmp = (CMobot**)malloc(sizeof(CMobot*) * (_numAllocated + allocStep));
    if(_robots != NULL) {
      memcpy(tmp, _robots, sizeof(CMobot*)*_numRobots);
      free(_robots);
    }
    _robots = tmp;
    _numAllocated += allocStep;
  }
  _robots[_numRobots] = &robot;
  _numRobots++;
  return 0;
}

int CMobotGroup::addRobots(CMobot robots[], int numRobots)
{
  int i;
  for(i = 0; i < numRobots; i++) {
    addRobot(robots[i]);
  }
  return 0;
}

int CMobotGroup::connect()
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->connect();
  }
  return 0;
}

int CMobotGroup::reset()
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->reset();
  }
  return 0;
}

int CMobotGroup::resetToZero()
{
  resetToZeroNB();
  return moveWait();
}

int CMobotGroup::resetToZeroNB()
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->resetToZeroNB();
  }
  return 0;
}

int CMobotGroup::motionArch(double angle) 
{
  argDouble = angle;
  _motionInProgress++;
  motionArchThread(this);
  return 0;
}

int CMobotGroup::motionArchNB(double angle) 
{
  argDouble = angle;
  _motionInProgress++;
  THREAD_CREATE(_thread, motionArchThread, this);
  return 0;
}

void* CMobotGroup::motionArchThread(void* arg) 
{
  CMobotGroup *cmg = (CMobotGroup*)arg;
  cmg->moveJointToNB(ROBOT_JOINT2, -cmg->argDouble/2);
  cmg->moveJointToNB(ROBOT_JOINT3, cmg->argDouble/2);
  cmg->moveJointWait(ROBOT_JOINT2);
  cmg->moveJointWait(ROBOT_JOINT3);
  cmg->_motionInProgress--;
  return NULL;
}

int CMobotGroup::motionDistance(double distance, double radius)
{
  argDouble = distance / radius;
  _motionInProgress++;
  motionDistanceThread(this);
  return 0;
}

int CMobotGroup::motionDistanceNB(double distance, double radius)
{
  argDouble = distance / radius;
  _motionInProgress++;
  THREAD_CREATE(_thread, motionDistanceThread, this);
  return 0;
}

void* CMobotGroup::motionDistanceThread(void* arg)
{
  CMobotGroup *cmg = (CMobotGroup*)arg;
  cmg->move(RAD2DEG(cmg->argDouble), 0, 0, RAD2DEG(cmg->argDouble));
  cmg->_motionInProgress--;
  return NULL;
}

int CMobotGroup::motionInchwormLeft(int num)
{
  argInt = num;
  _motionInProgress++;
  motionInchwormLeftThread(this);
  return 0;
}

int CMobotGroup::motionInchwormLeftNB(int num)
{
  argInt = num;
  _motionInProgress++;
  THREAD_CREATE(_thread, motionInchwormLeftThread, this);
  return 0;
}

void* CMobotGroup::motionInchwormLeftThread(void* arg)
{
  int i;
  CMobotGroup *cmg = (CMobotGroup*)arg;
  cmg->moveJointToNB(ROBOT_JOINT2, 0);
  cmg->moveJointToNB(ROBOT_JOINT3, 0);
  cmg->moveWait();
  for(i = 0; i < cmg->argInt ; i++) {
    cmg->moveJointTo(ROBOT_JOINT2, -50);
    cmg->moveJointTo(ROBOT_JOINT3, 50);
    cmg->moveJointTo(ROBOT_JOINT2, 0);
    cmg->moveJointTo(ROBOT_JOINT3, 0);
  }
  cmg->_motionInProgress--;
  return NULL;
}

int CMobotGroup::motionInchwormRight(int num)
{
  argInt = num;
  _motionInProgress++;
  motionInchwormRightThread(this);
  return 0;
}

int CMobotGroup::motionInchwormRightNB(int num)
{
  argInt = num;
  _motionInProgress++;
  THREAD_CREATE(_thread, motionInchwormRightThread, this);
  return 0;
}

void* CMobotGroup::motionInchwormRightThread(void* arg)
{
  int i;
  CMobotGroup *cmg = (CMobotGroup*)arg;

  cmg->moveJointToNB(ROBOT_JOINT2, 0);
  cmg->moveJointToNB(ROBOT_JOINT3, 0);
  cmg->moveWait();
  for(i = 0; i < cmg->argInt; i++) {
    cmg->moveJointTo(ROBOT_JOINT3, 50);
    cmg->moveJointTo(ROBOT_JOINT2, -50);
    cmg->moveJointTo(ROBOT_JOINT3, 0);
    cmg->moveJointTo(ROBOT_JOINT2, 0);
  }
  cmg->_motionInProgress--;
  return NULL;
}

int CMobotGroup::motionRollBackward(double angle)
{
  argDouble = angle;
  _motionInProgress++;
  motionRollBackwardThread(this);
  return 0;
}

int CMobotGroup::motionRollBackwardNB(double angle)
{
  argDouble = angle;
  _motionInProgress++;
  THREAD_CREATE(_thread, motionRollBackwardThread, this);
  return 0;
}

void* CMobotGroup::motionRollBackwardThread(void* arg)
{
  CMobotGroup *cmg = (CMobotGroup*)arg;
  cmg->move(-cmg->argDouble, 0, 0, -cmg->argDouble);
  cmg->_motionInProgress--;
  return NULL;
}

int CMobotGroup::motionRollForward(double angle)
{
  argDouble = angle;
  _motionInProgress++;
  motionRollForwardThread(this);
  return 0;
}

int CMobotGroup::motionRollForwardNB(double angle)
{
  argDouble = angle;
  _motionInProgress++;
  THREAD_CREATE(_thread, motionRollForwardThread, this);
  return 0;
}

void* CMobotGroup::motionRollForwardThread(void* arg)
{
  CMobotGroup *cmg = (CMobotGroup*)arg;
  cmg->move(cmg->argDouble, 0, 0, cmg->argDouble);
  cmg->_motionInProgress--;
  return NULL;
}

int CMobotGroup::motionSkinny(double angle)
{
  argDouble = angle;
  _motionInProgress++;
  motionSkinnyThread(this);
  return 0;
}

int CMobotGroup::motionSkinnyNB(double angle)
{
  argDouble = angle;
  _motionInProgress++;
  THREAD_CREATE(_thread, motionSkinnyThread, this);
  return 0;
}

void* CMobotGroup::motionSkinnyThread(void* arg)
{
  CMobotGroup *cmg = (CMobotGroup*)arg;
  cmg->moveJointToNB(ROBOT_JOINT2, cmg->argDouble);
  cmg->moveJointToNB(ROBOT_JOINT3, cmg->argDouble);
  cmg->_motionInProgress--;
  return NULL;
}

int CMobotGroup::motionStand()
{
  _motionInProgress++;
  motionStandThread(this);
  return 0;
}

int CMobotGroup::motionStandNB()
{
  _motionInProgress++;
  THREAD_CREATE(_thread, motionStandThread, NULL);
  return 0;
}

void* CMobotGroup::motionStandThread(void* arg)
{
  CMobotGroup* cmg = (CMobotGroup*)arg;
  cmg->resetToZero();
  cmg->moveJointTo(ROBOT_JOINT2, -85);
  cmg->moveJointTo(ROBOT_JOINT3, 70);
  cmg->moveWait();
  cmg->moveJointTo(ROBOT_JOINT1, 45);
  cmg->moveJointTo(ROBOT_JOINT2, 20);
  cmg->_motionInProgress--;
  return 0;
}

int CMobotGroup::motionTurnLeft(double angle)
{
  argDouble = angle;
  _motionInProgress++;
  motionTurnLeftThread(this);
  return 0;
}

int CMobotGroup::motionTurnLeftNB(double angle)
{
  argDouble = angle;
  _motionInProgress++;
  THREAD_CREATE(_thread, motionTurnLeftThread, this);
  return 0;
}

void* CMobotGroup::motionTurnLeftThread(void* arg)
{
  CMobotGroup* cmg = (CMobotGroup*)arg;
  cmg->move(-cmg->argDouble, 0, 0, cmg->argDouble);
  cmg->_motionInProgress--;
  return NULL;
}

int CMobotGroup::motionTurnRight(double angle)
{
  argDouble = angle;
  _motionInProgress++;
  motionTurnRightThread(this);
  return 0;
}

int CMobotGroup::motionTurnRightNB(double angle)
{
  argDouble = angle;
  _motionInProgress++;
  THREAD_CREATE(_thread, motionTurnRightThread, this);
  return 0;
}

void* CMobotGroup::motionTurnRightThread(void* arg)
{
  CMobotGroup* cmg = (CMobotGroup*)arg;
  cmg->move(cmg->argDouble, 0, 0, -cmg->argDouble);
  cmg->_motionInProgress--;
  return NULL;
}

int CMobotGroup::motionTumbleRight(int num)
{
  argInt = num;
  _motionInProgress++;
  motionTumbleRightThread(this);
  return 0;
}

int CMobotGroup::motionTumbleRightNB(int num)
{
  argInt = num;
  _motionInProgress++;
  THREAD_CREATE(_thread, motionTumbleRightThread, this);
  return 0;
}

void* CMobotGroup::motionTumbleRightThread(void* arg)
{
  int i;
  CMobotGroup* cmg = (CMobotGroup*)arg;
  int num = cmg->argInt;

  cmg->resetToZero();
#ifndef _WIN32
  sleep(1);
#else
  Sleep(1000);
#endif

  for(i = 0; i < num; i++) {
    cmg->moveJointTo(ROBOT_JOINT3, DEG2RAD(85));
    cmg->moveJointTo(ROBOT_JOINT2, DEG2RAD(-80));
    cmg->moveJointTo(ROBOT_JOINT3, DEG2RAD(0));
    cmg->moveJointTo(ROBOT_JOINT2, DEG2RAD(0));
    cmg->moveJointTo(ROBOT_JOINT3, DEG2RAD(-80));
    cmg->moveJointTo(ROBOT_JOINT3, DEG2RAD(-45));
    cmg->moveJointTo(ROBOT_JOINT2, DEG2RAD(85));
    cmg->moveJointTo(ROBOT_JOINT3, DEG2RAD(-80));
    cmg->moveJointTo(ROBOT_JOINT2, DEG2RAD(0));
    cmg->moveJointTo(ROBOT_JOINT3, DEG2RAD(0));
    cmg->moveJointTo(ROBOT_JOINT2, DEG2RAD(-80));
    if(i != (num-1)) {
      cmg->moveJointTo(ROBOT_JOINT2, DEG2RAD(-45));
    }
  }
  cmg->moveJointToNB(ROBOT_JOINT3, 0);
  cmg->moveJointToNB(ROBOT_JOINT2, 0);
  cmg->moveWait();

  cmg->_motionInProgress--;
  return NULL;
}

int CMobotGroup::motionTumbleLeft(int num)
{
  argInt = num;
  _motionInProgress++;
  motionTumbleLeftThread(this);
  return 0;
}

int CMobotGroup::motionTumbleLeftNB(int num)
{
  argInt = num;
  _motionInProgress++;
  THREAD_CREATE(_thread, motionTumbleLeftThread, this);
  return 0;
}

void* CMobotGroup::motionTumbleLeftThread(void* arg)
{
  int i;
  CMobotGroup* cmg = (CMobotGroup*)arg;
  int num = cmg->argInt;

  cmg->resetToZero();
#ifndef _WIN32
  sleep(1);
#else
  Sleep(1000);
#endif

  for(i = 0; i < num; i++) {
    cmg->moveJointTo(ROBOT_JOINT2, DEG2RAD(-85));
    cmg->moveJointTo(ROBOT_JOINT3, DEG2RAD(80));
    cmg->moveJointTo(ROBOT_JOINT2, DEG2RAD(0));
    cmg->moveJointTo(ROBOT_JOINT3, DEG2RAD(0));
    cmg->moveJointTo(ROBOT_JOINT2, DEG2RAD(80));
    cmg->moveJointTo(ROBOT_JOINT2, DEG2RAD(45));
    cmg->moveJointTo(ROBOT_JOINT3, DEG2RAD(-85));
    cmg->moveJointTo(ROBOT_JOINT2, DEG2RAD(80));
    cmg->moveJointTo(ROBOT_JOINT3, DEG2RAD(0));
    cmg->moveJointTo(ROBOT_JOINT2, DEG2RAD(0));
    cmg->moveJointTo(ROBOT_JOINT3, DEG2RAD(80));
    if(i != (num-1)) {
      cmg->moveJointTo(ROBOT_JOINT3, DEG2RAD(45));
    }
  }
  cmg->moveJointToNB(ROBOT_JOINT2, 0);
  cmg->moveJointToNB(ROBOT_JOINT3, 0);
  cmg->moveWait();
  cmg->_motionInProgress--;
  return NULL;
}

int CMobotGroup::motionUnstand()
{
  _motionInProgress++;
  motionUnstandThread(this);
  return 0;
}

int CMobotGroup::motionUnstandNB()
{
  _motionInProgress++;
  THREAD_CREATE(_thread, motionUnstandThread, NULL);
  return 0;
}

void* CMobotGroup::motionUnstandThread(void* arg)
{
  CMobotGroup* cmg = (CMobotGroup*)arg;
  cmg->moveToDirect(0, 0, 0, 0);
  cmg->moveJointTo(ROBOT_JOINT3, 45);
  cmg->moveJointTo(ROBOT_JOINT2, -85);
  cmg->moveWait();
  cmg->moveToDirect(0, 0, 0, 0);
  cmg->moveJointTo(ROBOT_JOINT2, 20);
  cmg->_motionInProgress--;
  return 0;
}

int CMobotGroup::motionWait()
{
  while(_motionInProgress > 0) {
#ifdef _WIN32
    Sleep(200);
#else
    usleep(200000);
#endif
  }
  return 0;
}

int CMobotGroup::driveJointToDirect(robotJointId_t id, double angle)
{
  driveJointToDirectNB(id, angle);
  return moveWait();
}

int CMobotGroup::driveJointTo(robotJointId_t id, double angle)
{
  driveJointToDirectNB(id, angle);
  return moveWait();
}

int CMobotGroup::driveJointToDirectNB(robotJointId_t id, double angle)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->driveJointToDirectNB(id, angle);
  }
  return 0;
}

int CMobotGroup::driveJointToNB(robotJointId_t id, double angle)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->driveJointToDirectNB(id, angle);
  }
  return 0;
}

int CMobotGroup::driveToDirect(double angle1, double angle2, double angle3, double angle4)
{
  driveToDirectNB(angle1, angle2, angle3, angle4);
  return moveWait();
}

int CMobotGroup::driveTo(double angle1, double angle2, double angle3, double angle4)
{
  driveToDirectNB(angle1, angle2, angle3, angle4);
  return moveWait();
}

int CMobotGroup::driveToDirectNB(double angle1, double angle2, double angle3, double angle4)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->driveToDirectNB(angle1, angle2, angle3, angle4);
  }
  return 0;
}

int CMobotGroup::driveToNB(double angle1, double angle2, double angle3, double angle4)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->driveToDirectNB(angle1, angle2, angle3, angle4);
  }
  return 0;
}

int CMobotGroup::isMoving()
{
  for(int i = 0; i < _numRobots; i++) {
    if(_robots[i]->isMoving()) {
      return 1;
    }
  }
  return 0;
}

int CMobotGroup::move(double angle1, double angle2, double angle3, double angle4)
{
  moveNB(angle1, angle2, angle3, angle4);
  return moveWait();
}

int CMobotGroup::moveNB(double angle1, double angle2, double angle3, double angle4)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->moveNB(angle1, angle2, angle3, angle4);
  }
  return 0;
} 

int CMobotGroup::moveBackward(double angle)
{
  int rc;
  DEPRECATED("moveBackward", "driveBackward");
  rc = moveBackwardNB(angle);
  if(rc) return rc;
  return moveWait();
}

int CMobotGroup::moveBackwardNB(double angle)
{
    DEPRECATED("moveBackwardNB", "driveBackwardNB");
	for(int i = 0; i < _numRobots; i++) {
    _robots[i]->moveBackwardNB(angle);
  }
  return 0;
}

int CMobotGroup::moveContinuousNB(robotJointState_t dir1, 
                       robotJointState_t dir2, 
                       robotJointState_t dir3, 
                       robotJointState_t dir4)
{
  DEPRECATED("moveContinuousNB", "setMovementStateNB");
  return setMovementStateNB(dir1, dir2, dir3, dir4);
}

int CMobotGroup::moveContinuousTime(robotJointState_t dir1, 
                           robotJointState_t dir2, 
                           robotJointState_t dir3, 
                           robotJointState_t dir4, 
                           double seconds)
{
  DEPRECATED("moveContinuousTime", "setMovementStateTime");
  return setMovementStateTime(dir1, dir2, dir3, dir4, seconds);
}

int CMobotGroup::moveDistance(double distance, double radius)
{
  int rc;
  DEPRECATED("moveDistance", "driveDistance");
  rc = moveDistanceNB(distance, radius);
  if(rc) return rc;
  return moveWait();
}

int CMobotGroup::moveDistanceNB(double distance, double radius)
{
    DEPRECATED("moveDistanceNB", "driveDistanceNB");
	for(int i = 0; i < _numRobots; i++) {
    _robots[i]->moveDistanceNB(distance, radius);
  }
  return 0;
}

int CMobotGroup::moveForward(double angle)
{
  int rc;
  DEPRECATED("moveForward", "driveForward");
  rc = moveForwardNB(angle);
  if(rc) return rc;
  return moveWait();
}

int CMobotGroup::moveForwardNB(double angle)
{
    DEPRECATED("moveForwardNB", "driveForwardNB");
	for(int i = 0; i < _numRobots; i++) {
    _robots[i]->moveForwardNB(angle);
  }
  return 0;
}

int CMobotGroup::moveJointContinuousNB(robotJointId_t id, robotJointState_t dir)
{
  DEPRECATED("moveJointContinuousNB", "setJointMovementStateNB");
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->moveJointContinuousNB(id, dir);
  }
  return 0;
}

int CMobotGroup::moveJointContinuousTime(robotJointId_t id, robotJointState_t dir, double seconds)
{
  DEPRECATED("moveJointContinuousTime", "setJointMovementStateTime");
  return setJointMovementStateTime(id, dir, seconds);
}

int CMobotGroup::moveJointTo(robotJointId_t id, double angle)
{
  moveJointToNB(id, angle);
  return moveWait();
}

int CMobotGroup::moveJointToDirect(robotJointId_t id, double angle)
{
  moveJointToDirectNB(id, angle);
  return moveWait();
}

int CMobotGroup::moveJointToNB(robotJointId_t id, double angle)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->moveJointToNB(id, angle);
  }
  return 0;
}

int CMobotGroup::moveJointToDirectNB(robotJointId_t id, double angle)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->moveJointToDirectNB(id, angle);
  }
  return 0;
}

int CMobotGroup::moveJointWait(robotJointId_t id)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->moveJointWait(id);
  }
  return 0;
}

int CMobotGroup::moveTo(double angle1, double angle2, double angle3, double angle4)
{
  moveToNB(angle1, angle2, angle3, angle4);
  return moveWait();
}

int CMobotGroup::moveToDirect(double angle1, double angle2, double angle3, double angle4)
{
  moveToDirectNB(angle1, angle2, angle3, angle4);
  return moveWait();
}

int CMobotGroup::moveToNB(double angle1, double angle2, double angle3, double angle4)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->moveToNB(angle1, angle2, angle3, angle4);
  }
  return 0;
}

int CMobotGroup::moveToDirectNB(double angle1, double angle2, double angle3, double angle4)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->moveToDirectNB(angle1, angle2, angle3, angle4);
  }
  return 0;
}

int CMobotGroup::moveWait()
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->moveWait();
  }
  return 0;
}

int CMobotGroup::moveToZeroNB()
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->moveToZeroNB();
  }
  return 0;
}

int CMobotGroup::moveToZero()
{
  moveToZeroNB();
  return moveWait();
}

int CMobotGroup::stopAllJoints()
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->stopAllJoints();
  }
  return 0;
}

int CMobotGroup::stopOneJoint(robotJointId_t id)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->stopOneJoint(id);
  }
  return 0;
}

int CMobotGroup::stopTwoJoints(robotJointId_t id1, robotJointId_t id2)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->stopTwoJoints(id1, id2);
  }
  return 0;
}

int CMobotGroup::stopThreeJoints(robotJointId_t id1, robotJointId_t id2, robotJointId_t id3)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->stopThreeJoints(id1, id2, id3);
  }
  return 0;
}

int CMobotGroup::setExitState(robotJointState_t exitState)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->setExitState(exitState);
  }
  return 0;
}

int CMobotGroup::setJointMovementStateNB(robotJointId_t id, robotJointState_t dir)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->setJointMovementStateNB(id, dir);
  }
  return 0;
}

int CMobotGroup::setJointMovementStateTime(robotJointId_t id, robotJointState_t dir, double seconds)
{
  int msecs = seconds * 1000.0;
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->setJointMovementStateNB(id, dir);
  }
#ifdef _WIN32
  Sleep(msecs);
#else
  usleep(msecs * 1000);
#endif
  return 0;
}

int CMobotGroup::setJointMovementStateTimeNB(robotJointId_t id, robotJointState_t dir, double seconds)
{
  int msecs = seconds * 1000.0;
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->setJointMovementStateNB(id, dir);
  }
  return 0;
}

int CMobotGroup::setJointSafetyAngle(double angle)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->setJointSafetyAngle(angle);
  }
  return 0;
}

int CMobotGroup::setJointSafetyAngleTimeout(double seconds)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->setJointSafetyAngleTimeout(seconds);
  }
  return 0;
}

int CMobotGroup::setJointSpeed(robotJointId_t id, double speed)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->setJointSpeed(id, speed);
  }
  return 0;
}

int CMobotGroup::setJointSpeeds(double speed1, double speed2, double speed3, double speed4)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->setJointSpeeds(speed1, speed2, speed3, speed4);
  }
  return 0;
}

int CMobotGroup::setJointSpeedRatio(robotJointId_t id, double ratio)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->setJointSpeedRatio(id, ratio);
  }
  return 0;
}

int CMobotGroup::setJointSpeedRatios(double ratio1, double ratio2, double ratio3, double ratio4)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->setJointSpeedRatios(ratio1, ratio2, ratio3, ratio4);
  }
  return 0;
}

int CMobotGroup::setMovementStateNB(robotJointState_t dir1, 
                       robotJointState_t dir2, 
                       robotJointState_t dir3, 
                       robotJointState_t dir4)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->setMovementStateNB(dir1, dir2, dir3, dir4);
  }
  return 0;
}

int CMobotGroup::setMovementStateTime(robotJointState_t dir1, 
                           robotJointState_t dir2, 
                           robotJointState_t dir3, 
                           robotJointState_t dir4, 
                           double seconds)
{
  int msecs = seconds * 1000.0;
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->setMovementStateNB(dir1, dir2, dir3, dir4);
  }
#ifdef _WIN32
  Sleep(msecs);
#else
  usleep(msecs*1000);
#endif
  for(int i = 0; i < _numRobots; i++) {
    //_robots[i]->stop();
    _robots[i]->setMovementStateNB(ROBOT_HOLD, ROBOT_HOLD, ROBOT_HOLD, ROBOT_HOLD);
  }
  return 0;
}

int CMobotGroup::setMovementStateTimeNB(robotJointState_t dir1, 
                           robotJointState_t dir2, 
                           robotJointState_t dir3, 
                           robotJointState_t dir4, 
                           double seconds)
{
  int msecs = seconds * 1000.0;
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->setMovementStateNB(dir1, dir2, dir3, dir4);
  }
  return 0;
}

int CMobotGroup::setTwoWheelRobotSpeed(double speed, double radius)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->setTwoWheelRobotSpeed(speed, radius);
  }
  return 0;
}

int CMobotGroup::turnLeft(double angle, double radius, double tracklength)
{
  int rc;
  rc = turnLeftNB(angle, radius, tracklength);
  if(rc) return rc;
  return moveWait();
}

int CMobotGroup::turnLeftNB(double angle, double radius, double tracklength)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->turnLeftNB(angle, radius, tracklength);
  }
  return 0;
}

int CMobotGroup::turnRight(double angle, double radius, double tracklength)
{
  int rc;
  rc = turnRightNB(angle, radius, tracklength);
  if(rc) return rc;
  return moveWait();
}

int CMobotGroup::turnRightNB(double angle, double radius, double tracklength)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->turnRightNB(angle, radius, tracklength);
  }
  return 0;
}
int CMobotGroup::driveDistance(double distance, double radius)
{
  int rc;
  rc = driveDistanceNB(distance, radius);
  if(rc) return rc;
  return moveWait();
}

int CMobotGroup::driveDistanceNB(double distance, double radius)
{
	for(int i = 0; i < _numRobots; i++) {
    _robots[i]->driveDistanceNB(distance, radius);
  }
  return 0;
}
int CMobotGroup::driveForward(double angle)
{
  int rc;
  rc = driveForwardNB(angle);
  if(rc) return rc;
  return moveWait();
}

int CMobotGroup::driveForwardNB(double angle)
{
	for(int i = 0; i < _numRobots; i++) {
    _robots[i]->driveForwardNB(angle);
  }
  return 0;
}
int CMobotGroup::driveBackward(double angle)
{
  int rc;
  rc = driveBackwardNB(angle);
  if(rc) return rc;
  return moveWait();
}

int CMobotGroup::driveBackwardNB(double angle)
{
	for(int i = 0; i < _numRobots; i++) {
    _robots[i]->driveBackwardNB(angle);
  }
  return 0;
}

int CMobotGroup::driveAccelTimeNB(double radius, double acceleration, double time)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->driveAccelTimeNB(radius, acceleration, time);
  }
  return 0;
}

int CMobotGroup::driveAccelToVelocityNB(double radius, double acceleration, double velocity)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->driveAccelToVelocityNB(radius, acceleration, velocity);
  }
  return 0;
}

int CMobotGroup::driveAccelToMaxSpeedNB(double radius, double acceleration)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->driveAccelToMaxSpeedNB(radius, acceleration);
  }
  return 0;
}

int CMobotGroup::driveAccelDistanceNB(double radius, double acceleration, double distance)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->driveAccelDistanceNB(radius, acceleration, distance);
  }
  return 0;
}

int CMobotGroup::driveAccelSmoothNB(double radius, double accel0, double accelf, double vmax, double distance)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->driveAccelSmoothNB(radius, accel0, accelf, vmax, distance);
  }
  return 0;
}

int CMobotGroup::driveAccelCycloidalNB(double radius, double distance, double time)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->driveAccelCycloidalNB(radius, distance, time);
  }
  return 0;
}

int CMobotGroup::driveAccelHarmonicNB(double radius, double distance, double time)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->driveAccelHarmonicNB(radius, distance, time);
  }
  return 0;
}

int CMobotGroup::accelJointTimeNB(robotJointId_t id, double acceleration, double time)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->accelJointTimeNB(id, acceleration, time);
  }
  return 0;
}

int CMobotGroup::accelJointToVelocityNB(robotJointId_t id, double acceleration, double velocity)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->accelJointTimeNB(id, acceleration, velocity);
  }
  return 0;
}

int CMobotGroup::accelJointToMaxSpeedNB(robotJointId_t id, double acceleration)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->accelJointToMaxSpeedNB(id, acceleration);
  }
  return 0;
}

int CMobotGroup::accelJointAngleNB(robotJointId_t id, double acceleration, double angle)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->accelJointAngleNB(id, acceleration, angle);
  }
  return 0;
}

int CMobotGroup::accelJointSmoothNB(robotJointId_t id, double accel0, double accelf, double vmax, double angle)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->accelJointSmoothNB(id, accel0, accelf, vmax, angle);
  }
  return 0;
}

int CMobotGroup::accelJointCycloidalNB(robotJointId_t id, double angle, double time)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->accelJointCycloidalNB(id, angle, time);
  }
  return 0;
}

int CMobotGroup::accelJointHarmonicNB(robotJointId_t id, double angle, double time)
{
  for(int i = 0; i < _numRobots; i++) {
    _robots[i]->accelJointHarmonicNB(id, angle, time);
  }
  return 0;
}

int CMobotGroup::openGripperNB(double angle)
{
	return CMobotGroup::moveToNB(-angle/2.0, 0, -angle/2.0, 0);
}

int CMobotGroup::openGripper(double angle)
{
  return CMobotGroup::moveTo(-angle/2.0, 0, -angle/2.0, 0);
}

int CMobotGroup::closeGripper()
{
	for(int i = 0; i < _numRobots; i++) {
        _robots[i]->closeGripperNB();
    }
    return moveWait();
}

int CMobotGroup::closeGripperNB()
{
    for(int i = 0; i < _numRobots; i++) {
        _robots[i]->closeGripperNB();
    }
    return 0;
}

int CMobotGroup::groupPlayMelody(const char *filename)
{
	mobotMelodyNote_t** _head;
	mobotMelodyNote_t *iter, *next;
	int tempo;
    
	/*Array that keeps track of the heads of the lists for each robot*/
	//_head=(mobotMelodyNote_t**)malloc(sizeof(mobotMelodyNote_t)*_numRobots);

	/*Read the melody file. Give a melody to each robot*/
	groupReadMelody(filename, _head, &tempo);
	for (int i = 0; i < _numRobots; i++)
	{
		if( _head[i] != NULL)
		{
			/*Load the melody in each robot and make it play*/
			_robots[i]->melodyLoadPacketNB(_head[i], tempo);
		}
		else /*end of the list for that robot*/
		{
			_robots[i]->stopMelody();
		}
		/*Robot 0 is the master robot. Gives the synchronization signal*/
		if(i == 0)
		{
			_robots[i]->melodySyncPacketsNB(_numRobots);
		}
	}

	/*Free the list*/
	for ( int i = 0; i < _numRobots; i++)
	{
		for(iter = _head[i]; iter != NULL; iter = next)
        {
            next = iter->next;
            free(iter);
        }
	}
	return 0;
}

/*Now it read for all the same file. Then I need to read different parts of the same file*/
int CMobotGroup::groupReadMelody(const char *filename, mobotMelodyNote_t** head, int* tempo)
{
	head=(mobotMelodyNote_t**)malloc(sizeof(mobotMelodyNote_t)*_numRobots);
	for ( int i = 0; i < _numRobots; i++)
	{
		_robots[i]->readMelody(filename, head[i], tempo);
	}
	return 0;
}



