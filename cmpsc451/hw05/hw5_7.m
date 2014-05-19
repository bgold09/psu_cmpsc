r = mynewton('hw5_sq2f', 'hw5_sq2df', 3, 10, 1e-12);
fprintf(1, 'test result for sqrt(2): r = %d\n\n', r);
r = mynewton('hw5_7_fun', 'hw5_7_df', 1.4, 10, 1e-12);
fprintf(1, 'root of f(x) = e^-x - cos(x): r = %d\n', r);