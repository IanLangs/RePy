"""
docstring
"""
from re import compile as reCompile

c = reCompile(r"\w+")
code = c.sub("w", "hello world")