% f(x) = (x^2 + 1)^-1
% use 21 equally spaced nodes on [-5, 5]
hold off;

a = -5;             % lower bound of interval
b = 5;              % upper bound of interval
n = 21;             % number of nodes

x = zeros(1, n);
y = zeros(1, n);

for i = 1 : n       % get x and y data points on interval
    j = 5*cos((2*i + 1)*pi()/42);
    x(i) = j;
    y(i) = (j.^2 + 1).^-1;
end

s = divdiff(x, y);

h2 = (b - a) / 41;
t = a : h2: b;      % 41 equally spaced nodes on [-5, 5]
v = polyvalue(s, x, t);

plot(t, v, '*');
hold on; 
plot(t, v);
plot(x, y, '*');
ezplot('(x.^2 + 1).^(-1)', [a, b]);

e = zeros(1, 42);
for i = 1 : 42
    e(i) = ((i.^2 + 1).^(-1)) - v(i);
end

plot(t, e);
grid;




