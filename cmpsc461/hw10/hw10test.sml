val mergeLess = merge (fn (x,y)=>x<=y);
mergeLess ([1,6,9,10], [2,4,20,40,60]);
val naturals = infiniteFrom 0;
firstN(10,naturals);
val someStrings = listToStream [ "a", "abc", "cde", "efgh", "hij", "lmno"];
firstN(4, someStrings);
filterCPS([1,2,3,4,5,6,7,8,9,0], fn n=>(n mod 2 = 0), fn k=>k);
filter([1,2,3,4,5,6,7,8,9,0], fn n=>(n mod 2 = 0));

val t0 = node(empty,2,empty,5,empty);
val t1 = node(empty,10,empty,12,empty);
val t2 = node(empty,15,empty,17,empty);
val t3 = node(t0,7,t1,14,t2);
val t4 = node(empty,20,empty,25,empty);
val t5 = node(empty,29,empty,31,empty);
val t6 = node(t3,19,t4,27,t5);
val t7 = node(t3,21,t4,27,t5);
val t8 = node(t3,21,t4,30,t5);

ordered t6;
ordered t7;
ordered t8;
