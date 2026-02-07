#include <bits/stdc++.h>

enum Type { VAR, NOT, AND, OR, XOR, TO, IFF } type;

struct Node {
	Type type;
	int val;
	Node *l, *r;
	Node(Type t, int v = 0, Node *a = nullptr, Node *b = nullptr): type(t), val(v), l(a), r(b) {}
};
