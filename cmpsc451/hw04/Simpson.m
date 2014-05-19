function v = Simpson(func, a, b, n)
    h = (b - a) / 2 / n;
    xodd = [a+h : 2*h : b-h];
    xeven = [a + 2*h : 2*h : b - 2*h];
    v = h/3 * (feval(func, a) + 4*sum(feval(func, xodd)) + 2*sum(feval(func, xeven)) + feval(func, b));
end
