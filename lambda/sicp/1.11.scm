(define (f n) 
  (if (< n 3) 
    n
    (+
      (f (- n 1))
      (* 2 (f (- n 2)))
      (* 3 (f (- n 3)))
    )
  )
)

;(f 30)

(define (f n)
  (if (< n 3)
    n
    (f-iter 2 1 0 3 n)
  )
)

(define (f-iter a b c counter max-count)
  (if (> counter max-count)
     a
     (f-iter (+ a (* 2 b) (* 3 c)) a b (+ counter 1) max-count)
  )
)

(f 30)
