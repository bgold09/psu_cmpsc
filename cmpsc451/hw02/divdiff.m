function a = divdiff(x, y)
%divdiff Summary of this function goes here
%   Detailed explanation goes here

n = numel(x);

b = zeros(n, n);

for i = 1 : n
    b(i, 1) = y(i);
end

% i is row index, j is column index
for j = 2 : n
   for i = j : n
       b(i, j) = (b(i, j - 1) - b(i - 1, j - 1))/(x(i) - x(i - j + 1));
   end
end

a = b;

end

