%   
%   (R)RR planar parallel manipulator
%   12/19/2013
%   
%   This script sends data to the Arduino and listens for data returned.
%   It uses serial port COM4 at 57600 baud.
%   
%   NOTES:
%   The serial monitor in Arduino must be closed (and any other program
%   using COM4) orelse the port will be busy and the script won't connect.
%   
%   You need to close the COM port before the program ends.  If the COM port
%   is not closed then it will keep COM4 busy.  Subsequent attempts to run 
%   the script will fail because the COM4 port will be busy.  You must 
%   disconnect the Arduino and close MATLAB, then restart in this case....
%   Close the COM port instead!!
%   
%   fclose(s);
%   delete(s);
%   clear s;
%
%

clear all;
clc;

s = serial('COM4', 'BaudRate', 57600, 'Parity', 'none');
%s.BaudRate = 57600;            % define baud rate
set(s, 'terminator', 'LF');    % define the terminator for println
fopen(s);
pause(2);

try                            % use try catch to ensure fclose
    
    %Define the x,y coordinate of the task point here.  Some examples
    %provided are a trace of a star, rectangle, and a smooth rectangle.
    %Phi is defined as a constant in the loop below because I never really 
    %got to test using different phi angles.  This can be modified
    %to the same form as xs, ys to provide variable phi values.
        
    %Star coords
    xs = [20,-20,20,0,-20,20];
    ys = [0,0,-20,20,-20,0];
        
    %Rectangle coords
    xs = [20,20,-20,-20,20];
    ys = [-20,20,20,-20,-20];
    
    %Smoother rectangle coords
    xs = [20*ones(1,41), 20:-1:-20, -20*ones(1,41), -20:1:20];
    ys = [-20:1:20, 20*ones(1,41), 20:-1:-20, -20*ones(1,41)];
    
    for data_point = 1:length(xs)
        
    
        x = xs(data_point);%20
        y = ys(data_point);%20
        phi = 0;                            %   Define phi here.

        l_1i = 57;
        l_2i = 114;
        l_3i = 30;
        l_i = [l_1i, l_2i, l_3i];

        %servo 1
        psi_1 = 210;
        a_1x = -116;
        a_1y = -67;

        %servo 2
        psi_2 = 330;
        a_2x = 116;
        a_2y = -67;
        
        %servo 3
        psi_3 = 90;
        a_3x = 0;
        a_3y = 134;
        
        a_ixs = [a_1x, a_2x, a_3x];
        a_iys = [a_1y, a_2y, a_3y];
        psi_is = [psi_1, psi_2, psi_3];
        
        thetas = [];
        for i = 1:3
            psi_i = psi_is(i);
            a_ix = a_ixs(i);
            a_iy = a_iys(i);
                        
            thetas = [thetas; new_inv_kin(x, y, phi, psi_i, l_1i, l_2i, l_3i, a_ix, a_iy)];
        end
        
        servo_angle_1 = thetas(1,1);
        servo_angle_2 = thetas(2,1);
        servo_angle_3 = thetas(3,1);
        servo_angles = [servo_angle_1, servo_angle_2, servo_angle_3];
        servo_angles
        if(isreal(servo_angles))
            if servo_angle_1 < 0
                servo_angle_1 = 360 + servo_angle_1
            end

            if servo_angle_3 < 0
                servo_angle_3 = 360 + servo_angle_3
            end

            servo_command = strcat(int2str(servo_angle_1), ',', int2str(servo_angle_2), ',', int2str(servo_angle_3))
            fprintf(s, servo_command);
            %fprintf(s, int2str(i));
            my_text = fscanf(s);
            my_text
            %i
            pause(0.05);
        else
            disp('Coordinate is outside the feasible space.');
        end
        pause(0.1);
    end
    
    
    
catch me
    fclose(s);                 % always want to close the serial port
    delete(s);
    clear s;
end           

fclose(s);
delete(s);
clear s;


