# Little Man Computer

This code implement a simulator for the Little Man Computer (LMC).

## Code structure

### Assembrer

The assembrer code is included in file [assembler.py](assembler.py).
```
class Assembler
    :param file_path
    :argument verbose = False
    
    @method compile

    @property success
    @property lmc_memory
```
Arguments:
- `file_path`: Path to assembly file
- `verbose`: Print messages for easy debug

Methods:
- `compile`: Compile assembly and returns memory for lmc

Properties:
- `success`: True is compilation complete successfully
- `lmc_memory`: Memory for lmc

Example of use:
```python
from assembler import Assembler

assembler = Assembler("tests/init.lmc")
assembler.compile()
print(assembler.lmc_memory)
```

### LMC

The LMC code is included in file [lmc.py](lmc.py).
```
class Lmc
    :param input_queue
    :param memory = None
    :param track_operations = False
    :argument verbose = False
    
    @method computer
    @method __iter__
    @method __next__

    @property memory
    @property program_counter
    @property accumulator
    @property flag
    @property input_queue
    @property output_queue
    @property log_operations
```
Arguments:
- `input_queue`: Input queue as list of int, where int are in range 0, 999
- `memory`: Memory for lmc
- `track_operations`: If true logs operation codes to property `log_operations`
- `verbose`: Print messages for easy debug

Methods:
- `compute`: Compute all code
- `__iter__`: Start lmc as iterator
- `__next__`: Do next compute step as iterator step

Properties:
- `memory`: Memory of lmc
- `program_counter`: Program counter
- `accumulator`: Accumulator registry
- `flag`: If true, last arithmetic operation produced overflow or underflow 
- `input_queue`: Input queue as list of int, where int are in range 0, 999
- `output_queue`: Output queue as list of int, where int are in range 0, 999
- `log_operations`: If `track_operations=True` has logs of operations code

Example of use:
```python
from assembler import Assembler
from lmc import Lmc

assembler = Assembler("tests/init.lmc")
memory = assembler.compile()

input_queue = [1, 2, 3]

lmc = Lmc(input_queue, memory = memory)
lmc.compute()
print(lmc.output_queue)
```

Example of use step-by-step:
```python
from assembler import Assembler
from lmc import Lmc

assembler = Assembler("tests/init.lmc")
memory = assembler.compile()

input_queue = [1, 2, 3]

lmc = Lmc(input_queue, memory = memory)
for _ in lmc:
    print(lmc.program_counter)
print(lmc.output_queue)
```

## Test codes
- [test_assembler.py](test_assembler.py) compile code of file passed as argument
- [test_lmc.py](test_lmc.py) execute code of file passed as argument