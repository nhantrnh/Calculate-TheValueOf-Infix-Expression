#include <iostream>
#include <fstream>
#include <string.h>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <stack>
using namespace std;

//MY PROJECT JUST CAN HANDLE AND CALCULATE AN EXPRESSION WITH THE OPERANDS WHICH HAVE 6-DIGIT (NOT INCLUDED " . " ) OR LESS.

string readFile(string file_name); // read file

void writeFile(string file_name, string content); // write file

int UT(string x); // determine the priority levels of operators	

int HT(string x); // determine value of operators and operands.

string calculateValue(string b, string x, string a); // calculate the value of the expression( with b is the 1st number, a is the 2nd number and x is the operators)

string check_num1_num2(stack<string>* St, stack<string>* Sh, string symbol); // check errors of num1 and num2

string check_round_bracket(stack<string>* St, stack<string>* Sh, string symbol); // check errors in " ( ) "

string check_expression(string expression); // check total errors in the expression

void calculate_num1_num2(stack<string>* St, stack<string>* Sh); //separate the operators and operands from the stack and add the calculated value according to the expression to the stack containing the number, to reuse

float calculateValue(string M); // calculate of the initially expression 

int main(){
	string data = readFile("infix.txt");
	cout << data << endl;
	if (data != ""){ 
		string valid = check_expression(data);
		if (valid == ""){
			stringstream ss;
			ss << setprecision(10) << fixed << calculateValue(data);  // the decimal part of the result
			string content = "1\n" + ss.str();
			writeFile("result.txt", content);
		}
		else{
			string content = "0\n" + valid;
			writeFile("result.txt", content);
		}
	}
	else{
		cout << "Cannot read file infix.txt" << endl;
		writeFile("result.txt", "Cannot read file infix.txt"); 
	}

	data.clear();
	return 0;
}

string readFile(string file_name){
	ifstream f;
	f.open(file_name);     // open to read file
	string data;
	getline(f, data);
	f.close();
	return data;
}

void writeFile(string file_name, string content){
	ofstream f;
	f.open(file_name);			// open to write
	f << content;
	f.close();
}

int UT(string x){
	if (x == "sqrt" || x == "^")
		return 3;
	if (x == "*" || x == "/" || x == "%" || x == "^" || x == "sqrt")
		return 2;
	else if (x == "+" || x == "-")
		return 1;
	else if (x == "(")
		return 0;

	return -1;
}

int HT(string x){
	if (x == "*" || x == "/" || x == "%" || x == "+" || x == "-" || x == "^" || x == "sqrt")
		return 2;
	else
		return 1;
}

string calculateValue(string b, string x, string a){
	float fResult = 0;
	stringstream ss1, ss2;
	float num1 = 0.0;
	float num2 = 0.0;
	ss1 << b;
	ss1 >> num1;
	ss2 << a;
	ss2 >> num2;
	if (x == "sqrt"){
		fResult = int(sqrt(num2));
	}
	if (x == "^"){
		fResult = pow(num1, num2);
	}
	if (x == "%"){
		fResult = int(num1) % int(num2);
	}
	if (x == "*"){
		fResult = num2 * num1;
	}
	else if (x == "/"){
		fResult = num1 / num2;
	}
	else if (x == "+"){
		fResult = num2 + num1;
	}
	else if (x == "-"){
		fResult = num1 - num2;
	}

	stringstream ss3;
	ss3 << fResult;
	return ss3.str();
}

string check_num1_num2(stack<string>* St, stack<string>* Sh, string symbol){
	while (!St->empty() && (UT(symbol) <= UT(St->top()))){				//Check for errors of numbers and signs when satisfying the condition that the sign you want to add has less precedence than the sign in the stack
		string x = St->top();
		St->pop();
		if (x == "("){
			string x = St->top();
			St->pop();
		}

		if ((x == "sqrt" && Sh->size() < 1) || (x != "sqrt" && Sh->size() < 2)){
			return "Doesn't have enough values";
		}

		stringstream ss1, ss2;
		float a_float = 0.0;
		string a_str = Sh->top();
		ss1 << a_str;
		ss1 >> a_float;
		ss2 << a_float;

		if (ss2.str() != a_str){
			return "Has non-number as a value";
		}

		Sh->pop();

		if (x != "sqrt"){
			stringstream ss3, ss4;
			float b_float = 0.0;;
			string b_str = Sh->top();
			ss3 << b_str;
			ss3 >> b_float;
			ss4 << b_float;

			if (ss4.str() != b_str){
				return "Has non-number as a value";
			}

			if (x == "/" && a_float == 0){
				return "Cannot divide 0";
			}

			stringstream ss5;
			ss5 << (int)b_float;
			stringstream ss6;
			ss6 << (int)a_float;
			if (x == "%" && ss6.str() != a_str){
				return "Cannot modulo non-integer";
			}
			else if (x == "%" && ss5.str() != b_str){
				return "Non-integer cannot be modulo";
			}

			if (x == "^" && ss6.str() != a_str && b_float < 0){
				return "Negative value to the power of non-integer";
			}
			else if (x == "^" && ss6.str() != a_str && b_float <= 0){
				return "0 to the power of non-integer";
			}
			else if (x == "^" && ss6.str() == a_str && a_float < 0 && (int)b_float == 0){
				return "0 to the power of negative integer";
			}
			Sh->pop();
			Sh->push(calculateValue(b_str, x, a_str));
		}
		else
		{
			if (a_float < 0){
				return "Cannot calculate square root of negative value";
			}
			Sh->push(calculateValue("", x, a_str));
		}
	}
	return "";
}

string check_round_bracket(stack<string>* St, stack<string>* Sh, string symbol){
	while (St->top() != "("){
		string x = St->top();
		St->pop();

		if ((x == "sqrt" && Sh->size() < 1) || (x != "sqrt" && Sh->size() < 2)){
			return "Doesn't have enough values";
		}

		stringstream ss1, ss2;
		float a_float;
		string a_str = Sh->top();
		ss1 << a_str;
		ss1 >> a_float;
		ss2 << a_float;

		if (ss2.str() != a_str){
			return "Has non-number as a value";
		}

		Sh->pop();

		if (x != "sqrt"){
			stringstream ss3, ss4;
			float b_float;
			string b_str = Sh->top();
			ss3 << b_str;
			ss3 >> b_float;
			ss4 << b_float;

			if (ss4.str() != b_str && b_float != (int)b_float){
				return "Has non-number as a value";
			}

			if (x == "/" && a_float == 0){
				return "Cannot divide 0";
			}

			stringstream ss5;
			ss5 << (int)b_float;
			stringstream ss6;
			ss6 << (int)a_float;
			if (x == "%" && ss6.str() != a_str && a_float != (int)a_float){
				return "Cannot modulo non-integer";
			}
			else if (x == "%" && ss5.str() != b_str && b_float != (int)b_float){
				return "Non-integer cannot be modulo";
			}

			if (x == "^" && ss6.str() != a_str && b_float < 0){
				return "Negative value to the power of non-integer";
			}
			else if (x == "^" && ss6.str() != a_str && b_float <= 0){
				return "0 to the power of non-integer";
			}
			else if (x == "^" && ss6.str() == a_str && a_float < 0 && (int)b_float == 0){
				return "0 to the power of negative integer";
			}
			Sh->pop();
			Sh->push(calculateValue(b_str, x, a_str));
		}
		else{
			if (a_float < 0){
				return "Cannot calculate square root of negative value";
			}
			Sh->push(calculateValue("", x, a_str));
		}
	}
	return "";
}

string check_expression(string expression){
	stack<string>* St = new stack<string>();
	stack<string>* Sh = new stack<string>();
	string top_symb;
	int start = 0;
	int end = expression.find(" ");
	string pre_symbol = "";

	while (end != -1){
		string symbol = expression.substr(start, end - start);
		start = end + 1;

		if (HT(symbol) == HT(pre_symbol) && symbol != "(" && symbol != ")" && pre_symbol != "(" && pre_symbol != ")" && pre_symbol != ""){
			delete St;
			delete Sh;
			return "This is not infix regression";
		}

		if (symbol == "("){
			St->push(symbol);
		}

		if (HT(symbol) == 1 && symbol != "(" && symbol != ")"){
			Sh->push(symbol);
		}

		if (HT(symbol) == 2){
			string bug = check_num1_num2(St, Sh, symbol);
			if (bug != ""){
				delete St;
				delete Sh;
				return bug;
			}
			St->push(symbol);
		}

		else if (symbol == ")"){
			if (St->empty()){
				delete St;
				delete Sh;
				return "Invalid round bracket";
			}

			string bug = check_round_bracket(St, Sh, symbol);
			if (bug != ""){
				delete St;
				delete Sh;
				return bug;
			}

			St->pop();
		}
		end = expression.find(" ", start);
		pre_symbol = symbol;
		symbol.clear();
	}

	string symbol = expression.substr(start, end - start);

	if (HT(symbol) == HT(pre_symbol) && symbol != "(" && symbol != ")" && pre_symbol != "(" && pre_symbol != ")" && pre_symbol != ""){
		delete St;
		delete Sh;
		return "This is not infix regression";
	}

	if (symbol == "("){
		St->push(symbol);
	}

	if (HT(symbol) == 1 && symbol != "(" && symbol != ")"){
		Sh->push(symbol);
	}

	if (HT(symbol) == 2){
		string bug = check_num1_num2(St, Sh, symbol);
		if (bug != ""){
			delete St;
			delete Sh;
			return bug;
		}
		St->push(symbol);
	}

	else if (symbol == ")"){
		if (St->empty()){
			delete St;
			delete Sh;
			return "Invalid round bracket";
		}

		string bug = check_round_bracket(St, Sh, symbol);
		if (bug != ""){
			delete St;
			delete Sh;
			return bug;
		}

		St->pop();
	}

	symbol.clear();

	while (!St->empty()){
		string x = St->top();
		St->pop();
		if (x == "(")
		{
			string x = St->top();
			St->pop();
		}

		if ((x == "sqrt" && Sh->size() < 1) || (x != "sqrt" && Sh->size() < 2))
		{
			delete St;
			delete Sh;
			return "Doesn't have enough values";
		}

		stringstream ss1, ss2;
		float a_float;
		string a_str = Sh->top();
		ss1 << a_str;
		ss1 >> a_float;
		ss2 << a_float;

		if (ss2.str() != a_str){
			delete St;
			delete Sh;
			return "Has non-number as a value";
		}

		Sh->pop();

		if (x != "sqrt"){
			stringstream ss3, ss4;
			float b_float;
			string b_str = Sh->top();
			ss3 << b_str;
			ss3 >> b_float;
			ss4 << b_float;

			if (ss4.str() != b_str){
				delete St;
				delete Sh;
				return "Has non-number as a value";
			}

			if (x == "/" && a_float == 0){
				delete St;
				delete Sh;
				return "Cannot divide 0";
			}

			stringstream ss5;
			ss5 << (int)b_float;
			stringstream ss6;
			ss6 << (int)a_float;
			if (x == "%" && ss6.str() != a_str){
				delete St;
				delete Sh;
				return "Cannot modulo non-integer";
			}
			else if (x == "%" && ss5.str() != b_str){
				delete St;
				delete Sh;
				return "Non-integer cannot be modulo";
			}

			if (x == "^" && ss6.str() != a_str && b_float < 0){
				delete St;
				delete Sh;
				return "Negative value to the power of non-integer";
			}
			else if (x == "^" && ss6.str() != a_str && b_float <= 0){
				delete St;
				delete Sh;
				return "0 to the power of non-integer";
			}
			else if (x == "^" && ss6.str() == a_str && a_float < 0 && (int)b_float == 0){
				delete St;
				delete Sh;
				return "0 to the power of negative integer";
			}
			Sh->pop();
			Sh->push(calculateValue(b_str, x, a_str));
		}
		else{
			if (a_float < 0){
				delete St;
				delete Sh;
				return "Cannot calculate square root of negative value";
			}
			Sh->push(calculateValue("", x, a_str));
		}
	}

	if (!Sh->empty()){
		stringstream ss1, ss2;
		float a_float;
		string a_str = Sh->top();
		ss1 << a_str;
		ss1 >> a_float;
		ss2 << a_float;

		if (ss2.str() != a_str){
			delete St;
			delete Sh;
			return "Has non-number as a value";
		}
	}

	delete St;
	delete Sh;
	return "";
}

void calculate_num1_num2(stack<string>* St, stack<string>* Sh){
	string a = "";
	a = Sh->top();
	Sh->pop();

	string x = "";
	x = St->top();
	St->pop();

	string b = "";
	if (x != "sqrt"){
		b = Sh->top();
		Sh->pop();
	}

	Sh->push(calculateValue(b, x, a));
}

float calculateValue(string M)
{
	float fResult = 0;
	int start = 0;
	int end = M.find(" ");

	stack<string>* Sh = new stack<string>();
	stack<string>* St = new stack<string>();

	while (end != -1){
		string value = M.substr(start, end - start);
		start = end + 1;
		if (HT(value) == 1 && value != "(" && value != ")")
			Sh->push(value);

		if (value == "(")
			St->push(value);

		if (HT(value) == 2){
			while (!St->empty() && (UT(value) <= UT(St->top()))){
				calculate_num1_num2(St, Sh);
			}
			St->push(value);
		}

		if (value == ")"){
			while (St->top() != "("){
				calculate_num1_num2(St, Sh);
			}
			string c = "";
			c = St->top();
			St->pop();
		}

		end = M.find(" ", start);
		value.clear();
	}

	string value = M.substr(start, end - start);
	if (HT(value) == 1 && value != "(" && value != ")")
		Sh->push(value);

	if (value == "(")
		St->push(value);

	if (HT(value) == 2){
		while (!St->empty() && (UT(value) <= UT(St->top()))){
			calculate_num1_num2(St, Sh);
		}
		St->push(value);
	}

	if (value == ")"){
		while (St->top() != "("){
			calculate_num1_num2(St, Sh);
		}
		string c = "";
		c = St->top();
		St->pop();
	}

	while (!St->empty()){
		calculate_num1_num2(St, Sh);
	}

	string strResult = "";
	strResult = Sh->top();
	Sh->pop();
	fResult = atof(strResult.c_str());
	delete St, Sh;
	strResult.clear();
	value.clear();
	return fResult;
}

