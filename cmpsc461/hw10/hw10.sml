(* Brian Golden
   CMPSC 461
   Assignment 10
   04/06/13
 *)

(* 1. merge - merges two ordered lists according to a predicate *)
fun merge p = 
  let
    fun m(nil, ys) = ys
       |m(xs, nil) = xs
       |m(x::xs, y::ys) = 
      if p(x,y)
      then x :: m(xs, y::ys)
      else y :: m(x::xs, ys)
  in m
  end;

(* 2. streams *)
datatype 'a stream = void | element of 'a * (unit -> 'a stream);

fun infiniteFrom n = element(n, fn()=>infiniteFrom(n+1));

fun firstN (n, void) = nil
   |firstN (0, str) = nil
   |firstN (n, element(x, str)) = x :: firstN(n-1, str());

(*fun listToStream nil = void
   |listToStream (x::xs) = element(x, listToStream(xs));*)

fun cons(x, str) = element(x, fn()=>str);

fun listToStream xs = List.foldr cons void xs;
