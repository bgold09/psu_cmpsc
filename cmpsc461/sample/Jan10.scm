;;; CMPSC 461
;;; January 10, 2013
;;; Sample Code from class, plus a little more

(define (area r) (* pi (sqr r)))

;;; Pi
(define pi (* 4 (- (* 4 (atan (/ 1 5))) (atan (/ 1 239)))))

;;; Two versions of square function
(define (sqr n) (* n n))
(define square (lambda (n) (* n n)))

;;; Recursive functions
(define (fact n)  (if (= n 0) 1 (* n (fact (- n 1)))))

(define (power m n)
  (if (zero? n) 
      1
      (* m (power m (- n 1)))))

;;; A faster version of the power function
(define (fpower m n)
  (cond ((zero? n) 1)
	((even? n) (square (fpower m (/ n 2))))
	(#t (* m (fpower m (- n 1))))))



;;; A simple test of primality
(define (prime? n) (nodivisors n (floor (sqrt n))))

;;; return true if n has no divisors between 2 and k
(define (nodivisors n k)
  (or (< k 2)
      (and (> (modulo n k) 0)
	   (nodivisors n (- k 1)))))



;;; Fermat's Little Theorem:  if n is prime and a is any positive
;;; integer less than n, then a raised to the nth power is congruent
;;; to a modulo n
;;; if n is not prime, then most numbers a<n will not satisfy this
;;; relation
;;; Thm:  if n is prime, 0<a<n, a^n mod n = a

(define (fermat-test n)
  (let ((a (+ 2 (random (- n 2))))) ;;; random number from 2 to (n-1)
       (= (modulo (fpower a n) n) a)))

;;; test if n is prime by applying "times" fermat tests
(define (fermat-prime? n times)
  (cond ((zero? times) #t)
	((fermat-test n) (fermat-prime? n (- times 1)))
	(else #f)))


;;; The above code slows down on large numbers due to the very
;;; large powers that need to be computed.  The code below avoids
;;; these large numbers by using some standard equalities:
;;;   ab mod n = [(a mod n)(b mod n)] mod n

(define (fast-fermat-prime? n times)
  (cond ((zero? times) #t)
	((fermat-test2 n) (fast-fermat-prime? n (- times 1)))
	(else #f)))


(define (fermat-test2 n)
  (let ((a (+ 2 (random (- n 2)))))  ;;; random number from 2 to n-1
    (= (nmod a (- n 1) n) 1)))


;;; compute a^n mod m using the fact that 
;;;    (a^(2k)) mod m = [(a^k mod m)(a^k mod m)] mod m
;;;    (a^(n+1) mod m = (a(a^n mod m))  mod m
(define (nmod a n m)
  (cond ((= n 0) 1)
        ((even? n)
         (modulo (sqr (nmod a (/ n 2) m))
                 m))
        (else
         (modulo (* a (nmod a (- n 1) m))
                 m))))
 

;; Some Mersenne Primes:
;;  524287, 2147483647, 2305843009213693951, 
;; 618970019642690137449562111, 162259276829213363391578010288127, 
;; 170141183460469231731687303715884105727 



;; approximation of e^x using a summation series
;; e^x = x^0/0! + x/1! + x^2/2! + ...
;; compute the approximation out to the kth term
(define (exapprox x k)
  (if (= k 0)
      1
      (+ (/ (fpower x k) (fact k)) (exapprox x (- k 1)))))

