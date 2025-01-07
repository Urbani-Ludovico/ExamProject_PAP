from sys import argv

from assembler import Assembler
from lmc import Lmc


try:
    argv[1]
except IndexError:
    print("Missing file path")
    exit(1)

ass = Assembler(argv[1], verbose = True)
ass.compile()

lmc = Lmc([1, 2, 3], memory = ass.lmc_memory, track_operations = True, verbose = True)
lmc.compute()
print("Operations:", lmc.log_operations)
print("Output:", lmc.output_queue)
