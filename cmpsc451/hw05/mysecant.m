function r = mysecant(f, x0, x1, nmax, tol)
    n = 0; 
    dx = (x1 - x0) / (feval(f, x1) - feval(f, x0)) * feval(f, x1);
    fprintf(1, 'n = %d, r = %g\n', n, x1-dx);
    
    while (dx > tol && feval(f, x1) > tol && n < nmax) 
        n = n + 1;
        x0 = x1;
        x1 = x1 - dx;
        dx = (x1 - x0) / (feval(f, x1) - feval(f, x0)) * feval(f, x1);
        fprintf(1, 'n = %d, r = %g\n', n, x1-dx);
    end
    r = x1 - dx;
end