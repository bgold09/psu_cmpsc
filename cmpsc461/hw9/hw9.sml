(* Brian Golden
   CMPSC 461
   Assignment 9
 *)

 (* 1. fix the type erros in the following three functions  *)
fun abc (0, x::xs) = xs
   |abc (k, x::xs) = abc(k-1, xs)
   |abc (k, nil) = ~1::nil;

fun def nil = 0::nil
   |def ((x, y)::xs) = (x + y) :: def(xs);

fun ghi (nil, ys) = nil
   |ghi (xs, nil) = nil
   |ghi (x::xs, y::ys) = (x,y) :: ghi(xs, ys);

(* 2. semi : int -> real, computes 1/1 - 1/2 + 1/3 - 1/4 + ... + 1/n *)
fun semi 1 = 1.0
   |semi n = if n mod 2 = 0 then ~1.0 / real(n) + semi(n-1)
             else 1.0 / real(n) + semi(n-1);

(* 3. remove : ('a * 'a list) -> ('a list * int) 
      removes all occurrences of a given value from a list and returns the 
      resulting list and number of occurrences removed 
*) 


(* 4. coprime : (int * int) -> bool
      determine if two arguments have no common divisors besides 1 
*)



(* 5. binaryRepresentation : int -> (int list)
      takes a non-negative integer and returns its binary representation 
*)


