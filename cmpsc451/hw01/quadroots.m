function [r1, r2] = quadroots(a, b, c)
%quadroots compute the roots of a quadratic function
%   Use the quadratic formula to compute roots of a quadratic function
%   r1,2 = (-b +- sqrt(b.^2 - 4*a*c))/(2*a)

r1 = (-b + sqrt(b.^2 - 4*a*c))/(2*a);
r2 = (-b - sqrt(b.^2 - 4*a*c))/(2*a);

end