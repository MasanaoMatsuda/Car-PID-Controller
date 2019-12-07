# Car-PID-Controller
This is a PID controller project. There is PID instruction below.
![result](https://github.com/MasanaoMatsuda/Car-PID-Controller/blob/media/gif/pid_running_straight.gif)

## The Effect each of the P,I,D components
#### P
 - P is the initial of a word **Proportion**.
 - This works to penalize for cross track error directly.
 - In this case it tries to keep our vehicle runs on center of a lane.
#### I
 - I is the initial of a word **Integral**.
 - This works to penalize for accumulated cross track error.
 - It effects reduce bias.
 - In this counter clockwise course our vehicle biased to run right sides.
#### D
 - D is the initial of a word **Differenciation**.
 - This works to penalize for leaving.
 - In this case when our vehicle is leaving from center line, it tries to turn more steering but when the vehicle is returning to center, it tries to turn steering softly.

## How the final hyperparameters were chosen
I tried manual tuning.  
Firstly I tried to tune only P-controller to drive align a center of road. Secondly I tried to tune D-controller with tuned P-controller to drive well curve. Only P-controller is not enough to drive curving road. Then I added I-controller. But I felt it was hard because the integral of CTE increase easily so I multiply 0.001 to raw CTE value.

Only PID controller it is hard to handle steep curve. It is needed to control vehicle speed.
![result](https://github.com/MasanaoMatsuda/Car-PID-Controller/blob/media/gif/pid_running_curve.gif)
