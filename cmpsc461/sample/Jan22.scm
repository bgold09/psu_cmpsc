;;; CMPSC 461
;;; January 22, 2013
;;; 


(define (random-list n k)
  (if (zero? n) '()
      (cons (random k) (random-list (- n 1) k))))


;;; Selection Sort
(define (sSort xs)
  (if (null? xs)
      xs
      (let ((first (least xs)))
	(cons first (sSort (remove first xs))))))

;;; return the smallest element of a non-empty list
(define (least xs)
  (if (null? (cdr xs))
      (car xs)
      (min (car xs) (least (cdr xs)))))

(define (min x y)
  (if (<= x y) x y))

;;; remove 1 occurrence of x from xs
;;; it is guaranteed to occur
(define (remove x xs)
  (if (eq? x (car xs))
      (cdr xs)
      (cons (car xs) (remove x (cdr xs)))))


;;; Merge Sort

;;; merge two ordered lists
(define (merge xs ys)
  (cond ((null? xs) ys)
	((null? ys) xs)
	((<= (car xs) (car ys)) (cons (car xs) (merge (cdr xs) ys)))
	(else (cons (car ys) (merge xs (cdr ys))))))


(define (mSort xs)
  (let ((len (length xs)))
    (if (< len 2)
	xs
	(let* ((mid (floor (/ len 2)))
	       (left (first mid xs))
	       (right (skip mid xs)))
	  (merge (mSort left) (mSort right))))))

;;; return the first n elements of xs
(define (first n xs)
  (if (zero? n)
      '()
      (cons (car xs) (first (- n 1) (cdr xs)))))

;;; skip over the first n elements of xs and return remaining list
(define (skip n xs)
  (if (zero? n)
      xs
      (skip (- n 1) (cdr xs))))


(define (twice f n) (f (f n)))

(define (plus x) 
  (lambda (n) (+ n x)))


(define (map f xs)
  (if (null? xs)
      xs
      (cons (f (car xs)) (map f (cdr xs)))))

(define (filter p xs)
  (if (null? xs)
      xs
      (if (p (car xs))
	  (cons (car xs) (filter p (cdr xs)))
	  (filter p (cdr xs)))))

(define (filter p xs)
  (if (null? xs)
      xs
      (let ((rest (filter p (cdr xs)))
	    (first (car xs)))
      (if (p first)
	  (cons first rest)
	  rest))))

(define (foldr f z xs)
  (if (null? xs)
      z
      (f (car xs) (foldr f z (cdr xs)))))

;;; (foldr f z (x0 x1 x2)) ---> (f x0 (f x1 (f x2 z)))

(define (sumlist xs) (foldr + 0 xs))

(define (foldl f z xs)
  (if (null? xs)
      z
      (foldl f (f z (car xs) (cdr xs)))))

;;; (foldl f z (x0 x1 x2)) ---> (f (f (f z x0) x1) x2)


(define (qSort xs)
  (if (< (length xs) 2)
      xs
      (let* ((pivot (car xs))
	     (rest (cdr xs))
	     (left (filter (lambda (n) (<= n pivot)) rest))
	     (right (filter (lambda (n) (> n pivot)) rest)))
	(append (qSort left)  (cons pivot (qSort right))))))



