from datetime import datetime
from subprocess import call
import ctypes
import sys

def RepairReplayLog(replayLog):
    isValid = False
    data = []
    dataSize = len(data)
    with open(replayLog) as f:
        for line in f:
            data.append(line)
        if "resultData" in line:
    	    isValid = True
    if not isValid:
	    data.append("], \"resultData\":{\"hasWin\":false,\"resultPayload\":\"AIBot Crashed Detected!\"}}")

    with open(replayLog, "w+") as f:
        f.truncate(0)
        for line in data:
            f.write(line)

if len(sys.argv) != 8:
    print("Invalid arguments")
    print(sys.argv)
    sys.exit(0)

mapname = sys.argv[1]
AIBOOTCAMP2_ENGINE_DIR = sys.argv[2]
AIBOT_PATH = sys.argv[3]
TEAM = sys.argv[4]
ConnectToProcessDelay = sys.argv[5]
InitTime = sys.argv[6]
TurnTime = sys.argv[7]

now = datetime.now()

replayfile=now.strftime("%Y%m%d_%H%M%S")
mapnames = ["L_000", "L_001", "L_002","L_003","L_004","L_005","L_010","L_011","L_012","L_013","L_020","L_021","L_022","L_023","L_030","L_031","L_032","L_033","L_034","L_040","L_041","L_042"]
result = []
i = mapnames.index(mapname)
for k in range(i+1):
    result1 = call([AIBOOTCAMP2_ENGINE_DIR+"/AIBootCamp2.exe", "-dllpath", AIBOT_PATH, "-mode", "match", "-scene", mapnames[k], "-team", TEAM, "-replayfile", replayfile, "-connecttoprocessdelay", ConnectToProcessDelay, "-initdelay", InitTime, "-turndelay", TurnTime, "-quit", "-batchmode"])    
    result.append(ctypes.c_int32(result1).value)

if sum(result) == 0:
  	print("Matches Completed : Victory!")
else:
    print("Match Completed : Failure")
    print(result)
    RepairReplayLog(".\\Replays\\" + replayfile + "\\" + mapname + "_" + replayfile + ".replay")