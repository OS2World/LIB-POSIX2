@echo off
rem check move write protected file
if exist rename.from del rename.from
if exist rename.to del rename.to

touch rename.from
attrib +R rename.from
touch rename.to

.\renametest rename.from rename.to

if exist rename.from goto end
if exist rename.to goto end
echo Test #7 failed

:end
if exist rename.from attrib -R rename.from
if exist rename.from del rename.from
if exist rename.to attrib -R rename.to
if exist rename.to del rename.to
