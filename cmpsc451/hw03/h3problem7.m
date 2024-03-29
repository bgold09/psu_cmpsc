t1 = [.032, .225, .389, .547, .699, .813, .938, 1.193, 1.321, 1.461, 1.549, 1.628];
y1 = [.924, .763, .649, .526, .409, .307, .219, .225, .351, .485, .652, .781];
y1 = -y1;

t2 = [1.628, 1.660, 1.719, 1.751, 1.809, 1.824];
y2 = [.781, .757, .725, .699, .661, .649];
y2 = -y2;

t3 = [1.824, 1.841, 1.853, 1.877, 1.897, 1.926, 1.958, 1.982, 2.017];
y3 = [.649, .687, .743, .789, .865, .921, .971, 1.032, 1.105];
y3 = -y3;

z1 = cspline(t1, y1);
z2 = cspline(t2, y2);
z3 = cspline(t3, y3);

x1 = 0.32:0.01:1.628;
x2 = 1.628:0.01:1.824;
x3 = 1.824:0.01:2.017;

S1 = cspline_eval(t1, y1, z1, x1);
S2 = cspline_eval(t2, y2, z2, x2);
S3 = cspline_eval(t3, y3, z3, x3);

plot(x1, S1, x2, S2, x3, S3);