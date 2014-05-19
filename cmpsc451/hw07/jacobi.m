function [x, nit] = jacobi(A, b, x0, tol, nmax)  
    nit = 0;
    n = length(A);
    x = zeros(1, n);
    norm = 1;
    res = 1;
    xp = x0;
    while (norm > tol && res > tol && nit < nmax)
        for i = 1 : 1 : n
            x(i) = b(i);
            if (i - 1 > 0)
                x(i) = x(i) - A(i, i-1) * xp(i-1);
            end
            
            if (i + 1 <= n)
                x(i) = x(i) - A(i, i+1) * xp(i+1);
            end
            x(i) = x(i)/A(i, i);
        end
        
        res = sum((A*x' - b).^2)^0.5; % L2-norm
        norm = sum((x - xp).^2)^0.5;  % L2-norm
        xp = x;
        nit = nit + 1;
    end
end

