;;; CMPSC 461
;;; January 24, 2013
;;; 

(define (random-list n k)
  (if (zero? n) '()
      (cons (random k) (random-list (- n 1) k))))

(define (filter p xs)
  (if (null? xs)
      xs
      (let ((rest (filter p (cdr xs)))
	    (first (car xs)))
      (if (p first)
	  (cons first rest)
	  rest))))

(define (qSort xs)
  (if (< (length xs) 2)
      xs
      (let* ((pivot (car xs))
	     (rest (cdr xs))
	     (left (filter (lambda (n) (<= n pivot)) rest))
	     (right (filter (lambda (n) (not (<= n pivot))) rest)))
	(append (qSort left)  (cons pivot (qSort right))))))


(define (qSort2 p xs)
  (if (< (length xs) 2)
      xs
      (let* ((pivot (car xs))
	     (rest (cdr xs))
	     (left (filter (lambda (n) (p n pivot)) rest))
	     (right (filter (lambda (n) (not (p n pivot))) rest)))
	(append (qSort2 p left)  (cons pivot (qSort2 p right))))))

(define (qSort3 p) 
  (lambda (xs)
  (if (< (length xs) 2)
      xs
      (let* ((pivot (car xs))
	     (rest (cdr xs))
	     (left (filter (lambda (n) (p n pivot)) rest))
	     (right (filter (lambda (n) (not (p n pivot))) rest)))
	(append ((qSort3 p) left)  (cons pivot ((qSort3 p) right)))))))



(define (f x y) (+ (* x x) (* y y)))
(define f (lambda (x y) (+ (* x x) (* y y))))

(define (g x) (lambda (y) (+ (* x x) (* y y))))
(define g (lambda (x) (lambda (y) (+ (* x x) (* y y)))))

(define (Curry f) (lambda (x) (lambda (y) (f x y))))
(define (unCurry g) (lambda (x y) ((g x) y)))


(define qSort4 (Curry qSort2))

;;; SKI Combinators
(define I (lambda (x) x))
(define K (lambda (x) (lambda (y) x)))
(define S (lambda (x) (lambda (y) (lambda (z) ((x z) (y z))))))


;;; Lazy Lists
(define (list->llist xs)
  (if (null? xs)
      '()
      (cons (car xs) (lambda () (list->llist (cdr xs))))))

(delay (+ 2 3))


;;; Streams

(define head car)
(define (tail stream) (force (cdr stream)))
(define empty-stream? null?)
(define the-empty-stream '())

(define ones (cons 1 (delay ones)))

(define (stream-section n stream)
  (if (= n 0)
      '()
      (cons (head stream)
	    (stream-section (- n 1) (tail stream)))))

(define (integers-starting-from n)
  (cons n (delay (integers-starting-from (+ n 1)))))

(define Nats (integers-starting-from 0))

(define (add-streams s1 s2)
  (let ((h1 (head s1))
	(h2 (head s2)))
    (cons (+ h1 h2)
	  (delay (add-streams (tail s1) (tail s2))))))

;;;(stream-section 10 (add-streams ones Nats))

;; Fibonacci
(define fibs
  (cons 0
	(delay (cons 1 (delay (add-streams (tail fibs) fibs))))))

(define (nth-element n stream)
  (if (= n 0)
      (head stream)
      (nth-element (- n 1) (tail stream))))

(define integers (cons 1 (delay (add-streams ones integers))))

(define (fibgen a b)
  (cons a (delay (fibgen b (+ a b)))))
(define fibs2 (fibgen 0 1))

(define (sieve stream)
  (cons (head stream)
	(delay (sieve (filter-stream (lambda (x) (not (divisible? x (head stream))))
				     (tail stream))))))

(define (divisible? x y) (= (remainder x y) 0))

(define (filter-stream p stream)
  (cond ((empty-stream? stream) the-empty-stream)
	((p (head stream)) (cons (head stream) 
				 (delay (filter-stream p (tail stream)))))
	(else (filter-stream p (tail stream)))))

(define primes (sieve (integers-starting-from 2)))

