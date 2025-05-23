
set MATLAB=F:\Develop\Matlab2023

cd .

if "%1"=="" ("F:\Develop\Matlab2023\bin\win64\gmake"  -f theta_gen.mk all) else ("F:\Develop\Matlab2023\bin\win64\gmake"  -f theta_gen.mk %1)
@if errorlevel 1 goto error_exit

exit /B 0

:error_exit
echo The make command returned an error of %errorlevel%
exit /B 1