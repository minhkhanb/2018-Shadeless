@echo off
call config.bat


echo *** Creating directories ***
mkdir %TMPRES%
mkdir %BIN%


copy  %DESIGN_FILE% %DESIGN_EXPORT_DIR%\data\Objects.xls
copy %DESIGN_FILE_EXPORT% %DESIGN_EXPORT_DIR%\data\GDS_Exporter.conf
copy %SRC_FILE_H% %DESIGN_EXPORT_DIR%\Src\GDS_Exporter.java

copy %PRJ_DIR%\doc\overall_design.xls   %DESIGN_EXPORT_DIR%\data\overall_design.xls
copy %PRJ_DIR%\doc\overall_design.xls   %DESIGN_EXPORT_DIR%\data\overall_design.xls
copy %PRJ_DIR%\doc\overall_design.xls   %DESIGN_EXPORT_DIR%\data\overall_design.xls
copy %PRJ_DIR%\doc\overall_design.xls   %DESIGN_EXPORT_DIR%\data\overall_design.xls
copy %PRJ_DIR%\doc\overall_design.xls   %DESIGN_EXPORT_DIR%\data\overall_design.xls

copy %PRJ_DIR%\doc\ExportDesign\run.bat   %DESIGN_EXPORT_DIR%\run.bat
copy %PRJ_DIR%\doc\ExportDesign\config.bat   %DESIGN_EXPORT_DIR%\config.bat

cd %DESIGN_EXPORT_DIR%
call make.bat
call run.bat

cd %DESIGN_EXPORT_DIR%\release
echo %DESIGN_EXPORT_DIR%\release
copy *.h %OBJECT_RELEASE_DIR%
copy *.cpp %OBJECT_RELEASE_DIR%



echo *** Building Data Complete ***

pause