#!/usr/bin/env python
import sys, os, commands

dir_path = sys.argv[1]


i_path = os.getcwd()
f_list = [x for x in commands.getstatusoutput("find %s -type f"%dir_path)[1].split("\n") if x.endswith(".root")]
dir_name ="test_list"#dir_path.split("/")[5]+"_"+dir_path.split("/")[6]

isDir = True
for d in os.listdir(".") :
  if d == dir_name: 
    isDir = False
    break
if isDir : os.mkdir(dir_name)

os.chdir(dir_name)
if os.listdir(".") : os.system("rm *")
path_head = 'root://cms-xrdr.sdfarm.kr:1094//xrd/'
#list_log = open("list_log.log","w")

co = 1
max_n = 100
f_in = 1
tmp_f = open(dir_name+"_%03d.txt"%f_in,"w")
for f in f_list :
  if divmod(co,max_n)[1]==0:
    tmp_f.close()
    f_in += 1
    tmp_f = open(dir_name+"_%03d.txt"%f_in,"w")
  #tmp_f.write(f.replace("/xrootd",path_head)+"\n")
  tmp_f.write(i_path+"/"+f+"\n")
  co += 1

