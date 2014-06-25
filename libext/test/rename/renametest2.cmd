@echo off
rem test whether rename replaces existing file
if exist rename.from del rename.from
if exist rename.to del rename.to

ver > rename.from
touch rename.to
.\renametest rename.from rename.to
if not exist rename.from goto end
echo Test #2 failed
del test.from

:end
if exist rename.to del rename.to
if exist rename.from del rename.from