import sys
import os
from typing import List
from io import TextIOWrapper

TYPE_LUT = {
    "!!variant_string_view-float" : "std::variant<std::string_view, float>"
}

def main():
    if len(sys.argv) != 2:
        print("Usage: python3 generate_ast <output_dir>")
        exit(64)

    types : List[str] = [ 
            "Binary   : Expr& left, Token& op, Expr& right",
            "Grouping : Expr& expression",
            "Literal  : !!variant_string_view-float value", # std::variant<std::string_view, float> kind of messes up the splitting, will subst. later
            "Unary    : Token& op, Expr& right"
        ]

    define_ast(sys.argv[1], "Expr", types)

def define_ast(output_dir : str, base_name : str, types : List[str]):
    path : str = os.path.join(os.getcwd(), output_dir, base_name + ".hpp")
    f : TextIOWrapper = open(path, "x")

    # Includes
    f.write('#include "Token.hpp"\n')
    f.write('#include <string_view>\n')
    f.write('#include <variant>\n\n')

    # Base class
    f.write("class {} {{}};\n\n".format(base_name))

    # AST classes
    for _type in types:
        class_name : str = _type.split(":")[0].strip()
        fields : str = _type.split(":")[1].strip()
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

        # Types containing spaces would break the split, so substitute them here
        if field_type.startswith("!!"):
            field_type : str = TYPE_LUT[field_type]
            field_type.strip("!!")

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
    f.write(" {}\n\n")

    # Private
    f.write("private:\n")

    # Variables
    for field in fields_list:
        field_type, field_name = field.strip().split(" ")

        if field_type.startswith("!!"):
            field_type : str = TYPE_LUT[field_type]
            field_type.strip("!!")

        f.write("\t{} {};\n".format(field_type, field_name))

    # Class - close
    f.write("};\n\n")

if __name__ == "__main__":
    main()