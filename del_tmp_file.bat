Rem zcc
Rem 2016.07.20 14:06
Rem 删除生成的临时文件
Rem

echo 正在清理系统垃圾文件，请稍等......

del /f /s /q *.ncb
del /f /s /q *.pch
del /f /s /q *.obj
del /f /s /q *.tlh
del /f /s /q *.tli
del /f /s /q *.idb
del /f /s /q *.pdb
del /f /s /q *.res
del /f /s /q *.ilk
del /f /s /q *.aps
del /f /s /q *.exp
del /f /s /q *.enc


echo 清理系统垃圾完成！

pause 
