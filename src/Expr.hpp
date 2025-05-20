#include "Token.hpp"
#include <string_view>
#include <variant>

class Binary;
class Grouping;
class Literal;
class Unary;

class Visitor {
public:
	virtual std::string visitBinaryExpr(const Binary& expr) = 0;
	virtual std::string visitGroupingExpr(const Grouping& expr) = 0;
	virtual std::string visitLiteralExpr(const Literal& expr) = 0;
	virtual std::string visitUnaryExpr(const Unary& expr) = 0;
};

class Expr {
public:
	virtual ~Expr() = default;
	virtual std::string accept(Visitor& visitor) const = 0;
};

class Binary : public Expr {
public:
	Binary(Expr* left, Token* op, Expr* right) : left(left), op(op), right(right) {}
	std::string accept(Visitor& visitor) const override { return visitor.visitBinaryExpr(*this); }

	Expr* left;
	Token* op;
	Expr* right;
};

class Grouping : public Expr {
public:
	Grouping(Expr* expression) : expression(expression) {}
	std::string accept(Visitor& visitor) const override { return visitor.visitGroupingExpr(*this); }

	Expr* expression;
};

class Literal : public Expr {
public:
	Literal(std::string_view value) : value(value) {}
	std::string accept(Visitor& visitor) const override { return visitor.visitLiteralExpr(*this); }

	std::string_view value;
};

class Unary : public Expr {
public:
	Unary(Token* op, Expr* right) : op(op), right(right) {}
	std::string accept(Visitor& visitor) const override { return visitor.visitUnaryExpr(*this); }

	Token* op;
	Expr* right;
};

