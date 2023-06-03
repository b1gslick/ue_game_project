@echo off
call "%~dp0devops\config.bat"

devops\misc\generate_project_file.bat "%CurrentEnginePath%" "%UBTRelativePath5%" "%VersionSelector%" "%ProjectName%"