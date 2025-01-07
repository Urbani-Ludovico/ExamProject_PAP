class Assembler(object):
    _commands = ["ADD", "BRA", "BRP", "BRZ", "DAT", "HLT", "INP", "LDA", "OUT", "STA", "STO", "SUB"]

    def __init__(self, file_path: str, verbose: bool = False):
        self._verbose = verbose
        if self._verbose:
            print("- - - LMC Assembler - - -")

        self.success = None

        self._lmc_memory = [0] * 100
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

    def compile(self) -> None:
        self._get_labels()

        if self._verbose:
            print(f"Compiling...")

        cell = -1
        for line in self.assembly:
            parts = get_line_parts(line)

            if len(parts) and parts[0] in self._available_addresses:
                parts.pop(0)

            if not len(parts):
                continue

            cell += 1
            if cell >= 100:
                self.success = False
                raise LmcAssemblyMaxLength("Max length of 100 instructions on memory exceeded")

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
                error = LmcAssemblyUnknownCommand(f"Unknown commnd in line {cell + 1}")
                error.add_note(" ".join(parts))
                raise error

    def _get_labels(self) -> None:
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
        if not len(parts):
            self.success = False
            raise LmcAssemblyMissingAddress("Missing address")
        part = parts.pop(0)

        try:
            cell = int(part)
            if 0 <= cell < 100:
                return cell
            else:
                self.success = False
                raise LmcAssemblyWrongAddress(f"{cell} is not a valid address")
        except ValueError:
            pass

        if part in self._available_addresses:
            if 0 <= self._available_addresses[part] < 100:
                return self._available_addresses[part]

            self.success = False
            raise LmcAssemblyWrongAddress(f"{part}[{self._available_addresses[part]}] is not a valid address")

        self.success = False
        raise LmcAssemblyWrongAddress(f"{part} is not a valid address")

    @property
    def lmc_memory(self):
        return self._lmc_memory.copy()


def get_line_parts(line: str) -> list[str]:
    line = remove_line_comments(line)
    return line.upper().split()


def remove_line_comments(line) -> str:
    comment_index = line.find("//")
    if comment_index >= 0:
        return line[:comment_index]
    return line


class LmcAssemblyUnknownCommand(Exception):
    pass


class LmcAssemblyMaxLength(Exception):
    pass


class LmcAssemblyMissingAddress(Exception):
    pass


class LmcAssemblyWrongAddress(Exception):
    pass

