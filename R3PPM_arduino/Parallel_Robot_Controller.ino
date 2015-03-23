//  (R)RR Planar Parallel Robot Controller
//  12/17/2013


#include <string>
#include <Servo.h>

#define num_servos 3       //  Define the number of servos as 3

Servo servo1;
Servo servo2;
Servo servo3;
Servo servos[num_servos] = {servo1, servo2, servo3};  //  Create an array of servo objects equal to the number of servos
const byte servo_pins[num_servos] = {12,11,10};    //  Define the pins to attach the servos to

int servo_write_count = 0;
int angle_count = 0;
String angles_strings[num_servos] = {"", "", ""};
int angles_ints[num_servos] = {90, 90, 90};

void setup()
{
  Serial.begin(57600);              //  Begin serial communication at 57600 baud
  Serial.flush();                   //  Flush the serial line
  
  for (int i = 0; i < num_servos; i++)    //  Make a loop to attach all servos to their defined pins.
  {
    servos[i].attach(servo_pins[i]);      //  Attach each servo to its pin.
  }
  
  //  Each servo only has 132 degrees range of ACTUATED motion.  25 deg - 157 deg.
  //  NOTE: Servo angles are defined backwards in order to represent the kinematics as defined.  i.e., 157 = servo angle zero, 25 = servo angle 132
  
  //servo1.write(90);//126 for simulator example, 157 to zero the servo, Servo 157 @ 240 on actual task coordinate frame (Good to go!!)
  //servo2.write(90);//91 for simulator example, 157 to zero the servo, Servo 157 @ 0 on the actual task coordinate frame (Good to go!!)
  //servo3.write(90);//151 for the simulator example, 157 to zero the servo, Servo 157 @ 120 on actual task coordinate frame (Good to go!!)
  
  //for (int i = 0; i < num_servos; i++)    //  Make a loop to initialize all joints to their zero configuration.
  //{
  //  servos[i].write(90);                  //  Write each servo to 90 representing the zero configuration for each joint.
  //}
  
}

void loop()
{
  char character;  // Declare a variable, character, of type char, representing the next character over the serial port.
  String content = "";  // Declare a variable of type String and initialize to "".  Each read of a character from the serial port will be appended to this string.

  while(Serial.available()) {          //  If there is data available over the serial line
      character = Serial.read();       //  Read each byte and cast to char type
      
      if (character != ',' && character != '\n')
      {
        content.concat(character);       //  Unless the current character is a space or new line, concatenate each character to the content string
      }
      else // If the current character is a space or newline then we need to split the string to obtain individual angle commands.
      {
        angles_strings[angle_count] = content;  // Write the current string to the string array of angles
        content = "";  // Reset the current string to a blank string.
        angle_count += 1;  // Increment the angle_strings[] array index for the next angle string.
      }
            
      delay(5);                        //  Delay by 5 ms each time a byte is read to allow time for the next character to be sent over serial.
  }
  angle_count = 0;
  
  if (angles_strings[1] != "") {                 //  If serial data is received, print the serial data to the serial monitor.
      //parse_serial_data();
      
      for (int i = 0; i < num_servos; i++)
      {
        //Serial.println(angles_strings[i]);
        angles_ints[i] = atoi(angles_strings[i].c_str());
        //Serial.println(angles_ints[i]);
        angles_strings[i] = "";
      }
      
      map_joint_to_servo(angles_ints);  //  Map the angles from the task frame to the servo frame.  (Servos are at different angles relative to the task frame).  map_joint_angles() needs to be performed before write_joint_angles()
      write_joint_angles(servos, angles_ints);  //  Write the angles to the servos, write_joint_angles(Array of servos, Array of servo angles)

  }
  
}

void map_joint_to_servo(int servo_angles[])
{
  //  maps joint angles to servo angles
  //  GIVEN:  joint angles, q's
  //  PERFORMS:  map(joint value, joint_low, join_high, servo_low, servo_high);
  //  RETURNS:  servo angles
  
  //  NOTE:  I expect this function can be a source of error for us when building.
  //         It may or may not be necessary depending on the range of motion of the servos
  //         and how accurate the servos are at writing specific angles.  (Does 1 'tick' on the servos = 1 degree?)
  //         (Basically the purpose is to translate from 'angles' to 'ticks' if necessary)
  //
  //  NOTE:  Assumed values are:
  //         Servo 1 Zero config @ 240 deg
  //         Servo 2 Zero config @ 0 deg
  //         Servo 3 Zero config @ 120 deg

  if (servo_angles[0] - 180 > 0)  //  Check to see if the servo angle is in the negative domain
  {
    servo_angles[0] = -1*(servo_angles[0] - 240) + 157;
    //Serial.println("Negative Domain");
  }
  else  //  case if the servo angle is in the positive domain or equal to zero
  {
    servo_angles[0] = -1*(120 + servo_angles[0]) + 157;
    //Serial.println("Positive Domain");
  }
    
  servo_angles[1] = -1*servo_angles[1] + 157;  //  NOTE:  servo angles need to be limited in matlab to be >= 0 (It can't wrap around at 360)
  servo_angles[2] = -1*(servo_angles[2] - 120) + 157;    
  Serial.println("Servo angle 1");
  Serial.println(servo_angles[0]);
  Serial.println("Servo angle 2");
  Serial.println(servo_angles[1]);
  Serial.println("Servo angle 3");
  Serial.println(servo_angles[2]);
  
}

void write_joint_angles(Servo servos[], int servo_angles[])
{
  //  needs to write the MAPPED joint angles to the servos
  //  GIVEN:  MAPPED joint angles
  //  PERFORMS:  servos[i].write(angle);
  //  RETURNS:  nothing
  for (int i = 0; i < num_servos; i++)
  {
    servos[i].write(servo_angles[i]);
  }

}
