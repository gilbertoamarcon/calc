#include <iostream>
#include <string>
#include <stack>
#include <cstdlib>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdexcept>
#include <cmath>

using namespace std;

void display(stack<double> raw_stk, string num, int dec_plcs, int help);

int main(){

	// Buffers
	stack<double> stk;
	stack<double> stk_bkp;
	string num = string("");
	double b = 0;
	double a = 0;
	char c = 0;
	int dec_plcs = 3;
	int help = 0;

	// Main loop
	for(;;){

		// Print stack
		display(stk,num,dec_plcs,help);

		// Input
		system("/bin/stty raw");
		c = getchar();
		system("/bin/stty cooked");

		// Stack restore
		if(c == 'R' || c == 'r'){
			stk = stk_bkp;
			continue;
		}

		// Stack backup
		stk_bkp = stk;

		// Quit
		if(c == 3){
			system("tput reset");
			return 0;
		}

		// Help
		if(c == 'H' || c == 'h'){
			help = !help;
			continue;
		}

		// Decimal places
		if(c == 'P' || c == 'p'){
			if(num.size()!=0){
				dec_plcs = stoi(num);
				num = string("");
			}
			continue;
		}

		// If BACKSPACE, delete
		if(c == 127){
			if(num.size()!=0)
				num.pop_back();
			else
			if(stk.size() >= 1)
				stk.pop();
			continue;
		}

		// If ENTER, stack or duplicate
		if(c == 13){
			if(num.size()!=0){
				stk.push(stod(num));
				num = string("");
			}else
				stk.push(stk.top());
			continue;
		}

		// If ESC, clear input
		if(c == 27){
			if(num.size()!=0)
				num = string("");
			continue;
		}

		// If numerical, concat
		if((c >= '0' && c <= '9') || c == '.'){
			num = num+string(1,c);			
			continue;
		}

		// If operand, operate
		if(stk.size() >= 1 && num.size()!=0){
			b = stod(num);
			a = stk.top(); stk.pop();
		}else 
		if(stk.size() >= 2){
			b = stk.top(); stk.pop();
			a = stk.top(); stk.pop();
		}else
			continue;

		// Operations
		num = string("");
		switch(c){
			case '+':{
				double r = a+b;
				stk.push(r);
				break;
			}
			case '-':{
				double r = a-b;
				stk.push(r);
				break;
			}
			case '*':{
				double r = a*b;
				stk.push(r);
				break;
			}
			case '/':{
				double r = a/b;
				stk.push(r);
				break;
			}
			case '^':{
				double r = pow(a,b);
				stk.push(r);
				break;
			}
			case 32:{
				stk.push(b);
				stk.push(a);
				break;
			}
		}

	}

	return 0;
}

void display(stack<double> raw_stk, string num, int dec_plcs, int help){

	// Ordering stack 
	stack<double> stk;
	while(!raw_stk.empty()){
		stk.push(raw_stk.top());
		raw_stk.pop();
	}

	// Getting terminal dimensions
	struct winsize size;
	ioctl(STDOUT_FILENO,TIOCGWINSZ,&size);
	int width = size.ws_col-3;
	int cases = size.ws_col-3;
	
	// Cleargin screen
	system("tput reset");
	if(help)
		printf("| Restore: R | Clear input: ESC | Swap: SPACE  | Dec plcs: P  | \n");

	// Bar
	for(int i = 0; i < size.ws_col; i++)
		printf("T");

	for(int i = 0; i < size.ws_row-stk.size()-2; i++)
		printf("\n");

	// Printing stack
	int i = stk.size(); 
	while(!stk.empty()){
		printf("%d: %*.*f\n",i--,width,dec_plcs,stk.top());
		stk.pop();
	}

	// Printing current input
	int c = num.size();
	while(--c >= 0)
		if(num.at(c) == '.') break;
	if(num.size()!=0){
		double in_num = stod(num);
		if(c == -1)	
			printf("%d: %*.0f",0,width,in_num);
		else
		if(c == num.size()-1)
			printf("%d: %*.*f.",0,width-1,num.size()-c-1,in_num);
		else
			printf("%d: %*.*f",0,width,num.size()-c-1,in_num);
	}else{
		printf("%d:",0,0);
		for(int i = 0; i < width; i++)
			printf(" ");
	}

}