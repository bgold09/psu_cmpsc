;;; CMPSC 461
;;; January 15, 2013
;;; Sample Code from class, plus a little more

;;; factorial function defined on the natural numbers
(define (fact n)
  (if (zero? n)
      1
      (* n (fact (- n 1)))))

(define (fact2 n)
  (cond ((= n 0) 1)
	(#t (* n (fact2 (- n 1))))))

(define (fact3 n)
  (if (zero? n) 
      1
      (let ( (f (fact3 (- n 1))) )
	(* n f))))


;;; power function define on m and n, for n>=0
;;;  m^0 = 1
;;;  m^n = m * m^(n-1) for n>0
(define (power m n)
  (if (zero? n)
      1
      (* m (power m (- n 1)))))

;;; Sum of powers
;;; (c^0 + c^1 + c^2 + c^3 + ... + c^n)
;;; if n=0 ==> 1
;;; if n>0 ==> c^n plus (c^0 + ... + c^(n-1))
(define (sum-of-powers c n)
  (if (zero? n) 1
      (+ (power c n) (sum-of-powers c (- n 1)))))


;;; gcd(m,n) is the greatest common divisor of two positive numbers
;;; if m==n then return m
;;; if m>n  then gcd(m,n) = gcd(m-n,n)
;;; if m<n  then gcd(m,n) = gcd(m,n-m)

(define (gcd m n)
  (cond ((= m n) m)
	((> m n) (gcd (- m n) n))
	(#t (gcd m (- n m)))))


;;; length of a list
(define (length xs)
  (if (null? xs)
      0
      (+ 1 (length (cdr xs)))))

;;; test if x is a member of a list
(define (member? x xs)
  (if (null? xs)
      #f
      (or (eq? x (car xs))
	  (member? x (cdr xs)))))

;;; sum the elements of list of numbers
(define (sumlist xs)
  (if (null? xs)
      0  ;; empty
      (+ (car xs) (sumlist (cdr xs)))))  ;; non-empty
  
;;; generate the list of prime factors of a number n>0
(define (prime-factors n)
  (pf n 2))

;;; compute the prime factors of n starting with k
(define (pf n k)
  (cond ((= n 1) '())
	((zero? (modulo n k)) (cons k (pf (/ n k) k)))
	(#t (pf n (+ k 1)))))

