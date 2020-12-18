SET currentOS=windows
SET QtIFWDir=C:\Qt\QtIFW-4.0.1\bin
SET configFile=config\%currentOS%.xml
SET packagesDir=packages
SET archiveFile=%packagesDir%\com.ariou.mangareader.root\data\MangaReader.7z
SET binariesDir=..\build-MangaReader-Static_Qt-Release\release
SET repoDir=releases\%currentOS%
SET installerDir=setups

ECHO "This script will create the MangaReader installer for %currentOS%."

PAUSE

cd ..
rm %archiveFile%
%QtIFWDir%\archivegen.exe %archiveFile% %binariesDir%
%QtIFWDir%\repogen.exe --update -p %packagesDir% %repoDir%
%QtIFWDir%\binarycreator.exe -n -c %configFile% -p %packagesDir% %installerDir%\MangaReader-%currentOS%-setup.exe
git add -A
git commit -m "upload new release"
git push

PAUSE