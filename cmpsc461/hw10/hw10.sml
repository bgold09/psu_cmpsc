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

fun listToStream nil = void
   |listToStream (x::xs) = element(x, fn()=>listToStream(xs));

(* 3. filter - filter a list based on a predicate *)

fun filterCPS(nil, p, K) = K nil
   |filterCPS(x::xs, p, K) =  
      filterCPS(xs, p, fn rest=> K(if p(x) then x::rest else rest));

fun filter (nil, p) = nil
   |filter (ys, p) = filterCPS(ys, p, fn m=>m);

(* 4. ternary tree *)
datatype 'a ternaryTree = empty 
         | node of ('a ternaryTree * 'a * 'a ternaryTree * 'a * 'a ternaryTree);

fun exOp NONE = 0
   |exOp (SOME(x)) = x;

(* 
 * ordered - a tree is ordered if left, center and right are ordererd, all
 * integers in left are less than or equal to x, all integers in center are
 * greater than or equal to x and less than or equal to y, and all integers to
 * in right are less than or equal to y 
 *)
fun ordered empty = true
   |ordered (node(left, x, center, y, right)) = 
    let 
      fun inrange(p, empty) = true
         |inrange(p, node(left, x, center, y, right)) = 
            p(x) andalso p(y) andalso 
            inrange(fn(n)=> exOp(Int.minInt) <= n andalso n <= x, left) andalso
            inrange(fn(n)=> x <= n andalso n <= y, center) andalso
            inrange(fn(n)=> y <= n andalso n <= exOp(Int.maxInt), right)
    in 
      inrange(fn(n)=> exOp(Int.minInt) <= n andalso n <= x, left) andalso
      inrange(fn(n)=> x <= n andalso n <= y, center) andalso
      inrange(fn(n)=> y <= n andalso n <= exOp(Int.maxInt), right)
    end;
