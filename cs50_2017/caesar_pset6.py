from sys import argv
if len(argv) == 2 and argv[1].isdigit():
    shift = int(argv[1])
    plaintext = input("Plaintext: ")
    print("Ciphertext: ", end = "")
    for i in plaintext:
        print((chr(((((ord(i) - 65) + shift) % 26) + 65)) if i.isupper() else chr(((((ord(i) - 97) + shift) % 26) + 97))) if i.isalpha() else i, end = "")
    print()
else:
    print("Arguments do not amount to 2 and/or argv[1] is not an int")