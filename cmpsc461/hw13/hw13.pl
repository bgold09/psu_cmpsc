/**
 * Brian Golden
 * CMPSC 461
 * Assignment 13
 * 4/27/13
 */

/**
 * combination - true when C is a combination of A and B, 
 *   i.e. C maintains the relative order of both A and B
 */
combination([], [], _).
combination([], B, B).
combination(A, [], A). 
combination(A, B, C) :- 
	append(A, B, Zs), permutation(Zs, C),
	hasOrder(A, C), hasOrder(B, C).

hasOrder([], _).
hasOrder([X|Xs], [Y|Ys]) :- (X=Y, hasOrder(Xs, Ys)); hasOrder([X|Xs], Ys).

/**
 * nway-combinatio(L,S) - true when L is a list of lists and 
 *    S is combination of all those lists 
 */
nway-combination([], _).
nway-combination(L, S) :- append(L, Zs), permutation(Zs, S), rnway(L, S).

rnway([],_).
rnway([X|Xs], S) :- hasOrder(X, S), rnway(Xs, S).

/**
 * perfectPermutation(N,L) - true when N is any positive number and 
 *   L is a perfect permutation of the list [1,2,3,...,N], 
 *   i.e. no elements of the list are in the same position
 */
perfectPermutation(N, L) :-
	N>0, upto(N, Xs), permutation(Xs, L), 
	reverse(Xs, Ys), test(Ys, L).

test([], _).
test([X|Xs], [Y|Ys]) :- not(X=Y), test(Xs, Ys).

upto(0,[]).
upto(N,[N|Xs]) :- N>0, M is N-1, upto(M,Xs).
