function [x, nit] = sor(A, b, x0, w, d, tol, nmax)
% SOR : solve linear system with SOR iteration
% Usage: [x,nit]=sor(A,b,x0,omega,d,tol,nmax)
% Inputs:
%   A: an n x n-matrix,
%   b: the rhs vector, with length n
%   x0: the start vector for the iteration
%   tol: error tolerance
%   w: relaxation parameter, (0 < w < 2),
%   d: band width of A.
% Outputs:
%   x: the solution vector
%   nit: number of iterations

    nit = 0;
    n = length(A);
    x = zeros(n, 1);
    norm = 1;
    res = 1;
    xp = x0;
    while (norm > tol && res > tol && nit < nmax)
        for i = 1 : 1 : n
            x(i) = b(i);
            j = i - 1;
            ct = 0;
            while (j > 0 && ct < d)
                x(i) = x(i) - A(i, j) * x(j);
                j = j - 1;
                ct = ct + 1;
                
            end
            
            j = i + 1;
            ct = 0;
            while (j <= n && ct < d)
                x(i) = x(i) - A(i, j) * xp(j);
                j = j + 1;
                ct = ct + 1;
            end
            x(i) = x(i) * w/A(i, i) + (1 - w) * xp(i);
        end
        res = sum((A*x - b).^2)^0.5;  % L2-norm
        norm = sum((x - xp).^2)^0.5;  % L2-norm
        xp = x;
        nit = nit + 1;
    end
end

