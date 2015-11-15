import sys, os, commands

dir_path = sys.argv[1]


f_list = [x for x in commands.getstatusoutput("find %s -type f"%dir_path)[1].split("\n") if x.endswith(".root")]
print f_list
for f in f_list:
  print f
  os.system("scp -P 4280 %s hyunyong@ui20.sdfarm.kr:/cms/scratch/hyunyong/test_mv/"%f)

