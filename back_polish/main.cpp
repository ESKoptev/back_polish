#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>
const int Depth = 64;
struct token {
	int type; //0 - операция, 1 - число
	char op; //'+', '-', '*', '/', '^' '(';
	double nmb;
} tok, prev_tok;
struct stack {
	token * Stack;
	int Sptr;
	int Depth;
} Stack, OutStack;
int push(stack * Stack, token tok)
{
	if (Stack->Sptr >= Stack->Depth - 1) return -1;
	Stack->Stack[Stack->Sptr] = tok;
	++Stack->Sptr;
	return 0;
}
int pop(stack * Stack, token * tok)
{
	if (Stack->Sptr <= 0) return -1;
	--Stack->Sptr;
	*tok = Stack->Stack[Stack->Sptr];
	return 0;
}
int PrintStack(stack Stack)
{
	for (int count = 0; count < Stack.Sptr; ++count)
		if (Stack.Stack[count].type == 0)
			printf("%c\n", Stack.Stack[count].op);
		else
			if (Stack.Stack[count].type == 1)
				printf("%f\n", Stack.Stack[count].nmb);
			else
			{
				printf("Неизвестный тип токена\n");
				return -1;
			}
	return 0;
}
bool IsOp(char c)
{
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '+' || c == '^';
}
int Prior(char c)
{
	switch (c)
	{
	case '(': return 0;
	case ')': return 1;
	case '+': return 2;
	case '-': return 2;
	case '*': return 3;
	case '/': return 3;
	case '^': return 4;
	default: return -255;
	}
}
char InputStr[256], OutputStr[256];
int main(int argc, char** argv)
{
	setlocale(LC_ALL, "ru-RU");
	setlocale(LC_NUMERIC, "en-US");
	Stack.Stack = (token*)malloc(Depth*sizeof(token));
	OutStack.Stack = (token*)malloc(Depth*sizeof(token));
	Stack.Depth = Depth;
	OutStack.Depth = Depth;
	Stack.Sptr = 0;
	OutStack.Sptr = 0;
	int count_out = 0;
	strcpy_s(InputStr, "3.14 + 0.4 * .2 / (1 - 5)^2"); //3.14 + 0.4 * .2 / (1 - 5)^2
	for (unsigned int count = 0; count < strlen(InputStr); ++count)
	{
		if (IsOp(InputStr[count]))
		{
			printf("%c\n", InputStr[count]);
			prev_tok = tok;
			tok = {0, InputStr[count], 0.0};
			if (Stack.Sptr == 0) push(&Stack, tok);
			else
			{
				token tok_ = { 0 };
				pop(&Stack, &tok_);
				if (Prior(tok_.op) < Prior(tok.op))
				{
					push(&Stack, tok_);
					push(&Stack, tok);
				}
				else
				{
					char TmpStr[256] = "";
					TmpStr[0] = tok_.op;
					TmpStr[1] = ' ';
					TmpStr[2] = 0;
					strcat_s(OutputStr, TmpStr);
					push(&OutStack, tok_);
					pop(&Stack, &tok_);
					while ((Prior(tok_.op)>=Prior(tok.op))&&(Stack.Sptr!=0))
					{
						TmpStr[0] = tok_.op;
						TmpStr[1] = ' ';
						TmpStr[2] = 0;
						strcat_s(OutputStr, TmpStr);
						push(&OutStack, tok_);
						pop(&Stack, &tok_);
					}
					if (Prior(tok_.op) >= Prior(tok.op))
					{
						TmpStr[0] = tok_.op;
						TmpStr[1] = ' ';
						TmpStr[2] = 0;
						strcat_s(OutputStr, TmpStr);
						push(&OutStack, tok_);
					}
					else push(&Stack, tok_);
					push(&Stack, tok);
				}
			}

		}
		else if (InputStr[count] == '(')
		{
			printf("%c\n", InputStr[count]);
			prev_tok = tok;
			tok = { 0, InputStr[count], 0.0 };
			push(&Stack, tok);
		}
		else if (InputStr[count] == ')')
		{
			printf("%c\n", InputStr[count]);
			while (tok.op != '(')
			{
				pop(&Stack, &tok);
				if (tok.op != '(')
				{
					char TmpStr[256] = "";
					TmpStr[0] = tok.op;
					TmpStr[1] = ' ';
					TmpStr[2] = 0;
					strcat_s(OutputStr, TmpStr);
					push(&OutStack, tok);
				}
			}
		}
		else if (((InputStr[count] >= '0') && (InputStr[count] <= '9')) || (InputStr[count] == '.'))
		{
			char *endptr;
			double Tmpd = strtod(&InputStr[count], &endptr);
			printf("%lf\n", Tmpd);
			count = endptr - InputStr - 1;
			prev_tok = tok;
			tok = {1, 0, Tmpd};
//			push(tok);
			char TmpStr[256] = "";
			sprintf_s(TmpStr, "%lf ", Tmpd);
			strcat_s(OutputStr, TmpStr);
			push(&OutStack, tok);
		}
		else if (InputStr[count] == ' ') continue;
		else
		{
			printf("Ошибка в формуле, позиция: %i\n", count);
			break;
		}
	}
	while (Stack.Sptr != 0)
	{
		pop(&Stack, &tok);
		char TmpStr[256] = "";
		TmpStr[0] = tok.op;
		TmpStr[1] = ' ';
		TmpStr[2] = 0;
		strcat_s(OutputStr, TmpStr);
		push(&OutStack, tok);
	}
	printf("Результат: %s\n", OutputStr);
	PrintStack(OutStack);
	for (int count = 0; count < OutStack.Sptr; ++count)
	{
		if (OutStack.Stack[count].type == 1) push(&Stack, OutStack.Stack[count]);
		else if (OutStack.Stack[count].type == 0)
		{
			token Nmb1, Nmb2;
			pop(&Stack, &Nmb2);
			pop(&Stack, &Nmb1);
			switch (OutStack.Stack[count].op)
			{
				case '+': Nmb1.nmb += Nmb2.nmb;
				break;
				case '-': Nmb1.nmb -= Nmb2.nmb;
				break;
				case '*': Nmb1.nmb *= Nmb2.nmb;
				break;
				case '/': Nmb1.nmb /= Nmb2.nmb;
				break;
				case '^': Nmb1.nmb = pow(Nmb1.nmb, Nmb2.nmb);
				break;
				default: printf("Неизвестная операция\n");
			}
			push(&Stack, Nmb1);
		}
	}
	token res;
	pop(&Stack, &res);
	printf("Результат: %f", res.nmb);
	free(Stack.Stack);
	free(OutStack.Stack);
}