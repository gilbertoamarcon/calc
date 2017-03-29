#include <iostream>
#include <string>
#include <stack>
#include <cstdlib>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdexcept>
#include <cmath>

// Keys
#define CTRL_C		3
#define ENTER		13
#define ESC			27
#define SPACE		32
#define BACKSPACE	127

// Counting decimal places
int get_dec_plcs(std::string in_buf, char k);
int get_dec_plcs(std::string in_buf, char k, char l);

// Printing GUI
void display(std::stack<double> raw_stack, std::string in_buf, int dec_plcs);

int main(){

	// Disabling terminal echo
	system("/bin/stty -echo");

	// Buffers
	std::stack<double> stack;
	std::stack<double> stack_bkp;
	std::string in_buf = std::string("");
	char c = 0;
	int dec_plcs = 3;

	// Main loop
	for(;;){

		// Print GUI
		display(stack,in_buf,dec_plcs);

		// Input
		system("/bin/stty raw");
		c = getchar();
		system("/bin/stty cooked");

		// Quit
		if(c == CTRL_C){
			system("tput reset");
			return 0;
		}

		// Stack restore
		if(c == 'R' || c == 'r')
			stack = stack_bkp;

		// Stack backup
		stack_bkp = stack;

		// Change decimal places
		if(c == 'P' || c == 'p'){
			if(in_buf.size()!=0){
				dec_plcs = stoi(in_buf);
				in_buf = std::string("");
			}
		}

		// If BACKSPACE
		if(c == BACKSPACE){

			// Delete from input buffer
			if(in_buf.size()!=0) in_buf.pop_back();
			else

			// Delete from stack
			if(stack.size() >= 1) stack.pop();

		}

		// If ESC clear input buffer
		if(c == ESC)
			in_buf = std::string("");

		// If ENTER
		if(c == ENTER){

			// Stack input buffer
			if(in_buf.size()!=0){
				stack.push(stod(in_buf));
				in_buf = std::string("");
			}else

			// Duplicate top stack entry
			stack.push(stack.top());

		}

		// If numerical concatenate input buffer
		if((c >= '0' && c <= '9') || (c == '.' && get_dec_plcs(in_buf,'.') == -1) || ((c == 'e' || c == 'E') && get_dec_plcs(in_buf,'e','E') == -1)){
			if(c == '.' && in_buf.size() == 0) in_buf += std::string(1,'0');
			if((c == 'e' || c == 'E') && in_buf.size() == 0) in_buf += std::string(1,'1');
			in_buf += std::string(1,c);
		}

		// If valid operand
		if(c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == 'L' || c == 'l' || c == SPACE){

			double a = 0;
			double b = 0;

			// If buffer not empty and >=1 stack entry
			if(in_buf.size()!=0 && stack.size() >= 1){
				b = stod(in_buf);
				a = stack.top(); stack.pop();
			}else 

			// If >=2 stack entries
			if(stack.size() >= 2){
				b = stack.top(); stack.pop();
				a = stack.top(); stack.pop();
			}else

			// Not enough operands
			continue;

			// Operations
			in_buf = std::string("");
			switch(c){
				case '+':			stack.push(a+b);			break;
				case '-':			stack.push(a-b);			break;
				case '*':			stack.push(a*b);			break;
				case '/':			stack.push(a/b);			break;
				case '^':			stack.push(pow(a,b));		break;
				case 'L': case 'l':	stack.push(log(a)/log(b));	break;
				case SPACE:
					stack.push(b);
					stack.push(a);
				 	break;
			}

		}

	}

	return 0;
}

// Counting decimal places
int get_dec_plcs(std::string in_buf, char k){
	return get_dec_plcs(in_buf,k,k);
}

// Counting decimal places
int get_dec_plcs(std::string in_buf, char k, char l){
	int c = in_buf.size();
	while(--c >= 0)
		if(in_buf.at(c) == k || in_buf.at(c) == l) break;
	return c;
}

void display(std::stack<double> raw_stack, std::string in_buf, int dec_plcs){

	// Ordering stack 
	std::stack<double> stack;
	while(!raw_stack.empty()){
		stack.push(raw_stack.top());
		raw_stack.pop();
	}
	
	// Cleargin screen
	system("tput reset");

	// Getting terminal dimensions
	struct winsize size;
	ioctl(STDOUT_FILENO,TIOCGWINSZ,&size);

	// Number of stack entries to print
	int num_stack_print = (size.ws_row-2 < stack.size())?size.ws_row-2:stack.size();

	// Number of empty rows
	int empty_rows = size.ws_row-1-num_stack_print;

	// Left-zero padding for stack indexes
	int stack_index = (stack.size()==0)?1:1+log((double)(num_stack_print))/log(10);

	int width = size.ws_col-2-stack_index;

	// Scrolling to the bottom
	for(int i = 0; i < empty_rows; i++)
		printf("\n");

	// Printing stack
	int i = stack.size(); 
	while(!stack.empty()){
		if(i < size.ws_row)
			printf("%*d: %*.*f\n",stack_index,i,width,dec_plcs,stack.top());
		stack.pop();
		i--;
	}

	// Input buffer
	printf("%*d: %*s",stack_index,0,width,in_buf.c_str());

}