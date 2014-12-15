__author__ = 'knelson'

import ast
import operator as op

'''
Notes:
Code taken from stack overflow answer located here:
http://stackoverflow.com/a/9558001


'''

operators = {ast.Add: op.add, ast.Sub: op.sub, ast.Mult: op.mul,
             ast.Div: op.truediv, ast.Pow: op.pow, ast.USub: op.neg}

def eval_expr(expr):
    """Safely evaluate and return the result of a math expression

    Example:
    print eval_expr('2**2')
    >> 4
    """
    return eval_node(ast.parse(expr, mode='eval').body)


def eval_node(node):
    """Recursively parse math expression using AST"""

    if isinstance(node, ast.Num):        # <number>
        return node.n
    elif isinstance(node, ast.BinOp):    # <left> <operator> <right>
        return operators[type(node.op)](eval_node(node.left), eval_node(node.right))
    elif isinstance(node, ast.UnaryOp):  # <operator> <operand> e.g., -1
        return operators[type(node.op)](eval_node(node.operand))
    else:
        raise TypeError(node)