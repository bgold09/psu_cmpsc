function ls=lspline(t, y, x)
% lspline computes the linear spline
% Inputs:
% t: vector, contains the knots
% y: vector, contains the interpolating values at knots
% x: vector, contains points where the lspline should be computed
% Output:
% ls: vector, contains the values of lspline at points x

n = length(t);
m = length(x);
ls = zeros(size(x)); 

for j=1:m
  v = x(j);
  for i=n-1:-1:1
    if (v-t(i)) >= 0
      break
    end
  end
  ls(j) = y(i) + (y(i+1) - y(i)) / (t(i+1) - t(i)) * (v - t(i));
end

end
