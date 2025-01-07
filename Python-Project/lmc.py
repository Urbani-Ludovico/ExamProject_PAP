class Lmc(object):
    def __init__(self, input_queue: list[int], memory: list[int] = None, verbose: bool = False):
        self._verbose = verbose
        if self._verbose:
            print("- - - LMC - - -")

        if self._verbose:
            print("Creating environment...")
        self.program_counter = 0
        self.accumulator = 0
        self.flag = False
        self.input_queue = input_queue
        self.output_queue = []

        if self._verbose:
            print("Creating memory...")
        if memory is not None:
            if not isinstance(memory, list) or len(memory) != 100:
                raise TypeError("Memory must be a list[100]")

            if any(map(lambda c: not isinstance(c, int) or c < 0 or c >= 1000, memory)):
                raise TypeError("Memory elements must be 0 <= int < 1000")

            self.memory = memory
        self.memory = [0] * 100

    def __iter__(self):
        return self

    def __next__(self):
        command = self.memory[self.program_counter]

        code = command // 100
        addr = command % 100
        if code == 0:
            raise StopIteration
        elif code == 1:
            self.accumulator += self.memory[addr]
            if self.accumulator >= 1000:
                self.accumulator %= 1000
                self.flag = True
            else:
                self.flag = False
            self.program_counter += 1
        elif code == 2:
            self.accumulator -= self.memory[addr]
            if self.accumulator < 0:
                self.accumulator = 1000 - self.accumulator
                self.flag = True
            else:
                self.flag = False
            self.program_counter += 1
        elif code == 3:
            self.memory[addr] = self.accumulator
            self.program_counter += 1
        elif code == 5:
            self.accumulator = self.memory[addr]
            self.program_counter += 1
        elif code == 6:
            self.program_counter = addr
        elif code == 7:
            if self.accumulator == 0 and not self.flag:
                self.program_counter = addr
        elif code == 8:
            if not self.flag:
                self.program_counter = addr
        elif code == 9 and addr == 1:
            try:
                i = self.input_queue.pop(0)
            except IndexError:
                raise LmcEmptyInputError("Trying to read head from empty input queue")

            if not isinstance(i, int) or i < 0 or i >= 1000:
                raise ValueError(f"{i} is not a valid input")

            self.accumulator = i
        elif code == 9 and addr == 2:
            self.output_queue.append(self.accumulator)
        else:
            raise LmcInvalidOperation(f"invalid operation {command}")

        self.program_counter %= 100

        return self.program_counter

    def compute(self):
        for _ in self:
            pass
        return self.output_queue


class LmcEmptyInputError(Exception):
    pass


class LmcInvalidOperation(Exception):
    pass
