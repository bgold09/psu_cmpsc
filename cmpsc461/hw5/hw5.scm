;;; Brian Golden
;;; CMPSC 461
;;; Homework 5
;;; 2/23/13

;;; problem 5 - simulate

;;; simulate(op, stack) takes a list of operations (op) and  a list of bindings
;;;   (stack), each of which consists of a name (symbol) and a value
;;; i.e. a binding (x . val), x is the variable and is bound to the value val
;;; simulate simulates stack operations specified by the op arguments and 
;;;   returns the appropriate values
(define (simulate op stack)
  (let ((fcn (car op))
	(args (cdr op)))
    (cond ((eq? fcn 'access) (cdr (getbinding (car args) stack)))
	  ((eq? fcn 'update) (update (car args) (cadr args) stack))
	  ((eq? fcn 'delete) (filter (lambda (x) (not (equal? (car x) (car args)))) stack))
	  ((eq? fcn 'create) (append stack (list (cons (car args) (cadr args)))))
	  (#t 6))))

;;; getbinding returns the binding on the stack with the variable b
(define (getbinding b stack)
  (cond ((null? stack) '())
	((equal? b (caar stack)) (car stack))
	(#t (getbinding b (cdr stack)))))

;;; update returns the stack with the binding matching x 
;;;   with the new binding val
(define (update x val stack)
  (cond ((null? stack) '())
	((equal? x (caar stack)) (append (list (cons x val)) (cdr stack)))
	(#t (append (list (car stack)) (update x val (cdr stack))))))

