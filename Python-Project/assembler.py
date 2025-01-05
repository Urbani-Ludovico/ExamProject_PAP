class Assembler(object):
    _commands = ["ADD", "BRA", "BRP", "BRZ", "DAT", "HLT", "INP", "LDA", "OUT", "STA", "STO", "SUB"]

    def __init__(self, file_path: str, verbose: bool = False):
        self._verbose = verbose

        self.success = None

        self.lmc_memory = [0] * 100
        self._available_addresses = {}

        if self._verbose:
            print("Loading file...")
        try:
            file = open(file_path, "r")
        except (FileNotFoundError, TypeError) as error:
            self.success = False
            raise error

        self.assembly = file.read().split("\n")
        if self._verbose:
            print(f"Read {len(self.assembly)} lines")

        self._compile()

    def _compile(self):
        self._get_labels()

    def _get_labels(self):
        if self._verbose:
            print(f"Extracting labels...")

        for cell, line in enumerate(self.assembly):
            parts = get_line_parts(line)

            if len(parts):
                if parts[0] not in self._commands:
                    self._available_addresses[parts[0]] = cell


def get_line_parts(line):
    line = remove_line_comments(line)
    return line.upper().split()


def remove_line_comments(line):
    comment_index = line.find("//")
    if comment_index >= 0:
        return line[:comment_index]
    return line
