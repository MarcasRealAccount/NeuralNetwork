@echo off
setlocal EnableDelayedExpansion

REM
REM Created by MarcasRealAccount on 21. May. 2021
REM 

set "cleanQuiet=false"
call :HandleBatchArguments :HandleArg %*

set "cleanBatchDir=%~dp0"
call :DeleteDir !cleanBatchDir!\.vs\
call :DeleteDir !cleanBatchDir!\bin\
call :DeleteDir !cleanBatchDir!\bin-int\
call :DeleteFile !cleanBatchDir!\*.sln
call :DeleteFile !cleanBatchDir!\*.vcxproj
call :DeleteFile !cleanBatchDir!\*.vcxproj.*
call :DeleteFile !cleanBatchDir!\Makefile
call :DeleteFile !cleanBatchDir!\*.make
call :DeleteFile !cleanBatchDir!\*\Makefile
if "!cleanQuiet!"=="false" (
	echo Finished cleanup
)
exit /b 0

:DeleteDir
if "!cleanQuiet!"=="false" (
	echo Deleting "%*"
)
rmdir /s /q "%*" >nul 2>&1
exit /b 0

:DeleteFile
if "!cleanQuiet!"=="false" (
	echo Deleting "%*"
)
del /q "%*" >nul 2>&1
exit /b 0

:HandleArg
if "%*"=="/q" (
	set "cleanQuiet=true"
)
exit /b 0

:HandleBatchArguments
for /f "tokens=1,* delims= " %%a in ("%*") do (
	set "HandleBatchArguments_argLabel=%%a"
	set "HandleBatchArguments_str=%%b"
)
set "HandleBatchArguments_currentToken="
set "HandleBatchArguments_isInsideQuotes=false"

REM It was at this very moment I the creator of this script knew I fucked up

:HandleBatchArguments_loop
if [!HandleBatchArguments_str!]==[] goto HandleBatchArguments_done
set HandleBatchArguments_character=!HandleBatchArguments_str:~0,1!
if "%HandleBatchArguments_character:"=""%"=="""" (
	if "!HandleBatchArguments_isInsideQuotes!"=="false" (
		set "HandleBatchArguments_isInsideQuotes=true"
	) else (
		set "HandleBatchArguments_isInsideQuotes=false"
	)
) else (
	if "!HandleBatchArguments_character!"==" " (
		if "!HandleBatchArguments_isInsideQuotes!"=="false" (
			if not [!HandleBatchArguments_currentToken!]==[] (
				call !HandleBatchArguments_argLabel! !HandleBatchArguments_currentToken!
				set /A index=!index!+1
			)
			set "HandleBatchArguments_currentToken="
		) else (
			set "HandleBatchArguments_currentToken=!HandleBatchArguments_currentToken!!HandleBatchArguments_character!"
		)
	) else (
		set "HandleBatchArguments_currentToken=!HandleBatchArguments_currentToken!!HandleBatchArguments_character!"
	)
)
set HandleBatchArguments_str=!HandleBatchArguments_str:~1!
goto :HandleBatchArguments_loop
:HandleBatchArguments_done
if not [!HandleBatchArguments_currentToken!]==[] (
	call !HandleBatchArguments_argLabel! !HandleBatchArguments_currentToken!
)
exit /b !errorlevel!