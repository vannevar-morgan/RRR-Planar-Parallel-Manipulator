(R)RR Planar Parallel Manipulator
=============================================

This was originally a student project for a robotics class.  See:

https://www.youtube.com/watch?v=0pd5GFRM_B0

The goal of this project was to understand the inverse kinematics for planar parallel manipulators. After figuring out the inverse kinematics, it was implemented on an actual (R)RR arm using BMS-620 MG servos as the joint actuators. It uses an Arduino Uno to control the servos and MATLAB to perform inverse kinematics. Inverse kinematics is performed using the sine-cosine method as presented by Williams and Shelley in "Inverse Kinematics for Planar Parallel Manipulators". This paper is available at:

http://www.ohio.edu/people/williar4/html../PDF/M97Paral.pdf

MATLAB interfaces with the Arduino over the serial port COM4 and writes the joint angles determined from inverse kinematics.


Additional relevant papers are:

www.academia.edu/2730197/A_CONTROL_APPROACH_OF_A_3-RRR_PLANAR_PARALLEL_MINIROBOT


A very useful simulator for a 3-DOF (R)RR Planar Parallel Manipulator is available here:

http://www.parallemic.org/Java/3RRR.html



Repository Contents
-------------------
* **/R3PPM_matlab** - matlab project files (kinematics)
* **/R3PPM_arduino** - arduino sketch


License Information
-------------------

All code is released under [GNU GPLv3.0](http://www.gnu.org/copyleft/gpl.html).
Please feel free to do anything you want with it.
