function v = trapezoid(func, a, b, n)
    h = (b - a) / n;
    x = [a+h : h : b-h];
    v = h * ((feval(func, a) + feval(func, b))/2 + sum(feval(func, x)));
end

