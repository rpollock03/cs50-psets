from cs50 import get_string
from sys import argv
import sys


def main():
    # exactly 1 argument must be passed in
    if len(sys.argv) != 2:
        print("Usage: python bleep.py dictionary")
        sys.exit("Usage: python bleep.py dictionary")
    # make sure argument is a .txt file
    if argv[1].endswith(".txt"):
        fileName = argv[1]
    else:
        print("Usage: python bleep.py dictionary")
        sys.exit("Usage: python bleep.py dictionary")
    # open .txt file containing banned words, read contents and break into lines
    f = open(fileName, 'r')
    if f.mode == 'r':
        lines = f.read().splitlines()
    # get user to be censored from user
    message = get_string("What message would you like to censor?\n")
    # break message into individual words
    for word in message.split(" "):
        # bool is for breaking out of for loops
        isBannedWordFound = False
        # compare each word in the message against each banned word
        for line in lines:
            # if matches, case insensitively, print *s and break out of loop
            if line == word or word == line.upper():
                wordLength = len(line)
                print("*"*wordLength, end=" ")
                isBannedWordFound = True
                break
        # if no match was found, print the word as is
        if not isBannedWordFound:
            print(word, end=" ")
    print()


if __name__ == "__main__":
    main()
