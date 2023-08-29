const Depth = 64;
class token {
	constructor(type, op, nmb) {
		this.type = type;
		this.op = String(op);
		this.nmb = nmb;
	}
}

class stack {
	Stack = [];
	Sptr;
	Depth;
	constructor(Stack, Sptr, Depth) {
		this.Stack = Stack;
		this.Sptr = Sptr;
		this.Depth = Depth;
	}
};

function push(Stack, tok)
{
	if (Stack.Sptr >= Stack.Depth - 1) return -1;
	Stack.Stack[Stack.Sptr] = tok;
	++Stack.Sptr;
	return 0;
}

function pop(Stack, tok)
{
	if (Stack.Sptr <= 0) return -1;
	--Stack.Sptr;
	tok = Stack.Stack[Stack.Sptr];
	return 0;
}

function PrintStack(Stack)
{
	for (count = 0; count < Stack.Sptr; ++count)
		if (Stack.Stack[count].type === 0) {
			console.log(Stack.Stack[count].op);
			console.log("\n");
		}
		else
			if (Stack.Stack[count].type == 1) {
				console.log(Stack.Stack[count].nmb);
				console.log("\n");
			}
		else {
			console.log("Неизвестный тип токена\n");
			return -1;
		}
	return 0;
}

function IsOp(c)
{
	return c === '+' || c === '-' || c === '*' || c === '/' || c === '+' || c === '^';
}

function Prior(c)
{
	switch (c) {
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

var tok = new token(0, "", 0);
var prev_tok = new token(0, "", 0);

var Stack = new stack([tok, tok, tok], 0, Depth);
var OutStack = new stack([tok, tok, tok], 0, Depth);

var InputStr = "";
var OutputStr = "";
function Evaluate()
{
	event.preventDefault();
	var count_out = 0;
	expr = document.getElementById("Expr").value;
	InputStr = String(expr);
	for (let count = 0; count < InputStr.length; ++count)
	{
		if (IsOp(InputStr[count])) {
			console.log(InputStr[count]);
			console.log("\n");
			prev_tok = tok;
			tok = new token(0, InputStr[count], 0.0);
			if (Stack.Sptr === 0) push(Stack, tok);
			else {
				tok_ = new token(0, "", 0.0);
				pop(Stack, tok_);
				if (Prior(tok_.op) < Prior(tok.op)) {
					push(Stack, tok_);
					push(Stack, tok);
				}
				else {
					let TmpStr = "";
					TmpStr[0] = tok_.op;
					TmpStr[1] = ' ';
					TmpStr[2] = 0;
					OutputStr += TmpStr.toString();
					push(OutStack, tok_);
					pop(Stack, tok_);
					while ((Prior(tok_.op) >= Prior(tok.op)) && (Stack.Sptr != 0)) {
						TmpStr[0] = tok_.op;
						TmpStr[1] = ' ';
						TmpStr[2] = 0;
						OutputStr += TmpStr.toString();
						push(OutStack, tok_);
						pop(Stack, tok_);
					}
					if (Prior(tok_.op) >= Prior(tok.op)) {
						TmpStr[0] = tok_.op;
						TmpStr[1] = ' ';
						TmpStr[2] = 0;
						OutputStr += TmpStr.toString();
						push(OutStack, tok_);
					}
					else push(Stack, tok_);
					push(Stack, tok);
				}
			}

		}
		else if (InputStr[count] === '(') {
			console.log(InputStr[count]);
			console.log("\n");
			prev_tok = tok;
			tok = new token(0, InputStr[count], 0.0 );
			push(Stack, tok);
		}
		else if (InputStr[count] === ')') {
			console.log(InputStr[count]);
			console.log("\n");
			while (tok.op != '(') {
				pop(Stack, tok);
				if (tok.op != '(') {
					let TmpStr = "";
					TmpStr[0] = tok.op;
					TmpStr[1] = ' ';
					TmpStr[2] = 0;
					OutputStr += TmpStr.toString();
					push(OutStack, tok);
				}
			}
		}
		else if (((InputStr[count] >= '0') && (InputStr[count] <= '9')) || (InputStr[count] == '.')) {
			let Tmpd = parseFloat(InputStr.substring(count));
			console.log(Tmpd);
			console.log("\n");
			var cnt = 0;
			for (cnt = count; cnt < InputStr.length; ++cnt) {
				let isE = false;
				let nsign = 0;
				if (((InputStr[cnt] === 'E') || (InputStr[cnt] === 'e')) && (!isE))
				{
					isE = true;
					continue;
				}
				if (((InputStr[cnt] == '+') || (InputStr[cnt] == '-')) && isE && (nsign < 1))
				{
					nsign = 1;
					continue;
				}
				if (((InputStr[count] >= '0') && (InputStr[count] <= '9')) || (InputStr[count] == '.'))
					continue;
				break;
			}
			count = cnt-1;
			prev_tok = tok;
			tok = new token(1, 0, Tmpd);
//			push(tok);
			let TmpStr = "";
			TmpStr = String(Tmpd).toString();
			OutputStr += TmpStr.toString();
			push(OutStack, tok);
		}
		else if (InputStr[count] === ' ') continue;
		else {
			console.log("Ошибка в формуле, позиция:")
			console.log(count);
			console.log("\n");
			break;
		}
	}
	while (Stack.Sptr != 0) {
		pop(Stack, tok);
		TmpStr = "";
		TmpStr[0] = tok.op;
		TmpStr[1] = ' ';
		TmpStr[2] = 0;
		OutputStr += TmpStr.toString();
		push(OutStack, tok);
	}

	document.getElementById("Output").innerHTML = OutputStr.toString();
//	alert(expr);
	return true;
}
