rem @echo off
rem check file move to dir
if exist rename.from rm rename.dir1
if exist rename.dir rmdir rename.dir

touch rename.from
mkdir rename.dir
.\renametest rename.from rename.dir
if not exist rename.from goto end
if exist rename.dir/rename.from goto end
echo Test #5 failed

:end
if exist rename.from del rename.from
if exist rename.dir rmdir rename.dir
