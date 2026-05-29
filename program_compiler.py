import os

PATH = "Programs/"

variables = {}

def santize_command(command : str) -> str:
    command = command.replace("\n","")
    return command

def parse_command(command):
    #recuperation opcode
    mots = command.split()
    
    match mots[0]:
        case 'print':
            opcode = (0x01).to_bytes(1, "big")

            operande = " ".join(mots[1:]).encode("utf-8")
            operande += bytes([0xFF, 0xFF])

            print(opcode.hex(), operande.hex())

            return (opcode, operande)

        case 'add':
            opcode = (0x02).to_bytes(1, "big")
            a = int(mots[1]).to_bytes(4, 'big', signed=True)
            b = int(mots[2]).to_bytes(4, 'big', signed=True)

            operande = a + b
            print(opcode.hex(), operande.hex())
            return (opcode, operande)
        
        case 'sub':
            opcode = (0x03).to_bytes(1, "big")
            a = int(mots[1]).to_bytes(4, 'big', signed=True)
            b = int(mots[2]).to_bytes(4, 'big', signed=True)

            operande = a + b
            print(opcode.hex(), operande.hex())
            return (opcode, operande)
        
        case 'mult':
            opcode = (0x04).to_bytes(1, "big")
            a = int(mots[1]).to_bytes(4, 'big', signed=True)
            b = int(mots[2]).to_bytes(4, 'big', signed=True)

            operande = a + b
            print(opcode.hex(), operande.hex())
            return (opcode, operande)
        
        case 'div':
            opcode = (0x05).to_bytes(1, "big")
            a = int(mots[1]).to_bytes(4, 'big', signed=True)
            b = int(mots[2]).to_bytes(4, 'big', signed=True)

            operande = a + b
            print(opcode.hex(), operande.hex())
            return (opcode, operande)
        
        case 'mov':
            """
            premier octet de l operande:
            si on a 0x01 au debut c'est qu on a creation d une variable
            si c est 0x02 c est que la variable existe deja
            seconde octet de l operande:
            si c est 0x01 c est que la deuxieme valeur existe deja
            si c est 0x02 c est que la deuxieme valeur est generique

            les prochains bytes sont:
            [1B longueur][N bytes nom]

            et si le deuxieme est une valeur qui existe deja alors
            [1B longueur][N bytes nom]
            sinon 
            [1B longueur][Data]
            """
            opcode = (0x06).to_bytes(1, "big")
            operande = b''

            create_a = False
            create_b = False

            #detection variables
            if not mots[1] in variables:
                #si on a pas encore la variable on la crée
                #on va donner une taille a la variable
                operande += (0x01).to_bytes(1, "big")
                create_a = True
            else:
                operande += (0x02).to_bytes(1, "big")
            if mots[2] in variables:
                #on detecte que la variable existe
                operande += (0x01).to_bytes(1, "big")
                variables[mots[1]] = variables[mots[2]]
            else:
                #la deuxieme valeur existe pas
                operande += (0x02).to_bytes(1, "big")
                variables[mots[1]] = mots[2]
                create_b = True

            #a et b -> les valeurs
            #A
            operande += len(mots[1]).to_bytes(1, "big")
            operande += mots[1].encode()#on met le nom de la variable
            #B
            operande += len(mots[2]).to_bytes(1, "big")
            operande += mots[2].encode()




            #suite des données
            print(operande)
            return (opcode, operande)



        case _:
            return

def compile_file(fichier):
    with open(PATH + fichier, "r") as f:
        data = f.readlines()

    with open("Binaries/compiledPrograms.bin", "wb") as f:
        for index in range(len(data)):
            commands = parse_command(santize_command(data[index]))
            for command in commands:
                f.write(command)
    
    

fichiers = os.listdir(PATH)
for file in fichiers:
    compile_file(file)