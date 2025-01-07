class Lmc(object):
    def __init__(self, input_queue: list[int], memory: list[int] = None, verbose: bool = False):
        self._verbose = verbose
        if self._verbose:
            print("- - - LMC - - -")

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
            if not isinstance(memory, list) or len(memory) != 100:
                raise TypeError("Memory must be a list[100]")

            if any(map(lambda c: not isinstance(c, int) or c < 0 or c >= 1000, memory)):
                raise TypeError("Memory elements must be 0 <= int < 1000")

            self._memory = memory
        self._memory = [0] * 100

    def __iter__(self):
        return self

    def __next__(self):
        command = self._memory[self.program_counter]

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
            if self._accumulator == 0 and not self.flag:
                self._program_counter = addr
        elif code == 8:
            if not self.flag:
                self._program_counter = addr
        elif code == 9 and addr == 1:
            try:
                i = self._input_queue.pop(0)
            except IndexError:
                raise LmcEmptyInputError("Trying to read head from empty input queue")

            if not isinstance(i, int) or i < 0 or i >= 1000:
                raise ValueError(f"{i} is not a valid input")

            self._accumulator = i
        elif code == 9 and addr == 2:
            self._output_queue.append(self._accumulator)
        else:
            raise LmcInvalidOperation(f"invalid operation {command}")

        self._program_counter %= 100

        return self._program_counter

    def compute(self):
        for _ in self:
            print(self._memory)
        return self.output_queue

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



class LmcEmptyInputError(Exception):
    pass


class LmcInvalidOperation(Exception):
    pass
