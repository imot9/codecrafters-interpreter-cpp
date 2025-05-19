#include "Expr.hpp"

class AstPrinter : public Visitor {
public:
    std::string print(const Expr& expr);

private:
    std::string visitBinaryExpr(const Binary& expr) override;
    std::string visitGroupingExpr(const Grouping& expr) override;
    std::string visitLiteralExpr(const Literal& expr) override;
    std::string visitUnaryExpr(const Unary& expr) override;

    template<typename... Exprs>
    std::string parenthesize(std::string_view name, Exprs&... exprs);
};