@echo off
rem check trivial dir move
if exist rename.dir1 rmdir rename.dir1
if exist rename.dir2 rmdir rename.dir2

mkdir rename.dir1
.\renametest rename.dir1 rename.dir2
if not exist rename.dir1 goto end
echo Test #3 failed

:end
if exist rename.dir1 rmdir rename.dir1
if exist rename.dir2 rmdir rename.dir2
