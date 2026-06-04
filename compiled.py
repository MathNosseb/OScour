import sys
import re

opcodes = {
    "mov_eax_imm": b"\xB8",
    "add_eax_imm": b"\x05",
    "sub_eax_imm": b"\x2D",
    "push_eax": b"\x50",
    "pop_eax": b"\x58",
    "ret": b"\xC3",
    "mov_eax_mem": b"\xA1",  # mov eax, moffs32
    "mov_mem_eax": b"\xA3",  # mov moffs32, eax
}

def to_imm32(n):
    if isinstance(n, int):
        return n.to_bytes(4, "little", signed=True)
    return int(n, 0).to_bytes(4, "little", signed=True)

def parse_labels(lines):
    labels = {}
    addr = 0

    for line in lines:
        line = line.strip()
        if not line or line.endswith(":"):
            if line.endswith(":"):
                labels[line[:-1]] = addr
            continue

        parts = line.split()
        op = parts[0]

        if op in ["mov", "add", "sub"]:
            addr += 5
        elif op in ["push", "pop", "ret"]:
            addr += 1
        elif op == "call":
            addr += 5
        elif op == "jmp":
            addr += 5

    return labels

def assemble(filename):
    lines = open(filename).read().splitlines()
    labels = parse_labels(lines)

    output = bytearray()
    pc = 0

    for line in lines:
        line = line.strip()

        if not line or line.endswith(":"):
            continue

        parts = re.split(r"[ ,]+", line)
        op = parts[0]
        # MOV EAX, [imm]
        if op == "mov" and parts[1] == "eax" and parts[2].startswith("["):
            addr = parts[2].strip("[]")
            output += opcodes["mov_eax_mem"]
            output += to_imm32(addr)
            pc += 5
        elif op == "mov" and parts[1].startswith("[") and parts[2] == "eax":
            addr = parts[1].strip("[]")
            output += opcodes["mov_mem_eax"]
            output += to_imm32(addr)
            pc += 5
        # MOV EAX, imm
        elif op == "mov" and parts[1] == "eax":
            output += opcodes["mov_eax_imm"]
            output += to_imm32(parts[2])
            pc += 5

        # ADD EAX, imm
        elif op == "add" and parts[1] == "eax":
            output += opcodes["add_eax_imm"]
            output += to_imm32(parts[2])
            pc += 5

        # SUB EAX, imm
        elif op == "sub" and parts[1] == "eax":
            output += opcodes["sub_eax_imm"]
            output += to_imm32(parts[2])
            pc += 5

        # PUSH/POP
        elif op == "push" and parts[1] == "eax":
            output += opcodes["push_eax"]
            pc += 1

        elif op == "pop" and parts[1] == "eax":
            output += opcodes["pop_eax"]
            pc += 1

        # RET
        elif op == "ret":
            output += opcodes["ret"]
            pc += 1

        # JMP label
        elif op == "jmp":
            target = labels[parts[1]]
            rel = target - (pc + 5)
            output += b"\xE9"
            output += to_imm32(rel)
            pc += 5

        # CALL label
        elif op == "call":
            target = labels[parts[1]]
            rel = target - (pc + 5)
            output += b"\xE8"
            output += to_imm32(rel)
            pc += 5

        else:
            raise Exception(f"Unknown instruction: {line}")

    open("out.bin", "wb").write(output)
    print("Generated out.bin")

if __name__ == "__main__":
    assemble(sys.argv[1])