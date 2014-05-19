A = diag(-2.011:-0.001:-2.019) + diag(ones(8, 1),1) + diag(ones(8,1),-1);
b = [-0.994974; 1.57407e-3; -8.96677e-4; -2.71137e-3; -4.07407e-3; 
     -5.11719e-3; -5.92917e-3; -6.57065e-3; -0.507084];
x0 = [0.95; 0.9; 0.85; 0.8; 0.75; 0.7; 0.65; 0.6; 0.55];
w = [1 : 0.1 : 1.9];
n = length(w);
iterations = zeros(1, n);

for i = 1 : 1 : n
    [x, nit] = sor(A, b, x0, w(i), 1, 1e-4, 100);
    iterations(i) = nit;
    fprintf(1, 'w = %g, nit = %d, solution:\n', w(i), nit);
    disp(x');
end

plot(w, iterations);