/**
 * Brian Golden
 * CMPSC 461
 * Assignment 13
 * 4/27/13
 */

/**
 * combination- true when C is a combination of A and B, 
 *   i.e. C maintains the relative order of both A and B
 */
combination([], [], _).
combination([], B, B).
combination(A, [], A). 
combination(A, B, C) :- 
	append(A, B, Zs), same_length(C, Zs),
	hasOrder(A, C), hasOrder(B, C), 
	permutation(Zs, C).

hasOrder([], _).
hasOrder([X|Xs], [Y|Ys]) :- (X=Y, hasOrder(Xs, Ys)); hasOrder([X|Xs], Ys).

/**
 * nway-combinatio(L,S) - true when L is a list of lists and 
 *    S is combination of all those lists 
 */
nway-combination([], _).
nway-combination(L, S) :- 
	append(L, Zs), same_length(Zs, S),
	permutation(Zs, S),
	rnway(L, S).

rnway([],_).
rnway([X|Xs], S) :- hasOrder(X, S), rnway(Xs, S).

/* aux predicates */
upto(0,[]).
upto(N,[N|Xs]) :- N>0, M is N-1, upto(M,Xs).

nreverse([],[]).
nreverse([X|Xs], Zs) :- nreverse(Xs,Ys), append(Ys,[X],Zs).
