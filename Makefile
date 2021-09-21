build:
	g++ -Wall -g -o poduri poduri.cpp
	g++ -Wall -g -o adrese adrese.cpp

run-p1:      
	./poduri

run-p2:      
	./adrese

clean:
	rm -f adrese poduri
