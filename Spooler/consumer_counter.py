import re, sys
matcher = re.compile ("C\d+: Prod \d+ String \d+")

def usage():
    print ("Usage: python3 consumer_counter.py <Filename>\nOutputs the number of producer strings consumed in Filename")
    sys.exit (1)

def main (filename):
    lines_matched = 0
    with open (filename, "r") as f:
        for line in f:
            if matcher.match (line):
                lines_matched += 1
    print ("{} lines consumed".format (lines_matched))

if __name__ == '__main__':
    if (len (sys.argv) != 2):
        usage ()
    else:
        main (sys.argv[1])
