n = [4, 8, 16, 32, 64, 128];
len = length(x);
V = zeros(1, len);
E = zeros(1, len);
val = 1 - exp(-0.8);

for i = 1 : 1 : 6
    V(i) = trapezoid('funItg', 0, 0.8, n(i));
    E(i) = abs(V(i) - val);
end

loglog(n, E);