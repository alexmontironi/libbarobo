/* Discription:
 * In this scene, six modules are assembled as two three-module tanks with grippers.
 *          1st 
 * |---------|--------|
 *1|    2    |   3   ^| 4
 * |---------|--------|
 *       |-------|
 *       |   1   | 3rd
 *       |-------|
 * |---------|--------|
 *1|    2    |   3   ^| 4
 * |---------|--------|            
 *          2nd                        
 *1. The symbol ^ indicates the switche on robots.
 *
 *2. The joint 1 of the third robot is the connecting point and the switch of the third robot
 *is near the second robot.
 *
 *3. Please make sure that the gripper is open when it is assembled.
 */
 
#include <mobot.h>
CMobot robot1;
CMobot robot2;
CMobot robot3;
CMobot robot4;
CMobot robot5;
CMobot robot6;
CMobotGroup group1;
CMobotGroup group2;

/* Connect robot variables to the robot modules. */
robot1.connect();
robot2.connect();
robot3.connect();
robot4.connect();
robot5.connect();
robot6.connect();

robot1.setJointSpeedRatios(0.8, 0.4, 0.4, 0.8);
robot2.setJointSpeedRatios(0.8, 0.4, 0.4, 0.8);
robot3.setJointSpeedRatios(0.6, 0.6, 0.6, 0.8);
robot4.setJointSpeedRatios(0.8, 0.4, 0.4, 0.8);
robot5.setJointSpeedRatios(0.8, 0.4, 0.4, 0.8);
robot6.setJointSpeedRatios(0.6, 0.6, 0.6, 0.8);

/* Add the two modules to be members of our group */
group1.addRobot(robot1);
group1.addRobot(robot2);
group1.addRobot(robot4);
group1.addRobot(robot5);
group2.addRobot(robot3);
group2.addRobot(robot6);

/* Set the robot to "home" position, where all joint angles are 0 degrees. */
group1.moveToZeroNB();
group2.moveToZeroNB();
group1.moveWait();
group2.moveWait();

// "gun" ready
group2.moveTo(0, 90, 0, 0);
group2.moveJointTo(ROBOT_JOINT4, 4*360);
group2.moveTo(0, 90, 60, 0);

// move forward
group1.motionRollForward(360);

// turn right
group1.motionTurnRight(270);

// move backward
group1.motionRollBackward(180);

// turn left
group1.motionTurnLeft(360);