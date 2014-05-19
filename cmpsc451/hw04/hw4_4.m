format;
R1 = romberg('sin', 0, pi, 8);
R2 = romberg('sqrt', 0, 1, 8);
E1 = zeros(1, 8);
E2 = zeros(1, 8);

for i = 1 : 1 : 8
    E1(i) = abs(2 - R1(i, i));
    E2(i) = abs(2/3 - R2(i, i)); 
end

fprintf(1, 'Romberg approx of integral sin(x), 0 <= x <= pi: %d\n', R1(8, 8));
fprintf(1, 'Romberg approx of integral sqrt(x), 0 <= x <= 1: %d\n\n', R2(8, 8));
format short e;
disp('Romberg triangle, sin(x), x in [0, pi]');
disp(R1);
disp('Romberg errors, sin(x), x in [0, pi]');
disp(E1);
disp('Romberg triangle, sqrt(x), x in [0, 1]');
disp(R2);
disp('Romberg errors, sqrt(x), x in [0, 1]');
disp(E2);

fprintf(1, 'quad(''sin'', 0, pi, 1e-9) = %d\n', quad('sin', 0, pi, 1e-9));
fprintf(1, 'quadl(''sin'', 0, pi, 1e-9) = %d\n', quadl('sin', 0, pi, 1e-9));
fprintf(1, 'quad(''sqrt'', 0, 1, 1e-9) = %d\n', quad('sqrt', 0, 1, 1e-9));
fprintf(1, 'quadl(''sqrt'', 0, 1, 1e-9) = %d\n', quadl('sqrt', 0, 1, 1e-9));
