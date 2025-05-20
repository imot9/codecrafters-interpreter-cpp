#include "AstPrinter.hpp"

#include <sstream>
#include <iomanip>
#include <cmath>

std::string AstPrinter::print(const Expr& expr) {
    return expr.accept(*this);
}

std::string AstPrinter::visitBinaryExpr(const Binary& expr)
{
    return this->parenthesize(expr.op->lexeme, expr.left, expr.right);
}

std::string AstPrinter::visitGroupingExpr(const Grouping& expr)
{
    return this->parenthesize("group", expr.expression);
}

std::string AstPrinter::visitLiteralExpr(const Literal& expr)
{
    if (expr.value.empty()) {
        return "nil";
    }
    
    try {
        float num = std::stof(std::string(expr.value));
        std::stringstream ss;
        if (num == std::floor(num)) {
            ss << std::fixed << std::setprecision(1) << num;
        } else {
            ss << expr.value;
        }
        return ss.str();
    } catch (const std::invalid_argument&) {
        return "\"" + std::string(expr.value) + "\"";
    }
}

std::string AstPrinter::visitUnaryExpr(const Unary& expr)
{
    return this->parenthesize(expr.op->lexeme, expr.right);
}

template<typename... Exprs>
std::string AstPrinter::parenthesize(std::string_view name, Exprs&... exprs)
{
    std::stringstream ss;

    ss << "(" << name;
    ((ss << " " << exprs->accept(*this)), ...);
    ss << ")";

    return ss.str();
}
