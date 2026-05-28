import os

PATH = "Programs/"
BINAIRES = "Binaries/"

compilator = [
    ["print",0x1]
]

def compile(fichier):
    content = ""

    #on compile le fichier
    with open(PATH + fichier, "r") as f:
        data = f.readlines()
    for instruction in data:
        op = instruction.split()
        for command in compilator:
            if command[0] == op[0]:
                len_command = len(op)
                match command[0]:
                    case "print":
                        for i in range(len_command-1):
                            texte = op[i+1].encode().hex()
                            content += hex(command[1]) + " " + texte + "\n"

    print(content)
    with open(BINAIRES + "compiledPrograms.bin", "wb") as f:
        #f.write(b"MYBIN")  # signature
        for line in content.splitlines():
            parts = line.split()

            if len(parts) != 2:
                continue

            opcode, hex_text = parts

            f.write(bytes([int(opcode, 16)]))
            f.write(bytes.fromhex(hex_text))
            f.write(b'\xff')  # flag de fin de ligne
            f.write(b'\xff')  # flag de fin de ligne
                        


# compile en binaire tous les fichiers
#on prend chaque fichiers du dossier

fichiers = os.listdir(PATH)


for fichier in fichiers:
    compile(fichier)