@rem $Id: do-cm3-front.cmd,v 1.1 2007-12-31 10:09:31 jkrell Exp $

@echo off
setlocal
call %~dp0clearenv || exit /b 1
call %~dp0sysinfo || exit /b 1
call %~dp0pkgcmds || exit /b 1

call %~dp0pkgcmds map_action %* || exit /b 1

call %~dp0do-pkg %* m3core libm3 m3middle m3objfile m3linker m3back m3staloneback m3front m3quake cm3 mklib || exit /b 1

@echo %~n0 : Success.
