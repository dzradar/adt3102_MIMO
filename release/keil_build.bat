@echo off
set UV_PRO_PATH=%1/app_lib/%2.uvprojx
echo Keil lib building...
UV4.exe -b -X -j0 -r %UV_PRO_PATH% -o ./build_log