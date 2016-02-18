#!/bin/bash

readDir=$1
srmls srm://cluster142.knu.ac.kr:8443/srm/managerv2?SFN=/pnfs/knu.ac.kr/data/cms/store/user/ygo/$readDir >& cplist.txt
awk '{print $2}' cplist.txt > tmplist.txt

: <<'END'
storeDir=$2
scDir="/u/user/goyeonju/scratch/files/"
if [ ! -d "$scDir$storeDir" ]; then
  mkdir $scDir$storeDir
fi

exec < tmplist.txt
set i=1
while read line
do
	if [ $i -gt 1 ]; then
		srmcp srm://cluster142.knu.ac.kr:8443/srm/managerv2?SFN=$line file:////$scDir$storeDir
	fi
	i=$(expr $i + 1);
done
END
