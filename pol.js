function Evaluate()
{
	event.preventDefault();
	expr = document.getElementById("Expr").value;
	document.getElementById("Output").innerHTML = expr;
//	alert(expr);
	return true;
}
