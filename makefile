
apricot.exe		:	main.cpp
			g++ -Wall -o apricot.exe main.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lbox2d

run			: apricot.exe
			./apricot.exe

clean		: apricot.exe
			rm -rf apricot.exe
