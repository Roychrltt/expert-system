#include "../include/expertSystem.hpp"

bool isOp(char c)
{
	return c == '!' || c == '+' || c == '|' || c == '^';
}

int precedence(char op)
{
	if (op == '!') return 4;
	if (op == '+') return 3;
	if (op == '^') return 2;
	if (op == '|') return 1;
	return 0;
}

std::vector<char> tokenize(const std::string& s)
{
	std::vector<char> t;
	for (size_t i = 0; i < s.size(); i++)
	{
		char c = s[i];
		if (c == ' ' || c == '\t' ) continue;
		if ((c >= 'A' && c <= 'Z') || (isOp(c) || c == '(' || c == ')')) t.push_back(c);
	}
	return t;
}

std::vector<char> shuntingYard(const std::vector<char>& tokens)
{
	std::vector<char> out;
	std::vector<char> ops;
	for (char c : tokens)
	{
		if (c >= 'A' && c <= 'Z') out.push_back(c);
		else if (isOp(c))
		{
			if (c == '!')
				ops.push_back(c);
			else
			{
				while (!ops.empty() && isOp(ops.back()) && (precedence(ops.back()) >= precedence(c)))
				{
					out.push_back(ops.back());
					ops.pop_back();
				}
				ops.push_back(c);
			}
		}
		else if (c == '(') ops.push_back(c);
		else if (c == ')')
		{
			while (!ops.empty() && ops.back() != '(')
			{
				out.push_back(ops.back());
				ops.pop_back();
			}
			if (!ops.empty() && ops.back() == '(') ops.pop_back();
		}
	}
	while (!ops.empty())
	{
		out.push_back(ops.back());
		ops.pop_back();
	}
	return out;
}

Expr* buildAstFromRpn(const std::vector<char>& rpn)
{
	std::vector<Expr*> st;
	for (char c : rpn)
	{
		if (c >= 'A' && c <= 'Z')
			st.push_back(new Expr(0, c, nullptr, nullptr));
		else if (isOp(c))
		{
			if (c == '!')
			{
				Expr* a = nullptr;
				if (!st.empty())
				{
					a = st.back();
					st.pop_back();
				}
				st.push_back(new Expr('!', 0, a, nullptr));
			}
			else
			{
				Expr* b = nullptr; Expr* a = nullptr;
				if (!st.empty())
				{
					b = st.back();
					st.pop_back();
				}
				if (!st.empty())
				{
					a = st.back();
					st.pop_back();
				}
				st.push_back(new Expr(c,0,a,b));
			}
		}
	}
	if (st.empty()) return nullptr;
	return st.back();
}
