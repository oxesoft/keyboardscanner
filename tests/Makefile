test: compile run clean

compile:
	@gcc -o keyboardscanner -g -I . -x c++ *.cpp ../keyboardscanner.ino ../*.cpp
run:
	@./keyboardscanner
debug:
	@lldb ./keyboardscanner
clean:
	@rm -rf keyboardscanner keyboardscanner.dSYM
