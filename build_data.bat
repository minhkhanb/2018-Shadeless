@echo off
call config.bat

echo *** Creating directories ***
mkdir %TMPRES%
mkdir %BIN%
mkdir %RES_DES%

cd %TOOLS_PATH%

call copy_files.bat %RES% %TMPRES%
cd %TMPRES%
rem pack text

rem pack animation
for %%i in (*.pine) do (
	%ANIM_EXPORT% 	-i %%i		-o %%~ni.bpine  QUALITY_TRUE_COLOR	EXPORT_AS_3D_TEXTURE	EXPORT_FMODULE_OPACITY USE_MULTI_MODULE_TYPE EXPORT_PALETTE_COLOR
)

for %%i in (*.bat) do (
	call %%i
)


copy *.h %SRC_DIR_H%

cd %PRJ_DIR%

echo *** Building Data Complete ***

pause