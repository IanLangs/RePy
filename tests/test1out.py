from re import compile as reCompile
code = reCompile(r"\d+")
print(code.findall("32876 383 3274"))