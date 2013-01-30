;;; Brian Golden
;;; CMPSC 461
;;; Homework 3
;;; 2/2/13

;;; difference(A, B)



;;; merge-streams takes two ordered streams of integers and 
;;;   merges them into one ordered stream
;;; example: s1 = (1 3 4 8 8 10 ... ), s2 = (0 1 2 9 12 13 30)
;;;   result: (0 1 1 2 3 4 8 8 9 10 12 13 23 30 ... )
(define (merge-streams s1 s2) 
  (cond ((and (empty-stream? s1) (empty-stream? s2)) the-empty-stream)
	((empty-stream? s1) s2)
	((empty-stream? s2) s1)
	(#t (cons (head s1) (delay (cons (head s2) (delay (merge-streams (tail s1) (tail s2)))))))))


;;; auxiliary functions

(define head car)
(define (tail stream) (force (cdr stream)))
(define empty-stream? null?)
(define the-empty-stream '())

(define ones (cons 1 (delay ones)))

(define (stream-section n stream)
  (if (= n 0)
      '()
      (cons (head stream) (stream-section (- n 1) (tail stream)))))

(define (integers-starting-from n)
  (cons n (delay (integers-starting-from (+ n 1)))))

(define Nats (integers-starting-from 0))
