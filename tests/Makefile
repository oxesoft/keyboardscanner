test: compile run clean

compile:
	@gcc -o keyboardscanner -g -D TEST_MODE -x c++ *.cpp ../keyboardscanner.ino ../*.cpp
run:
	@./keyboardscanner
debug:
	@lldb ./keyboardscanner
clean:
	@rm -rf keyboardscanner keyboardscanner.dSYM
