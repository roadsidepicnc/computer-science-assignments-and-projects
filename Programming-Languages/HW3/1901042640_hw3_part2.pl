%            PART 2

% knowledge base

flight(canakkale, erzincan, 6).
flight(erzincan, canakkale, 6).

flight(erzincan, antalya, 3).
flight(antalya, erzincan, 3).

flight(izmir, antalya, 2).
flight(antalya, izmir, 2).

flight(diyarbakir, antalya, 4).
flight(antalya, diyarbakir, 4).

flight(izmir, istanbul, 2).
flight(istanbul, izmir, 2).

flight(rize, istanbul, 4).
flight(istanbul, rize, 4).

flight(izmir, ankara, 6).
flight(ankara, izmir, 6).

flight(diyarbakir, ankara, 8).
flight(ankara, diyarbakir, 8).

flight(rize, ankara, 5).
flight(ankara, rize, 5).

flight(ankara, istanbul, 1).
flight(istanbul, ankara, 1).

flight(ankara, van, 4).
flight(van, ankara, 4).

flight(gaziantep, van, 3).
flight(van, gaziantep, 3).

% rules

route(X, Y, C) :- flight(X, Y, C).										% Checks a flight exists between given cities and cost.
route(X, Y, C) :- route_new(X, Y, C, []).								% Finds the alternative paths and their costs.
route_new(X, Y, C, _) :- flight(X, Y, C).								% Finds the direct route
route_new(X, Z, C, Visited) :- (\+ member(X, Visited), 					% Checks if the city is not a member of the Visited list 	
                               X \== Z, 								% Cheks if the target city is not found yet		
                               flight(X, Y, C1), 						% Checks if there is a flight between X and Y 			
                               route_new(Y, Z, C2, [X | Visited]), 		% Finds the alternative. Also adds X to the Visited list for avoiding conflicts. 
                               C is C1 + C2).							% Adds the cost