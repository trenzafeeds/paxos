#!/usr/bin/env python

"""
record.py

USAGE:
$ ./paxos [INT] | tests/record.py

"""

import sys

def process_line(inputline):
    return [int(num) for num in inputline.split() if num.isdigit()]

def avg_msg(dataarr):
    nodes = len(dataarr)
    messages = 0
    for entry in dataarr:
        messages += entry[2]
    return (messages, float(messages)/float(nodes))

if __name__== "__main__":
    argarr = []
    numarr = []
    if not sys.stdin.isatty():
        while True:
            line = sys.stdin.readline()
            if not line: break;
            if line[0:7] == "Process" and line not in argarr:
                argarr.append(line)
        for procline in argarr:
            numarr.append(process_line(procline))
       	result = avg_msg(numarr)
        print "Total messages: {}\nAverage messages per node {}".format(result[0], result[1])

    else:
        print "Please pipe output of paxos into record.py"
        
        
