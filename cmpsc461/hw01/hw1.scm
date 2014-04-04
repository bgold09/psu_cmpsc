;;; Brian Golden
;;; CMPSC 461
;;; Homework 1
;;; 1/19/13

;;; binom(n, k) is the binomial coefficient "n choose k"
;;; if (k == 0 && n >= 0) return 1
;;; if (n == 0 && k > 0)  return 0
;;; else  binom(n, k) = binom(n - 1, k - 1) + binom (n - 1, k)
(define (binom n k)
  (cond ((and (= k 0) (>= n 0)) 1)
	((and (= n 0) (> k 0))  0)
	(#t (+ (binom (- n 1) (- k 1)) (binom (- n 1) k)))))

;;; approx-pi(n) approximate pi by limit of sum( f(k/n) * (k/n - (k-1)/n) )
;;;   as n --> infinity, for all integers n >= 1
;;; if (n < 1) return 0
;;; else       approx-pi(n) = sum(sf(k, n)) for k in [1, n]
(define (approx-pi n) 
  (if (< n 1) 0
      (summation 1 n)))

;;; summation(k, n) is the summation from k to n evaluated at sf
;;; sf is the procedure/function defined below
;;; if (n == k) return sf(n, n), reached the end of the summation
;;; else        summation(k, n) = sf(k, n) + summation(k + 1, n)
(define (summation k n)
  (if (= k n)
      (sf n n)
      (+ (sf k n) (summation (+ k 1) n))))

;;; f(x) = 4 * sqrt(1 - x^2), used for pi approximation
;;; if !(-1 <= x <= 1) then f(x) is imaginary, just return false
(define (f x)
  (if (and (<= x 1) (<= -1)) (* 4 (sqrt (- 1 (* x x)))) #f))

;;; sf(k, n) = f(k/n) * (k/n - (k-1)/n), used for pi approximation
(define (sf k n) (* (f (/ k n))  (- (/ k n) (/ (- k 1) n))))

;;; sinTerms(x, e) approximates sin(x) using the Taylor series expansion
;;; sin(x) ≈ x - x^3/3! + x^5/5! + x^7/7! + ...
;;; let diff be the absolute value of (current approximation at x) - (sin(x))
;;; if (diff < e) return num
;;; else          sinTerms(x, e) = 1 + countTerms(x, e, 1, a, r) 
(define (sinTerms x e) 
  (let ((a (approx-sin 0 x 1))		;; a = approx(0, x, 1) = (x^1/x! = x)
	(r (sin x))			       ;; r = sin(x), the "actual" value
	(diff (- (approx-sin 0 x 1) (sin x)))) ;; diff = a - r
	(if (< diff e) 
	    1
	    (countTerms x e 1 a r))))

;;; countTerms(x, e, n, a, r) used to approximate sin(x), count terms needed
;;; x = value to approximate sin(x)
;;; e = error tolerance
;;; n = number of terms so far
;;; a = approximate sum so far, with n terms
;;; r = real value of sin(x)
(define (countTerms x e n a r)
  (let ((av (approx-sin a x (- (* 2 (+ n 1)) 1))))  ;; new approx value
  (if (< (absolute (- av r)) e)			    ;; if (|av - r| < e) done
      (+ n 1)
      (countTerms x e (+ n 1) av r))))		    ;; try next term

;;; approx-sin(v, x, k) adds a term of the Taylor series sine approximation to v
;;; approx-sin(v, x, k) = v + (x^k / k!)
(define (approx-sin v x k)
  (+ v (* (/ (expt x k) (fact k)) (expt -1 (* (floor (/ k 2)))))))

;;; myRand(n, m, a, c, seed) is a Linear Congurential Generator which
;;;   uses the algorithm: r_(n+1) = (a * r_n + c) mod m 
;;; where r_0 is the seed; for all i, 0 <= r_i < m 
;;; iterate n times to get the pseudo-random number r_n
(define (myRand n m a c seed)
  (if (< n 0)
      seed
      (randHelper n m a c (modulo (+ (* a seed) c) m) 1)))

;;; at the ith iteration, compute r_(i+1) if i < n
;;; prev = r_i
(define (randHelper n m a c prev i)
  (if (>= i n)    		;; performed enough iterations
      prev			;; return r_i
      (randHelper n m a c (modulo (+ (* a prev) c) m) (+ i 1))))

;;; quality? tests if the given values a, c, m will  
;;; produce quality random numbers using a LCG
(define (quality? a c m)
  (let ((avg (qualityHelper 1 m a c 0 0)))
       (and (>= avg 40) (<= avg 60))))

;;; qualityHelper produces the sum of 100 psuedo-random numbers using a LCG
(define (qualityHelper i m a c seed sum)
  (if (eq? i 100)  ;; done generating the 100 numbers
      (+ sum ( / (myRand 100 m a c seed) m))
      (qualityHelper (+ i 1) m a c seed (+ sum (/ (myRand (+ i 1) m a c seed) m) ))))

;;; factorial function defined on the natural numbers
(define (fact n)
  (if (zero? n) 1
      (* n (fact (- n 1)))))

;;; absolute value function defined on the real numbers
(define (absolute x)
  (if (< x 0) (* x -1) x))

