function [ thetas ] = new_inv_kin( x, y, phi, psi_i, l_1i, l_2i, l_3i, a_ix, a_iy)
    %   Performs inverse kinematics for a (R)RR planar parallel manipulator.
    %   12/19/2013


    c_ix = x + l_3i*cosd(phi+psi_i);
    c_iy = y + l_3i*sind(phi+psi_i);

    e = 2*(c_ix - a_ix)*l_1i;
    f = 2*(c_iy - a_iy)*l_1i;
    g = l_2i^2 - l_1i^2 - (c_ix - a_ix)^2 - (c_iy - a_iy)^2;

    theta_up = 2*atand( (-1*f + sqrt(e^2 + f^2 - g^2)) / (g-e));
    theta_down = 2*atand( (-1*f - sqrt(e^2 + f^2 - g^2)) / (g-e));
    
    
    thetas = [theta_up, theta_down];
        
    
    %%
    theta2_up = atan2d((c_iy - a_iy - l_1i*sind(theta_up)), (c_ix - a_ix - l_1i*cosd(theta_up))) - theta_up;
    theta2_down = atan2d((c_iy - a_iy - l_1i*sind(theta_down)), (c_ix - a_ix - l_1i*cosd(theta_down))) - theta_down;
    
    thetas2 = [theta2_up, theta2_down];

end

