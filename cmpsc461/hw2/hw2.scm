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
	  ((> b len) (removeFirst xs a 0))  ;; b > len, return everything before a
	  (#t (removeFirst (removeLast xs (- b a 1)) a 0)))))

;;; remove the last n elements of xs
(define (removeLast xs n)
  (if (null? xs) 
      xs
      (reverse (removeFirst (reverse xs) n 0))))

;;; remove the first elements of xs, from n to a
;;; should be called as (removeFirst xs a 0) to remove the elements before a
(define (removeFirst xs a n)
  (cond ((null? xs) xs)
	((< n a) (removeFirst (cdr xs) a (+ n 1)))
	(#t xs)))


;;; cartProduct(list1, list2) is the Cartesian Product of list1 and list2, 
;;;   i.e. list1 x list2
(define (cartProduct list1 list2)
   (if (or (null? list1) (null? list2)) 
       '()
       (append (makePairs (car list1) list2) (cartProduct (cdr list1) list2))))
       
;;; return a list of all pairs where the first element is a 
;;;   and the second element is an element of xs
;;; e.g. a = 1, xs = '(2 3 4), return '((1 2) (1 3) (1 4))
(define (makePairs a xs)
  (if (null? xs) 
      xs
      (cons (list a (car xs)) (makePairs a (cdr xs)))))

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
		  (cons (list count first) (compress (removeFirst xs count 0))))))))

;;; count the number of elements adjacent to the head of the list
(define (countAdj xs e)
  (cond ((null? xs) 0)
	((equal? (car xs) e) (+ (countAdj (cdr xs) e) 1))
	(#t 0)))

;;; expand(clist) returns an expanded version of a compressed list, 
;;;   as constructed using the compress function above
(define (expand clist)
  (cond ((null? clist) clist)
	((list? (car clist)) (append (duplicate (caar clist) (cadr (car clist))) (expand (cdr clist))))
	(#t (cons (car clist) (expand (cdr clist))))))  ;; head of the list is a single element

;;; return a list with n elements, all elements are x
(define (duplicate n x)
  (if (zero? n)
      '()
      (cons x (duplicate (- n 1) x))))







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
  
;;; construst a new list which is the concatenation of two lists
(define (append xs ys)
  (if (null? xs)
      ys
      (cons (car xs) (append (cdr xs) ys))))

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
