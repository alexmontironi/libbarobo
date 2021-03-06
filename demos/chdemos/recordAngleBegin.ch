/* File: recordanglebegin.ch
 * Record a joint angle and time, plot the acquired data */
#include <mobot.h>
#include <chplot.h>

CMobot robot;
double timeInterval = 0.1; // time interval in 0.1 second 
int numDataPoints;         // number of data points recorded
robotRecordData_t time, angle1;     // location for recorded time and angles for joint 1
CPlot plot;                        // plotting class

/* connect to the paired Mobot and move to the zero position */
robot.connect();
robot.resetToZero();

/* set the joints 1 and 4 speed to 45 degrees/second */
robot.setJointSpeed(ROBOT_JOINT1, 45);
robot.setJointSpeed(ROBOT_JOINT4, 45);

/* begin recording data */
robot.recordAngleBegin(ROBOT_JOINT1, time, angle1, timeInterval);

/* move the joints 1 and 4 720 degrees */
robot.move(720, 0, 0, 720);

/* end  recording data  */
robot.recordAngleEnd(ROBOT_JOINT1, numDataPoints);
printf("Captured %d data points.\n", numDataPoints);

/* plot the data */
plot.title("Angles for joint 1 versus time");
plot.label(PLOT_AXIS_X, "time (seconds)");
plot.label(PLOT_AXIS_Y, "angle for joint1 (degrees)");
plot.data2DCurve(time, angle1, numDataPoints);
plot.grid(PLOT_ON);
plot.plotting();
