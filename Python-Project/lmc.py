def operation_logger(func):
    """
    Logger for __iter__ function. Logs only if the variable _track_operations is true.
    """
    def wrapper(self):
        if self._track_operations:
            self._log_operations.append(self._memory[self._program_counter])
        func(self)

    return wrapper


class Lmc(object):
    def __init__(self, input_queue: list[int], memory: list[int] = None, track_operations: bool = False, verbose: bool = False):
        """
        Init LMC
        :param input_queue: Queue of input values
        :param memory: LMC memory (from assembler). If None it creates an empty memory
        :param track_operations: Keep track of operations in the variable log_operations
        :param verbose: Print messages for easy debug
        """
        self._verbose = verbose
        if self._verbose:
            print("- - - LMC - - -")

        self._track_operations = track_operations
        if self._track_operations:
            self._log_operations = []

        if self._verbose:
            print("Creating environment...")
        self._program_counter = 0
        self._accumulator = 0
        self._flag = False
        self._input_queue = input_queue
        self._output_queue = []

        if self._verbose:
            print("Creating memory...")
        if memory is not None:
            # Check type and size of memory
            if not isinstance(memory, list) or len(memory) != 100:
                raise TypeError("Memory must be a list[100]")

            # Check if memory is an array of int in range(1000)
            if any(map(lambda c: not (isinstance(c, int) and 0 <= c < 1000), memory)):
                raise TypeError("Memory elements must be 0 <= int < 1000")

            self._memory = memory
        else:
            self._memory = [0] * 100

    def __iter__(self):
        """
        Generate iterator for computing
        """
        return self

    @operation_logger
    def __next__(self):
        """
        Make a single step of computing
        :return: Next operation address
        """
        command = self._memory[self.program_counter]

        # Split command in instruction code and address
        code = command // 100
        addr = command % 100

        if code == 0:
            raise StopIteration
        elif code == 1:
            self._accumulator += self._memory[addr]
            if self._accumulator >= 1000:
                self._accumulator %= 1000
                self._flag = True
            else:
                self._flag = False
            self._program_counter += 1
        elif code == 2:
            self._accumulator -= self._memory[addr]
            if self._accumulator < 0:
                self._accumulator = 1000 - self._accumulator
                self._flag = True
            else:
                self._flag = False
            self._program_counter += 1
        elif code == 3:
            self._memory[addr] = self._accumulator
            self._program_counter += 1
        elif code == 5:
            self._accumulator = self._memory[addr]
            self._program_counter += 1
        elif code == 6:
            self._program_counter = addr
        elif code == 7:
            self._program_counter = addr if self._accumulator == 0 and not self.flag else self._program_counter + 1
        elif code == 8:
            self._program_counter = addr if not self._flag else self._program_counter + 1
        elif code == 9 and addr == 1:
            try:
                i = self._input_queue.pop(0)
            except IndexError:
                raise LmcEmptyInputError("Trying to read head from empty input queue")

            if not isinstance(i, int) or i < 0 or i >= 1000:
                raise ValueError(f"{i} is not a valid input")

            self._accumulator = i
            self._program_counter += 1
        elif code == 9 and addr == 2:
            self._output_queue.append(self._accumulator)
            self._program_counter += 1
        else:
            raise LmcInvalidOperation(f"invalid operation {command}")

        self._program_counter %= 100

        return self._program_counter

    def compute(self):
        """
        Compute all programm
        :return: Output queue
        """
        if self._verbose:
            print("Computing...")
        for _ in self:
            pass
        return self.output_queue

    # Use @property for mark variable as read only.
    # If the variable is a list, it returns a copy of list, for prevent editing of values.
    @property
    def memory(self):
        return self._memory.copy()

    @property
    def program_counter(self):
        return self._program_counter

    @property
    def accumulator(self):
        return self._accumulator

    @property
    def flag(self):
        return self._flag

    @property
    def input_queue(self):
        return self._input_queue.copy()

    @property
    def output_queue(self):
        return self._output_queue.copy()

    @property
    def log_operations(self):
        if self._track_operations:
            return self._log_operations
        return None


class LmcEmptyInputError(Exception):
    pass


class LmcInvalidOperation(Exception):
    pass
