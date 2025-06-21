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
    std::ostringstream ss;

    std::visit([&ss](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, float>) {
            ss << arg;
        } else if constexpr (std::is_same_v<T, std::string_view>) {
            if (arg.empty()) {
                ss << "nil";
            } else {
                ss << std::string(arg);
            }
        }
    }, expr.value);

    return ss.str();
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
