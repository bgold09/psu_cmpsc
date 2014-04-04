;;; Brian Golden
;;; CMPSC 461
;;; Homework 3
;;; 2/2/13

;;; difference(A, B) returns the list 
;;;   A \ B = {x | (x is an element of A) ^ (x is not an element of B)} 
(define (difference A B)
  (filter (lambda (n) (not (fold-left (lambda (x y) (or x (equal? y n))) #f B))) A))

;;; merge-streams takes two ordered streams of integers and 
;;;   merges them into one ordered stream
;;; example: s1 = (1 3 4 8 8 10 ... ), s2 = (0 1 2 9 12 13 30)
;;;   result: (0 1 1 2 3 4 8 8 9 10 12 13 23 30 ... )
(define (merge-streams s1 s2) 
  (cond ((and (empty-stream? s1) (empty-stream? s2)) the-empty-stream)
	((empty-stream? s1) s2)
	((empty-stream? s2) s1)
	(#t (cons (head s1) (delay (cons (head s2) (delay (merge-streams (tail s1) (tail s2)))))))))

;;; orderedTriples returns all ordered triples (i, j, k) such that 
;;;   0 <= i <= j <= k <= n, where n >= 0
(define (orderedTriples n) 
  (if (< n 0)
      '()
      (append (cart-triples (reverse (stream-section (+ n 1) Nats)) n) (orderedTriples (- n 1)))))

;;; cart-triples takes a list ys of elements <= k and returns all triples (i, j, k)
;;;   with all i's <= current element in ys, j = head of ys, k = k
(define (cart-triples ys k)
  (if (or (null? ys) (< k 0))
      '()
      (let ((j (car ys)))
	(append (makeTriples (stream-section (+ j 1) Nats) j k) (cart-triples (cdr ys) k)))))

;;; makeTriples makes a list all possible triples containing an element of xs,
;;;   followed by j, followed by k
(define (makeTriples xs j k)
  (if (null? xs)
      xs
      (cons (list (car xs) j k) (makeTriples (cdr xs) j k))))

;;; caesar-cipher takes a list and an integer n and returns a function f 
;;;   such that (f x) returns the element n elements to the right of x 
;;;   in the original list.
;;; If x is not in the original list, the function should return x
(define (caesar-cipher xs n)
  (lambda (x) 
    (if (member? x xs)
	(nth-element (modulo n (length xs)) (rotate xs (indexof x xs)))
	x)))

;;; rotate rotates the list xs n times such that after each rotation, 
;;;   the first elemement of the new list is the current second element, 
;;;   the second element is the current third, etc. and the last element 
;;;   is the current first element
(define (rotate xs n)
  (if (or (null? xs) (<= n 0))
      xs
      (rotate (append (cdr xs) (list (car xs))) (- n 1))))

;;; rot-n takes a list a and an integer and returns the list of elements of a
;;;   replaced with the element dictated by the caesar-cipher defined for the 
;;;   list of lower-case English letters and the given interger
;;; example: (rot-n 'r 0 c k y ) 13) --> result: (e 0 p x 1)
(define (rot-n a n)
  (if (null? a)
      a
      (cons ((caesar-cipher alph n) (car a)) (rot-n (cdr a) n))))

(define alph '(a b c d e f g h i j k l m n o p q r s t u v w x y z))

;;; auxiliary functions

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

(define head car)
(define (tail stream) (force (cdr stream)))
(define empty-stream? null?)
(define the-empty-stream '())

(define (stream-section n stream)
  (if (= n 0)
      '()
      (cons (head stream) (stream-section (- n 1) (tail stream)))))

(define (integers-starting-from n)
  (cons n (delay (integers-starting-from (+ n 1)))))

(define Nats (integers-starting-from 0))

(define (member? x xs)
  (if (null? xs)
      #f
      (or (eq? x (car xs))
	  (member? x (cdr xs)))))

;;; returns the index of element matching a in xs, assumes a is in xs
(define (indexof a xs)
  (if (eq? (car xs) a)
      0
      (+ 1 (indexof a (cdr xs)))))

(define (nth-element n xs)
  (cond ((or (null? xs) (< n 0)) '())
	((= n 0) (car xs))
	(#t (nth-element (- n 1) (cdr xs)))))
