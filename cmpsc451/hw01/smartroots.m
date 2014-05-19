function [r1, r2] = smartroots(a, b, c)
%smartroots compute the roots of a quadratic function more intelligently
%   Use the quadratic formula and the fact that r1*r2 = c/a to compute
%   roots of a quadratic function

r1 = (-b + sqrt(b.^2 - 4*a*c))/(2*a);
r2 = c/(a * r1);

end

