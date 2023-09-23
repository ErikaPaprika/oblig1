import fileinput
import re

expr = re.compile(r'counter \[([0-9]*)\]')

last_num = 0

for line in fileinput.input():
    num = int(expr.findall(line)[0])
    if num < last_num:
        print(f"Last number was {last_num}")
        print(line)
    last_num = num
