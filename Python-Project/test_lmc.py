from sys import argv

from assembler import Assembler
from lmc import Lmc


try:
    argv[1]
except IndexError:
    print("Missing file path")
    exit(1)

ass = Assembler(argv[1], verbose = True)

lmc = Lmc(memory = ass.lmc_memory, verbose = True)