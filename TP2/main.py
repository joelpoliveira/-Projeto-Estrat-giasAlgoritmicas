from sys import stdin, stdout
from pprint import pprint

MAX = 100000


def readln():
    return stdin.readline().rstrip()


def outln(n):
    stdout.write(str(n))
    stdout.write("\n")


def solve(dp, costs):
    for i in range(MAX):
        for j in range(MAX):
            pass


if __name__ == "__main__":
    costs = [0 for _ in range(MAX)]
    dp = [[False for _ in range(MAX)] for __ in range(MAX)]

    while True:
        line = list(map(int, readln().split()))

        # EOF
        if line == []:
            break
            
        # New pyramid to solve
        if line == [-1]:
            solve(dp, costs)
            #costs =  [0 for _ in range(50)]
            #dp = [[False for _ in range(50)] for __ in range(50)]

        id = line[0]

        if len(line) > 2:
            line = line[1::]
            for i in line:
                dp[id][i] = True
                dp[i][id] = True

        costs[id] = line[-1]

