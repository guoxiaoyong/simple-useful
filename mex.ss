(begin
  (define map
    (lambda (f ls)
      (if (null? ls)
          '()
          (cons (f (car ls))
            (map f (cdr ls))))))
  (let ((ls '(1 2 3 4)))
    (map (lambda (x) (cons x ls)) ls)))

(begin
  (define sum
    (lambda (n)
      (if (= n 0)
        0
        (+ n (sum (- n 1))))))
  (sum 1000000))

(begin
  (define sum
    (lambda (n acc)
      (if (= n 0)
        acc
        (sum (- n 1) (+ n acc)))))
  (sum 100000 0))

(begin
  (define n)
  (define acc)
  (define sum
    (lambda ()
      (if (= n 0)
        acc
        (begin
          (set! acc (+ n acc))
          (set! n (- n 1))
          (sum)))))
  (begin
    (set! acc 0)
    (set! n 1000000)
    (sum)))

(begin
  (define n)
  (define acc)
  (define sum
    (lambda ()
      (if (= n 0)
        acc
        (begin
          (set! acc (+ n acc))
          (set! n (- n 1))
          (lambda () (sum))))))
  (begin
    (set! acc 0)
    (set! n 1000000)
    (lambda () (sum))))

(begin
  (define n)
  (define acc)
  (define sum
    (lambda ()
      (if (= n 0)
        acc
        (begin
          (set! acc (+ n acc))
          (set! n (- n 1))
          sum))))
  (begin
    (set! acc 0)
    (set! n 1000000)
    sum))

(define-syntax while
  (syntax-rules (do)
    ((while exp stmts ...)
     (let loop ()
       (if exp (begin stmts ... (loop)))))))

(begin
  (define n)
  (define acc)
  (define sum
    (lambda ()
      (if (= n 0)
        #f
        (begin
          (set! acc (+ n acc))
          (set! n (- n 1))
          sum))))
  (define run
    (lambda ()
      (begin
        (while (sum) 'no-op)
        acc)))
  (begin
    (set! acc 0)
    (set! n 1000000)
    (run)))

(begin
  (define n)
  (define acc)
  (define action)
  (define sum
    (lambda ()
      (if (= n 0)
        (set! action #f)
        (begin
          (set! acc (+ n acc))
          (set! n (- n 1))
          (set! action sum)))))
  (define run
    (lambda ()
      (begin
        (while action (action))
        acc)))
  (begin
    (set! acc 0)
    (set! n 1000000)
    (set! action sum)
    (run)))

(begin
  (define id (lambda (acc) acc))
  (define sum
    (lambda (n cont)
      (cont
        (if (= n 0)
          0
          (+ n (sum (- n 1) id))))))
  (sum 1000000 id))

(begin
  (define id (lambda (acc) acc))
  (define sum
    (lambda (n cont)
      (if (= n 0)
        (cont 0)
        (cont (+ n (sum (- n 1) id))))))
  (sum 1000000 id))

(begin
  (define id (lambda (acc) acc))
  (define sum
    (lambda (n cont)
      (if (= n 0)
        (cont 0)
        (sum (- n 1)
          (lambda (acc)
            (cont (+ n acc)))))))
  (sum 1000000 id))

(begin
  (define id (lambda (acc) acc))
  (define sum
    (lambda (n cont)
      (if (= n 0)
        (cont 0)
        (sum (- n 1)
          (let ((n n) (cont cont))
            (lambda (acc)
              (cont (+ n acc))))))))
  (sum 1000000 id))

(begin
  (define id (lambda (acc) acc))
  (define sum
    (lambda (n cont)
      (if (= n 0)
        (apply-cont cont 0)
        (sum (- n 1) (cons n cont)))))
  (define apply-cont
    (lambda (cont acc)
      (if (null? cont)
        acc
        (apply-cont
          (cdr cont)
          (+ (car cont) acc)))))
  (sum 1000000 '()))

(begin
  (define action)
  (define id '())
  (define n)
  (define cont)
  (define acc)
  (define sum
    (lambda ()
      (if (= n 0)
        (begin
          (set! cont cont)
          (set! acc 0)
          (set! action apply-cont))
        (begin
          (set! cont (cons n cont))
          (set! n (- n 1))
          (set! action sum)))))
  (define apply-cont
    (lambda ()
      (if (null? cont)
        (set! action #f)
        (begin
          (set! acc (+ (car cont) acc))
          (set! cont (cdr cont))
          (set! action apply-cont)))))
  (define run
    (lambda ()
      (begin
        (while action (action))
        acc)))
  (begin
    (set! cont id)
    (set! n 1000000)
    (set! action sum)
    (run)))

(begin
  (define action)
  (define id
    (lambda ()
      (set! action #f)))
  (define n)
  (define cont)
  (define acc)
  (define sum
    (lambda ()
      (if (= n 0)
        (begin
          (set! acc 0)
          (set! action cont))
        (begin
          (set! cont
            (let ((n n) (cont cont))
              (lambda ()
                (begin
                  (set! acc (+ n acc))
                  (set! action cont)))))
          (set! n (- n 1))
          (set! action sum)))))
  (define run
    (lambda ()
      (begin
        (while action (action))
        acc)))
  (set! cont id)
  (set! n 1000000)
  (set! action sum)
  (run))
