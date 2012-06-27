/* Dicription:
 * In this scene, two two-connected robots do lift.
 *           1st                         2nd
 *  |---------|--------|        |---------|--------|     
 * 1|    2    |    3   | 4  X  1|    2    |   3    | 4
 *  |---------|--------|        |---------|--------|
 *  
 *           3rd                         4th
 *  |---------|--------|        |---------|--------|     
 * 1|    2     |   3   | 4  X  1|    2    |   3    | 4
 *  |---------|--------|        |---------|--------|
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
group1.addMobot(robot1);
group1.addMobot(robot3);
group2.addMobot(robot2);
group2.addMobot(robot4);

/* Set the robot to "home" position, where all joint angles are 0 degrees. */
group1.moveToZeroNB();
group1.moveToZeroNB();

group1.moveWait();
group2.moveWait();

/* First lift */
group1.moveToNB(0, -90,  0, 0);
group2.moveToNB(0, 0, 90, 0);
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
