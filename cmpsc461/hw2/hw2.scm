;;; Brian Golden
;;; CMPSC 461
;;; Homework 2
;;; 1/26/13

;;; slice(xs, a, b) returns a list containing all elements between 
;;;   the a'th (inclusive) and b'th (exclusive) elements of xs
;;; assume a >= 0, b >=0, a and b are within the bounds of xs
(define (slice xs a b)
  (let ((len (length xs)))
    (cond ((or (null? xs) (<= b a)) '())
	  ((> a len) xs)  ;; a > len, return eveything
	  ((> b len) (remove-first-n xs a))  ;; b > len, return everything before a
	  (#t (remove-first-n (remove-last-n xs (- b a 1)) a)))))


;;; cartProduct(list1, list2) is the Cartesian Product of list1 and list2, 
;;;   i.e. list1 x list2
(define (cartProduct list1 list2)
   (if (or (null? list1) (null? list2)) 
       '()
       (append (makePairs (car list1) list2) (cartProduct (cdr list1) list2))))

;;; compress(xs) returns a compressed version of xs such that:
;;;   for each contiguous region of two or more equal elements in the list, 
;;;   replace them with a two element list, where the first element is the 
;;;   number of occurrences and the second element is the element itself
(define (compress xs)
  (cond ((null? xs) xs)
	(#t (let* ((first (car xs))
		  (count (countAdj xs first)))
	      (if (= count 1) 
		  (cons first (compress (cdr xs)))
		  (cons (list count first) (compress (remove-first-n xs count))))))))

;;; expand(clist) returns an expanded version of a compressed list, 
;;;   as constructed using the compress function above
(define (expand clist)
  (cond ((null? clist) clist)
	((list? (car clist)) (append (duplicate (caar clist) (cadr (car clist))) (expand (cdr clist))))
	(#t (cons (car clist) (expand (cdr clist))))))  ;; head of the list is a single element

;;; interleaves the first half of xs and the reverse of the second half of xs
;;; (e.g. list = '(1 8 4 9 5 2 3 7 6) --> output = '(1 6 8 7 4 3 9 2 5))
(define (mangle xs)
  (if (null? xs) 
      xs
      (let* ((len (length xs))
	     (m (ceiling (/ len 2)))
	     (first (remove-last-n xs (- m 1)))		;; first half of xs
	     (rest (reverse (remove-first-n xs m))))	;; reverse of second half of xs
	(if (odd? len)
	    (interleave first rest)   			   ;; interleave the two parts
	    (interleave first (remove-last-elem rest)))))) ;; remove last element of rest, then interleave

;;;;;;; Auxiliary functions

;;; remove the last element of xs
(define (remove-last-elem xs)
  (if (null? xs) 
      xs
      (reverse (cdr (reverse xs)))))

;;; remove the last n elements of xs
(define (remove-last-n xs n) 
  (if (or (null? xs) (zero? n))
      xs
      (remove-last-n (remove-last-elem xs) (- n 1))))

;;; remove the first n elements of xs
(define (remove-first-n xs n)
  (if (or (null? xs) (zero? n))
      xs
      (remove-first-n (cdr xs) (- n 1))))

;;; return a list of all pairs where the first element is a 
;;;   and the second element is an element of xs
;;; e.g. a = 1, xs = '(2 3 4), return '((1 2) (1 3) (1 4))
(define (makePairs a xs)
  (if (null? xs) 
      xs
      (cons (list a (car xs)) (makePairs a (cdr xs)))))

;;; return a list with n elements, all elements are x
(define (duplicate n x)
  (if (zero? n)
      '()
      (cons x (duplicate (- n 1) x))))

;;; count the number of elements adjacent to the 
;;;   head of the list which are the same
(define (countAdj xs e)
  (cond ((null? xs) 0)
	((equal? (car xs) e) (+ (countAdj (cdr xs) e) 1))
	(#t 0)))

;;; combine xs and ys by first taking the head of xs and then head of ys 
;;;   on each iteration
;;; (e.g. xs = '(1 2 3), ys = '(4 5 6) --> output = '(1 4 2 5 3 6)
(define (interleave xs ys)
  (cond ((and (null? xs) (null? ys)) '())
	((null? xs) ys)
	((null? ys) xs)
	(#t (append (list (car xs) (car ys)) (interleave (cdr xs) (cdr ys))))))

