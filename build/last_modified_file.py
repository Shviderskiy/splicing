import sys, os

if __name__ == "__main__":

    files = [ x for line in sys.stdin
                for x in line.strip().split()
                if x != "" and x != "\\" ]

    mtime = lambda file: os.stat(file).st_mtime
    result = max(set(files[1:]), key=mtime)
    print(result)
