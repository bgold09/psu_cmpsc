function v = polyvalue(a, x, t)
%polyvalue Summary of this function goes here
%   Detailed explanation goes here

n = numel(x);
tsize = numel(t);
v = zeros(1, tsize);

for i = 1 : tsize
    v(i) = a(n, n);
    for k = n-1 : -1 : 1
        v(i) = v(i)*(t(i) - x(k)) + a(k, k);
    end
end