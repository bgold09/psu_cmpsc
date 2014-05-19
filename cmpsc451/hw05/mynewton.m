function r = mynewton(f, df, x, nmax, tol)
    n = 0; 
    dx = feval(f, x) / feval(df, x);
    fprintf(1, 'n = %d, r = %g\n', n, x-dx);
    
    while (dx > tol && feval(f, x) > tol && n < nmax) 
        n = n + 1;
        x = x - dx;
        dx = feval(f, x) / feval(df, x);
        fprintf(1, 'n = %d, r = %g\n', n, x-dx);
    end
    r = x - dx;
end