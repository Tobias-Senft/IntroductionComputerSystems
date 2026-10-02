import pandas as pd
data = pd.read_csv("OutputFolder/results.csv")

dynamic = data[data["method"] == "dynamic"]
basic = data[data["method"] == "basic"]

dynamicSummery = dynamic[["double_counted","counted","runtime_seconds"]].describe()
basicSummery = basic[["double_counted","counted","runtime_seconds"]].describe()
print(f"Dynamic data \n{dynamicSummery}")
print(f"Baic data \n{basicSummery}")

def outputPerLevel(levelString):
    dynamic_level = dynamic[dynamic["level"] == levelString]
    basic_level = basic[basic["level"] == levelString]
    dynamicSummery_level = dynamic_level[["double_counted","counted","runtime_seconds"]].describe()
    basicSummery_level = basic_level[["double_counted","counted","runtime_seconds"]].describe()
    print(f"{levelString} Dynamic data \n{dynamicSummery_level}")
    print(f"{levelString} Baic data \n{basicSummery_level}")
    return

outputPerLevel("easy")
outputPerLevel("medium")
outputPerLevel("hard")
outputPerLevel("impossible")