import elftools
from PIL.ImageChops import offset
from elftools.elf.elffile import ELFFile
RELEVENT_SECTIONS_NAMES = [".primary", ".text", ".seseq",".mySection", "mySection2", ".mySegment", ".mySegment2"]

def find_sections(file):
    elf_file = ELFFile(file)
    sections = {}
    for section in elf_file.iter_sections():
        #print(section.name)
        if(section.name in RELEVENT_SECTIONS_NAMES):
            sections[section.name] = (hex(section['sh_offset']), hex(section['sh_size']))
            offset = section['sh_offset']
            len = section['sh_size']
            file.seek(offset)
            raw = file.read(len)
            buffer = bytearray(raw)
            #for i in buffer:
            #    print(hex(i))

            #break
    return sections

def calculate_hash(file_path: str, section):
    hash_value = 0x811c9dc5
    prime = 0x01000193
    with open("hash.txt", "w") as fo:
        with open(file_path, "r+b") as f:
            offset = int(section[0], 16)
            size = int(section[1], 16)
            f.seek(offset)
            for i in range(size):
                line = f.read(1)
                value_hex = int(line.hex(), 16)
                hash_value = hash_value^value_hex
                hash_value = (hash_value * prime) & 0xFFFFFFFF
                #print(hex(hash_value))
                fo.write(hex(value_hex)+ '\n')
        return hash_value&0xff





def encrypt_hex(line, value, seed):
    """

    :param line: variable to help with encryption, can be used as a key or something
    :param value: hex value to encrypt
    :param seed: hex value to xor with
    :return:
    """
    key = int(line, 16) + seed
    result = (key ^ value) & 0xFF
    return result

def decrypt_hex(line, value, seed):
    key = int(line, 16) - seed
    result = (key ^ value) & 0xFF
    return result
def encrypt_section(file_loc, section, seed = 0x1337):
    rewrite = {}
    with open(file_loc, "r+b") as f:
        offset = int(section[0], 16)
        size = int(section[1], 16)
        f.seek(offset)
        for i in range(size):

            line = f.read(1)
            value_hex = int(line.hex(), 16)
            # print(value_hex)
            # print(type(value_hex))

            crypted = encrypt_hex(hex(i), value_hex, seed)
            decypted = encrypt_hex(hex(i), crypted, seed)
            rewrite[offset+i] = crypted
            #print(f"line: {hex(offset+i)}, value: {hex(value_hex)}, {hex(crypted)}, {hex(decypted)}")
            #encrypted_line = encrypt_hex(line, 0xDEADBEEF, 0x1337)
            f.seek(offset + i)
            f.write(crypted.to_bytes(1, byteorder='big'))
            #print(f"rewriting {hex(offset+i)} with {hex(crypted).encode()}")
            #f.write(encrypted_line)
    #with open()


def main():
    file = open("agent.out", "rb")
    sec = find_sections(file)
    print(sec)
    hash_value = calculate_hash("agent.out", sec[".primary"])
    print(hash_value)
    encrypt_section("agent.out", sec[".mySegment"])
    #print("encrypting .to_encrypt")
    encrypt_section("agent.out", sec[".seseq"], hash_value^0x1337)
    print(hash_value^0x1337)
    encrypt_section("agent.out", sec[".mySegment2"])
    sec2 = find_sections(file)

    print(sec)
main()


