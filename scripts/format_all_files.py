import os, fnmatch, subprocess

filelist = []

def run(command):
     subprocess.run(f"echo {command}", shell=True, check=True)
     subprocess.run(command, shell=True, check=True)

for root, dirs, files in os.walk('.'):
    for file in files:
        filelist.append(os.path.join(root,file))

for name in filelist: 
    if fnmatch.fnmatch(name, "*.cpp") or fnmatch.fnmatch(name, "*.h"):
        run(f"clang-format -i {name}")
        print(name)
