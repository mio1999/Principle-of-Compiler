#define _CRT_SECURE_NO_WARNINGS

#include<iostream>
#include<fstream>
#include<stdlib.h>
using namespace std;

const char* keyword[17] = { "main","if","else","switch","case","define","include","int","doule","double","char","for","while","scanf","printf","cin","cout"};/*关键字*/
const char symbol[15]= {'+', '-', '*', '/', '<' , '>','=', ';', '(', ')', '{', '}', '#','!' ,'"',};  /*18~32~38*/
/*标识符数字  17*/
/*数字种别码 39*/
char ch;
FILE* fp;
char token[255];
int p_token=0;

struct Token {
	int type;
	char* str;
};

bool JudgeNumber() {
	if (ch >= '0' && ch <= '9')
		return true;
	else
		return false;
}

bool JudgeWord() {
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return true;
	else
		return false;
}

int JudgeSymbol() {
	int i = 0;
	while (i != 15) {
		if (ch == symbol[i]) {
			if (ch == '/' || ch == '=' || ch == '>' || ch == '<' || ch =='!')
				return 2;
			else {
				char cha[2] = {ch,'\0'};
				Token* mytoken = new Token;
				mytoken->type = i + 18;
				mytoken->str = cha;
				cout << "( " << mytoken->type << " , " << mytoken->str << " )" << endl;
				delete mytoken;
				return 1;/*无任何其他因素不需往下继续读一个字符确定类型*/
			}
		}
		i++;
	}
	return 0;
}

bool JudgeKeyword(char* str) {
	int i = 0;
	while (i != 13) {
		if (strcmp(str,keyword[i])==0) {
			Token* mytoken = new Token;
			mytoken->type = i;
			mytoken->str = token;
			cout << "( " << mytoken->type << " , " << mytoken->str << " )" << endl;
			delete mytoken;
			return 0;
		}
		i++;
	}
	Token* mytoken = new Token;
	mytoken->type = 17;
	mytoken->str = token;
	cout << "( " << mytoken->type << " , " << mytoken->str << " )" << endl;
	delete mytoken;
	return 0;
}




int contact() {
	token[p_token] = ch;
	p_token++;
	token[p_token] = '\0';
	return 0;
}

int main() {
	bool order=true;
	fp = fopen("d:/test.txt","r");
	ch = fgetc(fp);
	while (order) {
		p_token = 0;
		if (ch == ' ' || ch == 10) {
			ch = fgetc(fp);
		}

		if (JudgeWord()) {
			while (JudgeWord() || JudgeNumber()|| ch == '_') {
				contact();
				ch = fgetc(fp);
			}
			if (ch != -1)
				order = true;
			else
				order = false;
			JudgeKeyword(token);
		}
		else if (JudgeNumber()) {
			while (JudgeNumber()) {
				contact();
				ch = fgetc(fp);
			}
			if (ch != -1)
				order = true;
			else
				order = false;
			Token* mytoken = new Token;
			mytoken->type = 36;
			mytoken->str = token;
			cout << "( " << mytoken->type << " , " << mytoken->str << " )" << endl;
			delete mytoken;
		}
   		else if (JudgeSymbol()==1) {
			ch = fgetc(fp);
			if (ch != -1)
				order = true;
			else
				order = false;
		}
  		else if (JudgeSymbol() == 2) {
			if (ch == '>') {
				contact();
				ch = fgetc(fp);
				if (ch == '>') {
					contact();
					Token* mytoken = new Token;
					mytoken->type = 35;
					mytoken->str = token;
					cout << "( " << mytoken->type << " , " << mytoken->str << " )" << endl;
					delete mytoken;
					ch = fgetc(fp);
				}
				else if (ch == '=') {
					contact();
					Token* mytoken = new Token;
					mytoken->type = 36;
					mytoken->str = token;
					cout << "( " << mytoken->type << " , " << mytoken->str << " )" << endl;
					delete mytoken;
					ch = fgetc(fp);
				}
				else if (ch != -1 && ch != '>' && ch != '=' ) {
					order = true;
					Token* mytoken = new Token;
					mytoken->type = 23;
					mytoken->str = token;
					cout << "( " << mytoken->type << " , " << mytoken->str << " )" << endl;
					delete mytoken;
				}
				else {
					order = false;
					Token* mytoken = new Token;
					mytoken->type = 23;
					mytoken->str = token;
					cout << "( " << mytoken->type << " , " << mytoken->str << " )" << endl;
					delete mytoken;
				}
				if (ch != -1)
					order = true;
				else
					order = false;
			}
			else if (ch == '<') {
				contact();
				ch = fgetc(fp);
				if (ch == '<') {
					contact();
					Token* mytoken = new Token;
					mytoken->type = 33;
					mytoken->str = token;
					cout << "( " << mytoken->type << " , " << mytoken->str << " )" << endl;
					delete mytoken;
					ch = fgetc(fp);
				}
				else if (ch == '=') {
					contact();
					Token* mytoken = new Token;
					mytoken->type = 34;
					mytoken->str = token;
					cout << "( " << mytoken->type << " , " << mytoken->str << " )" << endl;
					delete mytoken;
					ch = fgetc(fp);
				}
				else if (ch != -1 && ch != '<' && ch != '>') {
					order = true;
					Token* mytoken = new Token;
					mytoken->type = 22;
					mytoken->str = token;
					cout << "( " << mytoken->type << " , " << mytoken->str << " )" << endl;
					delete mytoken;
				}
				else {
					order = false;
					Token* mytoken = new Token;
					mytoken->type = 22;
					mytoken->str = token;
					cout << "( " << mytoken->type << " , " << mytoken->str << " )" << endl;
					delete mytoken;
				}
				if (ch != -1)
					order = true;
				else
					order = false;
			}
			else if (ch == '=') {
				contact();
				ch = fgetc(fp);
				if (ch == '=') {
					contact();
					Token* mytoken = new Token;
					mytoken->type = 37;
					mytoken->str = token;
					cout << "( " << mytoken->type << " , " << mytoken->str << " )" << endl;
					delete mytoken;
					ch = fgetc(fp);
				}
				else if (ch != -1 && ch != '=') {
					order = true;
					Token* mytoken = new Token;
					mytoken->type = 24;
					mytoken->str = token;
					cout << "( " << mytoken->type << " , " << mytoken->str << " )" << endl;
					delete mytoken;
				}
				else {
					order = false;
					Token* mytoken = new Token;
					mytoken->type = 24;
					mytoken->str = token;
					cout << "( " << mytoken->type << " , " << mytoken->str << " )" << endl;
					delete mytoken;
				}
				if (ch != -1)
					order = true;
				else
					order = false;
			}
			else if (ch == '/') {
				contact();
				ch = fgetc(fp);
				if (ch == '*') {
					while (ch != '/' && ch != -1) {
						ch = fgetc(fp);
					}
					ch = fgetc(fp);
				}
				else {
					Token* mytoken = new Token;
					mytoken->type = 21;
					mytoken->str = token;
					cout << "( " << mytoken->type << " , " << mytoken->str << " )" << endl;
					delete mytoken;
				}
				if (ch != -1)
					order = true;
				else
					order = false;
			}
			else if (ch == '!') {
				contact();
				ch = fgetc(fp);
				if (ch == '=') {
					contact();
					Token* mytoken = new Token;
					mytoken->type = 38;
					mytoken->str = token;
					cout << "( " << mytoken->type << " , " << mytoken->str << " )" << endl;
					delete mytoken;
					ch = fgetc(fp);
				}
				else if (ch != -1 && ch != '=') {
					order = true;
					Token* mytoken = new Token;
					mytoken->type = 31;
					mytoken->str = token;
					cout << "( " << mytoken->type << " , " << mytoken->str << " )" << endl;
					delete mytoken;
				}
				else {
					order = false;
					Token* mytoken = new Token;
					mytoken->type = 31;
					mytoken->str = token;
					cout << "( " << mytoken->type << " , " << mytoken->str << " )" << endl;
					delete mytoken;
				}
				if (ch != -1)
					order = true;
				else
					order = false;
			}
		}
	}
	return 0;
}
