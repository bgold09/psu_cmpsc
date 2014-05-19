m = 5;
x = [0, 0.2, 0.4, 0.6, 0.8, 1];
y = [1.996, 1.244, 0.81, 0.541, 0.375, 0.259];
S = log(y);

xsum = sum(x);
xsqsum = sum(x.^2);
lnsum = sum(log(1 + x));
xlnsum = sum(x .* log(1 + x));

A = [m+1, xsum, -lnsum; xsum, xsqsum, -xlnsum; lnsum, xlnsum, -sum((log(1 + x)).^2)];
b = [sum(S); sum(S .* x); sum(S .* log(1 + x))];
sol = A\b;

a0 = exp(sol(1)); 
yb = a0 * exp(sol(2) * x) ./ (1 + x).^sol(3);
plot(x, yb);

fprintf(1, 'quadratic sum: %f\n', sum((y - S).^2));