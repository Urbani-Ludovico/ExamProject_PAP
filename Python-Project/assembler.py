class Assembler(object):
    _commands = ["ADD", "BRA", "BRP", "BRZ", "DAT", "HLT", "INP", "LDA", "OUT", "STA", "STO", "SUB"]

    def __init__(self, file_path: str, verbose: bool = False):
        """
        Init assembler and load assembly file.
        :param file_path: Path to assembly file
        :param verbose: Print messages for easy debug
        """

        self._verbose = verbose
        if self._verbose:
            print("- - - LMC Assembler - - -")

        # Compile success. It is used if another program capture compile exception without exit
        self.success = None

        self._lmc_memory = [0] * 100  # Init Lmc memory to zero
        self._available_addresses = {}

        if self._verbose:
            print("Loading file...")
        try:
            file = open(file_path, "r")
        except (FileNotFoundError, TypeError) as error:
            self.success = False
            raise error

        # Read assembly as list of strings. Lmc has 100 cells of memory (100 instructions max), so I assume that the file is not big, and it is not necessary to read file line-by-line.
        self.assembly = file.read().split("\n")
        if self._verbose:
            print(f"Read {len(self.assembly)} lines")

    def compile(self) -> None:
        """
        Compile code and generate memory for LMC.
        """

        # Before compile, loop over all code and get lebels positions
        # Having a short number of istruction per file (max 100, insted assembler raise exception), this has not a large computational cost
        self._get_labels()

        if self._verbose:
            print(f"Compiling...")

        cell = -1
        for line in self.assembly:
            # Split assembly line into array of arguments
            parts = get_line_parts(line)

            # Remove label at beginning because it's already parsed in _get_labels
            if len(parts) and parts[0] in self._available_addresses:
                parts.pop(0)

            # Skip empty line
            if not len(parts):
                continue

            # Having 100 cells of memory for both instructions and data, if file exceeds 100 insstructions the program raise an exceptions
            cell += 1
            if cell >= 100:
                self.success = False
                raise AssemblerMaxLengthError("Max length of 100 instructions on memory exceeded")

            command = parts.pop(0)

            if command == "ADD":
                self._lmc_memory[cell] = 100 + self._get_address(parts)
            elif command == "SUB":
                self._lmc_memory[cell] = 200 + self._get_address(parts)
            elif command == "STA":
                self._lmc_memory[cell] = 300 + self._get_address(parts)
            elif command == "LDA":
                self._lmc_memory[cell] = 500 + self._get_address(parts)
            elif command == "BRA":
                self._lmc_memory[cell] = 600 + self._get_address(parts)
            elif command == "BRZ":
                self._lmc_memory[cell] = 700 + self._get_address(parts)
            elif command == "BRP":
                self._lmc_memory[cell] = 800 + self._get_address(parts)
            elif command == "INP":
                self._lmc_memory[cell] = 901
            elif command == "OUT":
                self._lmc_memory[cell] = 902
            elif command == "HLT":
                self._lmc_memory[cell] = 0
            elif command == "DAT":
                # Dat, different from other commands, can read a number up to 999 because it is a data and not an address
                value = 0
                if len(parts):
                    try:
                        value = int(parts.pop(0))
                    except ValueError:
                        pass
                    if value < 0 or value >= 1000:
                        raise ValueError(f"{value} is not a correct value")
                self._lmc_memory[cell] = value
            else:
                self.success = False
                error = AssemblerUnknownCommandError(f"Unknown commnd in line {cell + 1}")
                error.add_note(" ".join(parts))
                raise error

    def _get_labels(self) -> None:
        """
        Loop over lines of assembly and generate a dictionary to translate labels to addresses
        """
        if self._verbose:
            print(f"Extracting labels...")

        cell = 0
        for line in self.assembly:
            parts = get_line_parts(line)

            if len(parts):
                if parts[0] not in self._commands:
                    self._available_addresses[parts[0]] = cell
                cell += 1

    def _get_address(self, parts: list[str]) -> int:
        """
        Parse an address. Decide if a value after a command is an address or a label. If is a label checks if it exists else raise an exception.
        :param parts: Parts list. It gets all list because it checks if it is empty or not
        :return: Address
        """
        if not len(parts):
            self.success = False
            raise AssemblerMissingAddressError("Missing address")
        part = parts.pop(0)

        try:
            cell = int(part)
            if 0 <= cell < 100:
                return cell
            else:
                self.success = False
                raise AssemblerWrongAddressError(f"{cell} is not a valid address")
        except ValueError:
            pass

        if part in self._available_addresses:
            if 0 <= self._available_addresses[part] < 100:
                return self._available_addresses[part]

            self.success = False
            raise AssemblerWrongAddressError(f"{part}[{self._available_addresses[part]}] is not a valid address")

        self.success = False
        raise AssemblerWrongAddressError(f"{part} is not a valid address")

    # Use @property for mark variable as read only.
    # If the variable is a list, it returns a copy of list, for prevent editing of values.
    @property
    def lmc_memory(self):
        return self._lmc_memory.copy()


def get_line_parts(line: str) -> list[str]:
    """
    Splits line into command
    :param line: Assembly line
    :return: Command split
    """
    line = remove_line_comments(line)
    return line.upper().split()


def remove_line_comments(line) -> str:
    """
    Remove comments from line
    :param line: Assembly line
    :return: Command without comment
    """
    comment_index = line.find("//")
    if comment_index >= 0:
        return line[:comment_index]
    return line


class AssemblerUnknownCommandError(Exception):
    pass


class AssemblerMaxLengthError(Exception):
    pass


class AssemblerMissingAddressError(Exception):
    pass


class AssemblerWrongAddressError(Exception):
    pass
