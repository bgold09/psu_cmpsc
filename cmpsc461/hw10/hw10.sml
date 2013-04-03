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

