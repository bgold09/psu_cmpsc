% f(x) = tan(x)
% f(x+h) = f(x) + hf'(x) + (1/2!)h^2f"(x) + O(h^3)
% evaluate at x = 1

% truncation error = (1/3)*h^2*f"'(?)
% f"'(?) = -2 (-2+cos(2?)) sec^4(?)

y = zeros(1, 4);    % approimate values at tan(1)
e = zeros(1, 4);    % error values
h = [0.1, 0.01, 0.001, 0.0001];

% let ? = 0.5
xi = 0.5;
for i = 1 : 4
    y(i) = tan(1) + h(i)*sec(1) + 0.5*(h(i)).^(2)*2*tan(1)*(sec(1)).^2;
    e(i) = (1/3)*h(i).^2*-2*(-2+cos(2*xi))*sec(xi).^4;
end

loglog(e, h);
hold on;
loglog(e, h, '*');
grid;