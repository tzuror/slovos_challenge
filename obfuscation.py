import random

def generate_random_string(length):
    letters = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'
    return ''.join(random.choice(letters) for _ in range(length))
def generate_random_number(length):
    digits = '123456789'
    return ''.join(random.choice(digits) for _ in range(length))
def generate_random_function(name):
    """
    :return:
    """
    function_code = f"""
int __attribute__((used, constructor)){name}() 
{{
    int a = {generate_random_number(5)};
    int b = {generate_random_number(5)};
    a&b;
    int x = 0;
    switch(a){{
        case {generate_random_number(5)}:
            x = {generate_random_number(3)}; x+=10;
        case {generate_random_number(6)}:
            x = {generate_random_number(3)}; x+=10;;
        case {generate_random_number(6)}:
            x = {generate_random_number(3)}; x+=10;;
        case {generate_random_number(6)}:
            x = {generate_random_number(3)}; x+=10;;
        case {generate_random_number(6)}:
            x = {generate_random_number(3)}; x+=10;;
        case {generate_random_number(6)}:
            x = {generate_random_number(3)}; x+=10;;
        case {generate_random_number(6)}:
            x = {generate_random_number(3)}; x+=10;;
        case {generate_random_number(6)}:
            x = {generate_random_number(3)}; x+=10;;
        case {generate_random_number(6)}:
            x = {generate_random_number(3)}; x+=10;;
        case {generate_random_number(6)}:
            x = {generate_random_number(3)}; x+=10;;
        case {generate_random_number(6)}:
            x = {generate_random_number(3)}; x+=10;;
        case {generate_random_number(6)}:
            x = {generate_random_number(3)}; x+=10;
        case {generate_random_number(6)}:
            x = {generate_random_number(3)}; x+=10;
        case {generate_random_number(6)}:
            x = {generate_random_number(3)}; x+=10;
        default:
            x = {generate_random_number(3)}; x+=10;
    }}
    char buffer[16];
    for (int i = 0; i < 16; i++) {{
        buffer[i] = '{generate_random_string(1)}';
    }}
    return x + a + b;
}}
"""
    return function_code

def main():
    AMOUNT1 = 500

    name_set = set()
    file1 = open("junk1.c", "w")
    for _ in range(AMOUNT1):
        name = generate_random_string(10)
        while name in name_set:
            name = generate_random_string(10)
        name_set.add(name)
        func = generate_random_function(name)
        file1.write(func)
    file1.close()

    AMOUNT2 = 733
    file2 = open("junk2.c", "w")
    for _ in range(AMOUNT2):
        name = generate_random_string(10)
        while name in name_set:
            name = generate_random_string(10)
        name_set.add(name)
        func = generate_random_function(name)
        file2.write(func)
    file2.close()

    AMOUNT3 = 456
    file3 = open("junk3.c", "w")
    for _ in range(AMOUNT3):
        name = generate_random_string(10)
        while name in name_set:
            name = generate_random_string(10)
        name_set.add(name)
        func = generate_random_function(name)
        file3.write(func)
    file3.close()



main()

