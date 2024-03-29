(* Brian Golden
   CMPSC 461
   Assignment 9
   03/30/13
 *)

 (* 1. fix the type erros in the following three functions  *)
fun abc (0, x::xs) = xs
   |abc (k, x::xs) = abc(k-1, xs)
   |abc (k, nil) = nil;

fun def nil = 0::nil
   |def ((x, y)::xs) = (x + y) :: def(xs);

fun ghi (nil, ys) = nil
   |ghi (xs, nil) = nil
   |ghi (x::xs, y::ys) = (x,y) :: ghi(xs, ys);

(* 2. semi : int -> real, computes 1/1 - 1/2 + 1/3 - 1/4 + ... + 1/n *)
fun semi 1 = 1.0
   |semi n = if n mod 2 = 0 
             then ~1.0 / real(n) + semi(n-1)
             else  1.0 / real(n) + semi(n-1);

(* 3. remove : ('a * 'a list) -> ('a list * int) 
      removes all occurrences of a given value from a list and returns the 
      resulting list and number of occurrences removed 
*) 
fun remove (x, nil) = (nil, 0)
   |remove (x, y::ys) = 
     let val (newList, count) = remove(x, ys)
     in 
       if x = y 
       then (newList, count+1)
       else (y::newList, count)
     end;

(* 4. coprime : (int * int) -> bool
      determine if two arguments have no common divisors besides 1, 
      which is equivalen to saying that their greatest common divisor (GCD)
      is equal to 1. Compute the GCD using Euclid's algorithm. 
*)
fun coprime (x, y) = 
  let fun gcd (a, 0) = a 
         |gcd (a, b) = gcd(b, a mod b)
  in gcd(x, y) = 1
  end;

(* 5. binaryRepresentation : int -> (int list)
      takes a non-negative integer and returns its binary representation 
*)
fun binaryRepresentation n = 
  let 
    local 
      fun rev(nil, ys) = ys | rev(x::xs, ys) = rev(xs, x::ys)
    in
      fun treverse xs = rev(xs, nil)
    end;
    fun build 0 = 0::nil
       |build 1 = 1::nil
       |build m = (m mod 2) :: build(m div 2)
  in treverse (build n)
  end;
