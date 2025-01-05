
class Lmc(object):
    def __init__(self, memory: list[int] = None, verbose: bool = False):
        self._verbose = verbose
        if self._verbose:
            print("- - - LMC - - -")

        if self._verbose:
            print("Creating memory...")
        if memory is not None:
            if not isinstance(memory, list) or len(memory) != 100:
                raise TypeError("Memory must be a list[100]")

            if any(map(lambda c: not isinstance(c, int) or c < 0 or c >= 1000, memory)):
                raise TypeError("Memory elements must be 0 <= int < 1000")

            self.memory = memory
        self.memory = [0] * 100

