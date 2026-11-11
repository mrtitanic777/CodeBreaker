/* (the kind of operand the instruction op takes: 0eh takes 4) */
f00057(op)
char op;
{
    return op == 0xe ? 4 : op;
}
