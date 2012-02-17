/* Filename: liftTwo.ch
 * lift two two-connected robots
   For both two-connected robots, 
   the joint4 of the first robot should be connected to the joint1 of the second robot. 
           1st                         2nd
  |---------|--------|        |---------|--------|     
 1|    2    |    3   | 4  X  1|    2    |   3    | 4
  |---------|--------|        |---------|--------|
  
           3rd                         4th
  |---------|--------|        |---------|--------|     
 1|    2     |   3   | 4  X  1|    2    |   3    | 4
  |---------|--------|        |---------|--------|
*/
#include <mobot.h>
CMobot robot1;
CMobot robot2;
CMobot robot3;
CMobot robot4;
CMobotGroup group1, group2;

/* Connect robot variables to the robot modules. */
robot1.connect();
robot2.connect();
robot3.connect();
robot4.connect();

/* Add 1st and 3rd  modules as members of one group with the identical motions */
/* Add 2nd and 4th  modules as members of one group with the identical motions */
group1.addRobot(robot1);
group1.addRobot(robot3);
group2.addRobot(robot2);
group2.addRobot(robot4);

/* Set the robot to "home" position, where all joint angles are 0 degrees. */
group1.moveToZeroNB();
group1.moveToZeroNB();

group1.moveWait();
group2.moveWait();

/* First lift */
group1.moveNB(0, -90,  0, 0);
group2.moveNB(0, 0, 90, 0);
group1.moveWait();
group2.moveWait();
/* Second lift */
group1.moveToNB(0, 0, 90,  0);
group2.moveToNB(0,  -90, 0, 0);
group1.moveWait();
group2.moveWait();
/* Back to zero position */
group1.moveToZeroNB();
group2.moveToZeroNB();
group1.moveWait();
group2.moveWait();