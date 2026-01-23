#include <bits/stdc++.h>

struct Expr {
    char op;
    char var;
    Expr* left;
    Expr* right;
    Expr(char o=0, char v=0, Expr* l=nullptr, Expr* r=nullptr): op(o), var(v), left(l), right(r) {}
};

struct Rule {
    Expr* expr;
    char result;
    Rule(Expr* e=nullptr, char r=0): expr(e), result(r) {}
};

static std::map<char, bool> initialFacts;
static std::map<char, int> stateMemo;
static std::vector<Rule> rules;
static std::map<char, std::vector<int>> producers;

static int precedence(char op) {
    if (op == '!') return 4;
    if (op == '+') return 3;
    if (op == '^') return 2;
    if (op == '|') return 1;
    return 0;
}

static bool isOp(char c) {
    return c=='!'||c=='+'||c=='|'||c=='^';
}

static std::vector<std::string> tokenize(const std::string& s) {
    std::vector<std::string> t;
    for (size_t i=0;i<s.size();) {
        char c = s[i];
        if (c==' '||c=='\t') { ++i; continue; }
        if (c>='A' && c<='Z') { std::string r; r.push_back(c); t.push_back(r); ++i; continue; }
        if (isOp(c) || c=='(' || c==')') { std::string r; r.push_back(c); t.push_back(r); ++i; continue; }
        ++i;
    }
    return t;
}

static Expr* buildAstFromRpn(const std::vector<std::string>& rpn) {
    std::vector<Expr*> st;
    for (auto &tk : rpn) {
        char c = tk[0];
        if (tk.size()==1 && c>='A' && c<='Z') {
            st.push_back(new Expr(0,c,nullptr,nullptr));
        } else if (tk.size()==1 && isOp(c)) {
            if (c=='!') {
                Expr* a = nullptr;
                if (!st.empty()) { a = st.back(); st.pop_back(); }
                st.push_back(new Expr('!',0,a,nullptr));
            } else {
                Expr* b = nullptr; Expr* a = nullptr;
                if (!st.empty()) { b = st.back(); st.pop_back(); }
                if (!st.empty()) { a = st.back(); st.pop_back(); }
                st.push_back(new Expr(c,0,a,b));
            }
        }
    }
    if (st.empty()) return nullptr;
    return st.back();
}

static std::vector<std::string> shuntingYard(const std::vector<std::string>& tokens) {
    std::vector<std::string> out;
    std::vector<char> ops;
    for (size_t i=0;i<tokens.size();++i) {
        std::string tk = tokens[i];
        char c = tk[0];
        if (tk.size()==1 && c>='A' && c<='Z') {
            out.push_back(tk);
        } else if (tk.size()==1 && isOp(c)) {
            if (c=='!') {
                ops.push_back(c);
            } else {
                while (!ops.empty() && isOp(ops.back()) && (precedence(ops.back()) >= precedence(c))) {
                    std::string o; o.push_back(ops.back()); out.push_back(o); ops.pop_back();
                }
                ops.push_back(c);
            }
        } else if (c=='(') {
            ops.push_back(c);
        } else if (c==')') {
            while (!ops.empty() && ops.back() != '(') {
                std::string o; o.push_back(ops.back()); out.push_back(o); ops.pop_back();
            }
            if (!ops.empty() && ops.back() == '(') ops.pop_back();
        }
    }
    while (!ops.empty()) {
        std::string o; o.push_back(ops.back()); out.push_back(o); ops.pop_back();
    }
    return out;
}

static bool evalExpr(Expr* e);

static bool solveChar(char v) {
    if (initialFacts.count(v) && initialFacts[v]) { stateMemo[v] = 2; return true; }
    int st = 0;
    if (stateMemo.count(v)) st = stateMemo[v];
    if (st == 2) return true;
    if (st == 3) return false;
    if (st == 1) return false;
    stateMemo[v] = 1;
    bool result = false;
    auto it = producers.find(v);
    if (it != producers.end()) {
        for (int idx : it->second) {
            Rule* r = &rules[idx];
            if (r->expr == nullptr) continue;
            if (evalExpr(r->expr)) { result = true; break; }
        }
    }
    stateMemo[v] = result ? 2 : 3;
    return result;
}

static bool evalExpr(Expr* e) {
    if (e == nullptr) return false;
    if (e->op == 0) return solveChar(e->var);
    if (e->op == '!') return !evalExpr(e->left);
    if (e->op == '+') return evalExpr(e->left) && evalExpr(e->right);
    if (e->op == '|') return evalExpr(e->left) || evalExpr(e->right);
    if (e->op == '^') {
        bool a = evalExpr(e->left);
        bool b = evalExpr(e->right);
        return (a && !b) || (!a && b);
    }
    return false;
}

static std::string trim(const std::string& s) {
    size_t a = 0; while (a < s.size() && isspace((unsigned char)s[a])) ++a;
    size_t b = s.size(); while (b> a && isspace((unsigned char)s[b-1])) --b;
    return s.substr(a, b-a);
}

static void addRuleFromLine(const std::string& line) {
    std::string s = line;
    size_t pos = s.find("<=>");
    bool equiv = false;
    if (pos != std::string::npos) {
        equiv = true;
    } else {
        pos = s.find("=>");
    }
    if (pos == std::string::npos) return;
    std::string left = trim(s.substr(0,pos));
    std::string right = trim(s.substr(pos + (equiv?3:2)));
    auto parseSide = [&](const std::string& side)->Expr* {
        std::vector<std::string> tokens = tokenize(side);
        std::vector<std::string> rpn = shuntingYard(tokens);
        return buildAstFromRpn(rpn);
    };
    Expr* leftAst = parseSide(left);
    for (size_t i=0;i<right.size();++i) {
        char c = right[i];
        if (c >= 'A' && c <= 'Z') {
            rules.emplace_back(leftAst, c);
            producers[c].push_back((int)rules.size()-1);
        }
    }
    if (equiv) {
        Expr* rightAst = parseSide(right);
        for (size_t i=0;i<left.size();++i) {
            char c = left[i];
            if (c >= 'A' && c <= 'Z') {
                rules.emplace_back(rightAst, c);
                producers[c].push_back((int)rules.size()-1);
            }
        }
    }
}

int main(int argc, char** argv) {
    std::istream* inptr = &std::cin;
    std::ifstream fin;
    if (argc >= 2) {
        fin.open(argv[1]);
        if (fin) inptr = &fin;
    }
    std::vector<char> queries;
    std::string line;
    while (std::getline(*inptr, line)) {
        std::string s = trim(line);
        if (s.empty()) continue;
        if (s[0] == '#') continue;
        if (s.find("=>") != std::string::npos || s.find("<=>") != std::string::npos) {
            addRuleFromLine(s);
            continue;
        }
        if (s[0] == '=') {
            for (size_t i=1;i<s.size();++i) {
                char c = s[i];
                if (c>='A' && c<='Z') initialFacts[c] = true;
            }
            continue;
        }
        if (s[0] == '?') {
            for (size_t i=1;i<s.size();++i) {
                char c = s[i];
                if (c>='A' && c<='Z') queries.push_back(c);
            }
            continue;
        }
    }
    for (char q : queries) {
        stateMemo.clear();
        bool res = solveChar(q);
        if (res) {
            std::cout << q << " is true\n";
        } else {
            std::cout << q << " is false\n";
        }
    }
    return 0;
}

