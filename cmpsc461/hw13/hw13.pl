/**
 * Brian Golden
 * CMPSC 461
 * Assignment 13
 * 4/27/13
 */

/**
 * combination - check if a list is a combination of two lists,
 *   i.e. the relative order elements of the two lists is preserved
 */

combination([], [], _).
combination([], Ys, Ys).
combination(Xs, [], Xs). 
combination(Xs, Ys, Zs) :- 
	append(Xs,Ys,Cs), same_length(Zs, Cs),
	hasOrder(Xs, Zs), hasOrder(Ys, Zs), 
	permute(Cs, Zs).

hasOrder([], _).
hasOrder([X|Xs], [Y|Ys]) :- (X=Y, hasOrder(Xs, Ys)); hasOrder([X|Xs], Ys).

/* aux predicates */
permute([],[]).
permute([X|Xs], Zs) :- permute(Xs,Ys), insert(X,Ys,Zs).

insert(X, Xs, [X|Xs]).
insert(X, [Y|Ys], [Y|Zs]) :- insert(X, Ys, Zs).

upto(0,[]).
upto(N,[N|Xs]) :- N>0, M is N-1, upto(M,Xs).

nreverse([],[]).
nreverse([X|Xs], Zs) :- nreverse(Xs,Ys), append(Ys,[X],Zs).
