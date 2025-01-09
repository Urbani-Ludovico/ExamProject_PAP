from sys import argv

from assembler import Assembler


try:
    argv[1]
except IndexError:
    print("Missing file path")
    exit(1)

ass = Assembler(argv[1], verbose = True)
ass.compile()
print(ass.lmc_memory)
