@echo off
rem check trivial file move
if exist rename.from del rename.from
if exist rename.to del rename.to

touch rename.from
.\renametest rename.from rename.to
if exist rename.to goto end
echo Test #1 failed
if exist rename.from del rename.from

:end
if exist rename.to del rename.to
