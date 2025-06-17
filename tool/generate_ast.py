import sys
import os
from typing import List
from io import TextIOWrapper

def main():
    if len(sys.argv) != 2:
        print("Usage: python3 generate_ast <output_dir>")
        exit(64)

    types : List[str] = [ 
            "Binary   : Expr* left, Token* op, Expr* right",
            "Grouping : Expr* expression",
            "Literal  : std::string_view value",
            "Unary    : Token* op, Expr* right"
        ]

    define_ast(sys.argv[1], "Expr", types)

def define_ast(output_dir : str, base_name : str, types : List[str]):
    path : str = os.path.join(os.getcwd(), output_dir, base_name + ".hpp")
    f : TextIOWrapper = open(path, "x")

    # Pragma
    f.write('#pragma once\n')

    # Includes
    f.write('#include "Token.hpp"\n')
    f.write('#include <string_view>\n')
    f.write('#include <variant>\n\n')

    # Forward declaration of Expressions
    for _type in types:
        f.write("class {};\n".format(_type.split(":")[0].strip()))

    f.write("\n")

    # Visitor class
    define_visitor(f, base_name, types)

    # Base class
    f.write("class {} {{\n".format(base_name))
    f.write("public:\n")
    f.write("\tvirtual ~Expr() = default;\n")
    f.write("\tvirtual std::string accept(Visitor& visitor) const = 0;\n")
    f.write("};\n\n")


    # AST classes
    for _type in types:
        class_name : str = _type.split(":", 1)[0].strip()
        fields : str = _type.split(":", 1)[1].strip()
        define_type(f, class_name, base_name, fields)

    f.close()

def define_type(f : TextIOWrapper, class_name : str, base_name : str, fields : str):
    fields_list : List[str] = fields.split(",")

    # Class - open
    f.write("class {} : public {} {{\n".format(class_name, base_name))

    # Public
    f.write("public:\n")

    # Constructor - open
    f.write("\t{}(".format(class_name))
    for field_num, field in enumerate(fields_list):
        field_type, field_name = field.strip().split(" ")
        f.write("{} {}".format(field_type, field_name))

        if field_num != len(fields_list) - 1:
            f.write(", ")


    f.write(") : ")

    # Contructor - init list
    for field_num, field in enumerate(fields_list):
        _, field_name = field.strip().split(" ")
        f.write("{}({})".format(field_name, field_name))

        if field_num != len(fields_list) - 1:
            f.write(", ")

    # Constructor - close
    f.write(" {}\n")

    # Accept method
    f.write("\tstd::string accept(Visitor& visitor) const override {{ return visitor.visit{}(*this); }}\n\n".format(class_name + base_name))

    # Private - TODO is this needed?
    # f.write("private:\n")

    # Variables
    for field in fields_list:
        field_type, field_name = field.strip().split(" ")
        f.write("\t{} {};\n".format(field_type, field_name))

    # Class - close
    f.write("};\n\n")

def define_visitor(f : TextIOWrapper, base_name : str, types : List[str]):
    f.write("class Visitor {\n")
    f.write("public:\n")

    for _tpye_num, _type in enumerate(types):
        type_name : str = _type.split(":")[0].strip()
        f.write("\tvirtual std::string visit{}(const {}& {}) = 0;".format(type_name + base_name, type_name, base_name.lower()))

        if _tpye_num != len(types) - 1:
            f.write("\n")

    f.write("\n};\n\n")

if __name__ == "__main__":
    main()