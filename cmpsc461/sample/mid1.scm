;;; midterm 1 problems


;;; p5 - recursive fcn for product of a list
(define (lprod xs)
  (if (null? xs)
      1
      (* (car xs) (lprod (cdr xs)))))

(define (nrep n)
  (if (= n 1)
      '(1)
      (append (make-list n n) (nrep (- n 1)))))
      
(define (sumlist xs)
  (fold-right + 0 xs))

(define (all p xs) 
  (fold-right (lambda (x y) (and (p x) y)) #t xs))

;(define (higher-append xs ys)
;  (foldr (lambda (x y) ()) xs ys)


(define (concatenate lists)
  (fold-right append '() lists))

(define (try-all fs z)
  (map (lambda (p) (p z)) fs))

(define head car)
(define (tail stream) (force (cdr stream)))
(define ones (cons 1 (delay ones)))

(define (stream-section n stream)
  (if (= n 0)
      '()
      (cons (head stream) (stream-section (- n 1) (tail stream)))))

(define (integers-starting-from n)
  (cons n (delay (integers-starting-from (+ n 1)))))

(define Nats (integers-starting-from 0))

(define (every-other s)
  (cons (head s) (delay (every-other (tail (tail s))))))

(define Odds (every-other (integers-starting-from 1)))
(define Odds2 (every-other (tail Nats)))


(define (nfold f n x)
  (if (= n 0)
      x
      (f (nfold f (- n 1) x))))

(define (bend n)
  (if (= n 0)
      '()
      (cons n (bend (- n 1)))))

(define (mm xs ys)
  (if (null? xs)
      '()
      (append (map (lambda (y) (list (car xs) y)) ys) (mm (cdr xs) ys))))
