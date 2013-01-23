;;; CMPSC 461
;;; January 17, 2013
;;; 

(define (first xs) (car xs))

(define (robust-first xs)
  (if (null? xs) '()
      (car xs)))

;;; construst a new list which is the concatenation of two lists
;;;  
(define (append xs ys)
  (if (null? xs)
      ys
      (cons (car xs) 
	    (append (cdr xs) ys))))

;;; naive reverse
(define (reverse xs)
  (if (null? xs)
      '()
      (append (reverse (cdr xs)) (list (car xs)))))


;;; tail recursive reverse
(define (trev xs ys)
  (if (null? xs)
      ys
      (trev (cdr xs) (cons (car xs) ys))))

(define (treverse xs)
  (trev xs '()))


;;; generate a list of numbers (n, n-1, ..., 0)
(define (upto n)
  (if (zero? n)
      (list 0)
      (cons n (upto (- n 1)))))


;;; get the last element of a list
;;; assume xs is non-null
(define (last xs)
  (if (= (length xs) 1)  ;; (if (null? (cdr xs))
      (car xs)
      (last (cdr xs))))


;;; return a list with all occurrences of x removed
(define (remove x xs)
  (if (null? xs) 
      xs
      (if (eq? x (car xs))
	  (remove x (cdr xs))
	  (cons (car xs) (remove x (cdr xs))))))

;;; alternative definition 
(define (remove2 x xs)
  (if (null? xs) 
      xs
      (let ((rest (remove2 x (cdr xs))))
	(if (eq? x (car xs))
	    rest
	    (cons (car xs) rest)))))

;;; Sorting
(define (ordered? xs)
  (or (<= (length xs) 1)
      (and (<= (car xs) (cadr xs))
	   (ordered? (cdr xs)))))

;;; insertion sort
(define (isort xs)
  (if (null? xs)
      xs
      (insert (car xs) (isort (cdr xs)))))

;; insert x into the ordered list xs
(define (insert x xs)
  (cond ((null? xs) (list x))
	((> x (car xs)) (cons (car xs) (insert x (cdr xs))))
	(else (cons x xs))))

		  
      

