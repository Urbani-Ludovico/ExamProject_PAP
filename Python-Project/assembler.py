class Assembler(object):
    def __init__(self, file_path: str, verbose: bool = False):
        self.verbose = verbose

        self.success = None

        self.lmc_memory = [0] * 100

        if self.verbose:
            print("Loading file...")
        try:
            file = open(file_path, "r")
        except (FileNotFoundError, TypeError) as error:
            self.success = False
            raise error

        self.assembly = file.read().split("\n")
