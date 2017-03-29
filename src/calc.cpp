#include <iostream>
#include <string>
#include <stack>
#include <cstdlib>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdexcept>
#include <cmath>

// Counting decimal places
int get_dec_plcs(std::string in_buf);

// Printing GUI
void display(std::stack<double> raw_stack, std::string in_buf, int dec_plcs);

int main(){

	// Disabling terminal echo
	system("/bin/stty -echo");

	// Buffers
	std::stack<double> stack;
	std::stack<double> stack_bkp;
	std::string in_buf = std::string("");
	double b = 0;
	double a = 0;
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
		if(c == 3){
			system("tput reset");
			return 0;
		}

		// Stack restore
		if(c == 'R' || c == 'r'){
			stack = stack_bkp;
			continue;
		}

		// Stack backup
		stack_bkp = stack;

		// Change decimal places
		if(c == 'P' || c == 'p'){
			if(in_buf.size()!=0){
				dec_plcs = stoi(in_buf);
				in_buf = std::string("");
			}
			continue;
		}

		// If BACKSPACE
		if(c == 127){

			// Delete from input buffer
			if(in_buf.size()!=0) in_buf.pop_back();
			else

			// Delete from stack
			if(stack.size() >= 1) stack.pop();

			continue;
		}

		// If ENTER, stack or duplicate
		if(c == 13){

			// Stack input buffer
			if(in_buf.size()!=0){
				stack.push(stod(in_buf));
				in_buf = std::string("");
			}else

			// Duplicate top tack entry
			stack.push(stack.top());

			continue;
		}

		// If ESC clear input buffer
		if(c == 27){
			in_buf = std::string("");
			continue;
		}

		// If numerical concatenate input buffer
		if((c >= '0' && c <= '9') || (c == '.' && get_dec_plcs(in_buf) == -1)){
			in_buf += std::string(1,c);
			continue;
		}

		// If invalid operand, continue
		if(c != '+' && c != '-' && c != '*' && c != '/' && c != '^' && c != 'L' && c != 'l' && c != 32)		
			continue;

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
			case '+':
				stack.push(a+b); continue;
			case '-':
				stack.push(a-b); continue;
			case '*':
				stack.push(a*b); continue;
			case '/':
				stack.push(a/b); continue;
			case '^':
				stack.push(pow(a,b)); continue;
			case 'L': case 'l':
				stack.push(log(a)/log(b)); continue;
			case 32: // SPACE: swap
				stack.push(b);
				stack.push(a);
				continue;
		}

		// Pushing operands back to stack
		stack.push(a);
		stack.push(b);

	}

	return 0;
}

// Counting decimal places
int get_dec_plcs(std::string in_buf){
	int c = in_buf.size();
	while(--c >= 0)
		if(in_buf.at(c) == '.') break;
	return c;
}

void display(std::stack<double> raw_stack, std::string in_buf, int dec_plcs){

	// Ordering stack 
	std::stack<double> stack;
	while(!raw_stack.empty()){
		stack.push(raw_stack.top());
		raw_stack.pop();
	}

	// Getting terminal dimensions
	struct winsize size;
	ioctl(STDOUT_FILENO,TIOCGWINSZ,&size);
	int width = size.ws_col-3;
	
	// Cleargin screen
	system("tput reset");

	// Scrolling to the bottom
	for(int i = 0; i < size.ws_row-stack.size()-1; i++)
		printf("\n");

	// Printing stack
	int i = stack.size(); 
	while(!stack.empty()){
		printf("%d: %*.*f\n",i--,width,dec_plcs,stack.top());
		stack.pop();
	}

	// Decimal places
	int c = get_dec_plcs(in_buf);

	// If input buffer empty
	if(in_buf.size() == 0){
		printf("%d:",0);
		for(int i = 0; i < width; i++)
			printf(" ");
	}

	// If input buffer not empty
	else{

		// Numerical input from buffer
		double num_in = stod(in_buf);

		// No decimal point
		if(c == -1)	
		printf("%d: %*.0f",0,width,num_in);
		else

		// Decimal point, places=0
		if(c == in_buf.size()-1)
		printf("%d: %*.*f.",0,width-1,in_buf.size()-c-1,num_in);
		else

		// Decimal point, places>=0
		printf("%d: %*.*f",0,width,in_buf.size()-c-1,num_in);
	}

}