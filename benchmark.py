import json
import sys
import time


def main():
    start = time.time()
    with open(sys.argv[1]) as f:
        data = json.load(f)
    end = time.time()
    a = data["vals"]
    print("Time to load: ", end - start)


main()
