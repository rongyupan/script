# replace sstr with rstr
def modifyip(tfile,sstr,rstr):
    try:
        lines=open(tfile,'r').readlines()   # split each lines
        flen=len(lines)-1
        for i in range(flen):
            if sstr in lines[i]:
                lines[i]=lines[i].replace(sstr,rstr)
        open(tfile,'w').writelines(lines)
        
    except Exception as e:
        print(e)

import os

file_list = list()

# root: absolute path name 
# dirnames: list of every folder name 
# filenames: list of every file name
# root, dirnames, filnames is matched in every cycle. 
for root, dirnames, filenames in os.walk("./"):
    for filename in filenames:
        if filename == "CMakeLists.txt":
            # acquire the absolute path of each cmake file. 
            file_list.append(os.path.join(root, filename))

# test the result
print(file_list)

for i in file_list:
    modifyip(i, "compute_30","compute_50")
    modifyip(i, "sm_30", "sm_50")

