@echo off
rem check whether a dir move removes existing dir
if exist rename.dir1 rmdir rename.dir1
if exist rename.dir2 rmdir rename.dir2

mkdir rename.dir1
mkdir rename.dir2
.\renametest rename.dir1 rename.dir2
if not exist rename.dir1 goto end
echo Test #4 failed

:end
if exist rename.dir1 rmdir rename.dir1
if exist rename.dir2 rmdir rename.dir2
