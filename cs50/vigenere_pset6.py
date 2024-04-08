from sys import argv
if len(argv) == 2 and argv[1].isalpha():
    plaintext = input("Plaintext: ")
    k = 0
    print("Ciphertext: ", end = "")
    for i in plaintext:
        pcase = 65 if i.isupper() else 97
        if i.isalpha():
            print(chr(((ord(i) - pcase) + (ord(argv[1][k]) - (65 if argv[1][k].isupper() else 97))) % 26 + pcase), end = "")
            k = (k+1) % len(argv[1])
        else:
            print(i, end = "")
    print()
else:
    print("Error: Arguments do not amount to 2 and/or argv[1] is not an alphabetic string")