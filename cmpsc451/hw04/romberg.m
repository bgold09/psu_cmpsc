function R = romberg(func, a, b, n)
    R = zeros(n, n);
    h = b - a;    
    R(1, 1) = (feval(func, a) + feval(func, b)) * h/2;
    
    for i = 1 : 1 : n-1
       R(i+1, 1) = R(i, 1)/2;
       h = h/2;
       
       for k = 1 : 1 : 2^(i-1)
           R(i+1, 1) = R(i+1, 1) + h * feval(func, a + (2*k - 1) * h);           
       end
    end
    
    for j = 2 : 1 : n
        for i = j : 1 : n
            R(i, j) = R(i, j-1) + (R(i, j-1) - R(i-1, j-1))/(4^(j) - 1);
        end
    end
end