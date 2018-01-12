; define
(define sum
    (lambda(L)
        (cond
          ((eqv? L '()) 0)(else(+(car L)(sum (cdr L))))
        )
    )
)

; call
(sum '(1 2 3 4 5 6 7 8 9))
