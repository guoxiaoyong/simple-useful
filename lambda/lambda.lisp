(define true (lambda (x y) x))
(define false (lambda (x y) y))
(define if_then_else (lambda (condition_expr true_clause false_clause) (condition_expr true_clause false_clause)))
(define succ (lambda (n) (lambda (f x) (f (n f x)))))
(define add (lambda (m n) (m succ n)))
(define n0 false)
(define n1 (succ n0))
(define n2 (succ n1))
(define n3 (succ n2))
(define n4 (succ n3))
(define n5 (succ n4))
(define f (lambda (x) (+ x 1)))
(display (n0 f 0))
(display (n4 f 0))
(define (to_int n) (n f 0)
(display (to_int n0))
