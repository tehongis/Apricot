
apricot		:	main.cpp
			g++ -Wall -o apricot main.cpp -lsfml-graphics -lsfml-window -lsfml-system

run			: apricot
			./apricot

clean		: apricot
			rm -rf apricot
