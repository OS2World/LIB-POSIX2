@echo off
rem check move file to write protected file
if exist rename.from del rename.from
if exist rename.to del rename.to

touch rename.from
touch rename.to
attrib +R rename.to 
.\renametest rename.from rename.to

if exist rename.from goto end
if exist rename.to goto end
echo Test #6 failed

:end
if exist rename.from del rename.from
if exist rename.to attrib -R rename.to
if exist rename.to del rename.to
