#!/bin/bash
echo Release git commit or local files?
echo 1: git commit [Will overwrite local files if selected.]
echo 2: local files
read var0
if [ $var0 = 1 ]
then
    #pull the latest version from the remote git repository
    git checkout .
    git pull --rebase
    
    #checkout the version you want to release
    echo -n "Please enter release commit ID:"
    read commit_ID
    git reset --hard $commit_ID
fi

echo Select project to release:
echo 1: adt3102_hare_breath
echo 2: adt3102_hare_human_detection
echo 3: adt3102_smoke_alarm
echo 4: adt3102_mole_gesture
echo 5: adt3102_resolution_test
echo 6: adt3102_smoke_alarm_4action
echo ========================== 
read var1
case $var1 in
  1)
    release_name=adt3102_hare_breath
    ;;
  2)
    release_name=adt3102_hare_human_detection
    ;;
  3)
    release_name=adt3102_smoke_alarm
    ;;
  4)
    release_name=adt3102_mole_gesture
    ;;
  5)
    release_name=adt3102_resolution_test
    ;;
  6)
    release_name=adt3102_smoke_alarm_4action
    ;;
  *)
    echo Invalid project name selected. 
	exit
    ;;
esac
project_path=../app/$release_name
echo Project $project_path selected.

filename=filelist_$release_name.f
if [ -f "$filename" ]; then
    echo "File list ready."
else
    echo Please get $filename ready in this folder.
	exit
fi

echo Select your keil compiler version:
echo 1: "<pArmCC>5060750::V5.06 update 6 (build 750)::.\ARMCC</pArmCC>"
echo 2: "<pArmCC>5050169::V5.05 update 2 (build 169)::ARMCC</pArmCC>"
echo ===============================================================
read var2
case $var2 in
  1)
    keil_compiler_version="<pArmCC>5060750::V5.06 update 6 (build 750)::.\ARMCC</pArmCC>"
    ;;
  2)
    keil_compiler_version="<pArmCC>5050169::V5.05 update 2 (build 169)::ARMCC</pArmCC>"
    ;;
  *)
    echo Invalid project name selected. 
	exit
    ;;
esac
sed -i "s|<pArmCC>.*$|$keil_compiler_version|g" ../app/$release_name/app_lib/$release_name.uvprojx

#compile lib file for app
./keil_build.bat $project_path $release_name
if [ $var0 = 1 ]
then
    git checkout ../app/$release_name/app_lib/$release_name.uvprojx
fi
cp $project_path/app_lib/Objects/$release_name.lib $project_path/src/
# package the files in filelist
dos2unix $filename
FILELIST=""
SPACE=" "
for LINE in `cat $filename`
do
	FILELIST="$FILELIST$SPACE$LINE"
	echo $LINE
done
echo $FILELIST
rm -f $release_name.tar.gz
tar -zcvf $release_name.tar.gz $FILELIST
#tar -x $release_name.tar.gz
#tar -zcvf test.tar.gz $FILELIST
#mv ./$release_name.tar.gz ../../release/$release_name/$release_name.tar.gz