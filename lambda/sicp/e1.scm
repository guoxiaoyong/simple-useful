(/ (+ 5 4 (- 2 (- 3 (+ 6 (/ 4 5))))) (* 3 (- 6 2) (- 2 7)))

(define (sum_of_squares x y) (+ (* x x) (* y y)))

(define (get_big_two_then_calculate_sum_of_squares x y z)
  (cond
    ((and (<= x y) (<= x z)) (sum_of_squares y z))
    ((and (<= y z) (<= y x)) (sum_of_squares z x))
    ((and (<= z x) (<= z y)) (sum_of_squares x y))
    (else #f)
  )
)

(display (get_big_two_then_calculate_sum_of_squares 1 2 3))
(display (get_big_two_then_calculate_sum_of_squares 2 3 1))
(display (get_big_two_then_calculate_sum_of_squares 4 3 1))

(define (a-plus-abs-b a b) ((if (> b 0) + -) a b))
(display (a-plus-abs-b 10 -11))

(define (p) (p))
(define (test x y) (if (= x 0) 0 y))
;;;;(test 0 (p))

(define (fib n)
(cond ((= n 0) 0)
((= n 1) 1)
(else (+ (fib (- n 1))
(fib (- n 2))))))
(display (fib 20))
