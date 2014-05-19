x = [0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1];
y = [0.7829, 0.8052, 0.5753, 0.5201, 0.3783, 0.2923, 0.1695, 0.0842, 0.0415, 0.009, 0];
p1 = polyval(polyfit(x, y, 1), x);
p2 = polyval(polyfit(x, y, 2), x);
p4 = polyval(polyfit(x, y, 4), x);
p8 = polyval(polyfit(x, y, 8), x);
plot(x, p1, x, p2, x, p4, x, p8, x, y, 'x');