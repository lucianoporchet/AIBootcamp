import json
import sys
import urllib.request

if len(sys.argv) != 2:
    print(sys.argv)
    sys.exit(0)

isValid = False

data = []
dataSize = len(data)
with open(sys.argv[1]) as f:
    for line in f:
        data.append(line)
    if "resultData" in line:
    	isValid = True

if not isValid:
	data.append("], \"resultData\":{\"hasWin\":false,\"resultPayload\":\"AIBot Crashed Detected!\"}}")

with open(sys.argv[1], "w+") as f:
	f.truncate(0)

	for line in data:
		f.write(line)
