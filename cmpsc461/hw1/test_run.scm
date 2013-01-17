(binom 7 3)

;Value: 35

(binom 12 4)

;Value: 495

(binom 1 0)

;Value: 1

(binom 0 0)

;Value: 1

(binom 2 2)

;Value: 1

(approx-pi 10)

;Value: 2.9045183262483176

(approx-pi 100)

;Value: 3.1204170317790467

(approx-pi 1000)

;Value: 3.1395554669110286

(approx-pi 10000)

;Value: 3.141391477611317

(sinTerms 3 0.1)      

;Value: 4

(sinTerms 2 0.001)

;Value: 5

(sinTerms 10 0.00001)

;Value: 17

(sinTerms (approx-pi 10000) 0.0001)

;Value: 7

(sinTerms 22 0.00005)

;Value: 33

(myRand 11 (expt 2 31) 1103515245 12345 42)

;Value: 391441865

(myRand 12 (expt 2 31) 1103515245 12345 42)

;Value: 1108520142

(myRand 13 (expt 2 31) 1103515245 12345 42)

;Value: 1206814703

(myrand 0 10 2 0 1337)

;Value: 1337

(myrand 1 10 2 0 1337) 

;Value: 4

(quality? 1103515245 12345 (expt 2 31))

;Value: #t

(quality? 16807 0 (- (expt 2 31) 1))

;Value: #t

(quality? 180 20 (expt 2 31))

;Value: #f

(quality? 180 20 100)

;Value: #f

(quality? 13 7 101)

;Value: #t

(quality? 13 7 1024)

;Value: #t
