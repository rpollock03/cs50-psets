import sys
from cs50 import get_string

# validate key
if len(sys.argv) != 2:
    print("Usage: python caesar.py k")
    sys.exit("Usage: python caesar.py k")
else:
    keyString = sys.argv[1]
    for c in keyString:
        if c.isalpha == True:
            print("Usage: python caesar.py k")
            sys.exit("Usage: python caesar.py k")
        else:
            key = int(sys.argv[1])

# get and validate plaintext
    plainText = get_string("Plaintext: ")
    print("ciphertext: ", end="")
    for c in plainText:
        if c.isalpha():
            if c.islower():
                # 97ascii value for lowercase a
                cipheredChar = (((ord(c)-97)+key) % 26)+97
                cipheredChar = chr(cipheredChar)
                print(cipheredChar, end="")
            elif c.isupper():
                # 65 ascii value for uppercase a
                cipheredChar = (((ord(c)-65)+key) % 26)+65
                cipheredChar = chr(cipheredChar)
                print(cipheredChar, end="")
        else:
            print(c, end="")
    print()

