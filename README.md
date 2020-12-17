# MangaReader

## Notes for myself

### Generating the Windows installer

1. Build the project in Release mode (static build)

**The following commands should be executed from the main folder**

2. Update the version and release dates in `config\config.xml` and `packages\com.ariou.mangareader.root\meta\package.xml`.

3. Generate the archive containing the executable and `.o` files
  ```
  C:\Qt\QtIFW-4.0.1\bin\archivegen.exe packages\com.ariou.mangareader.root\data\MangaReader.7z ..\build-MangaReader-Static_Qt-Release\release
  ```

4. Generate the repository
  ```
  C:\Qt\QtIFW-4.0.1\bin\repogen.exe --update -p packages release
  ```

5. Generate the online installer
  ```
  C:\Qt\QtIFW-4.0.1\bin\binarycreator.exe -n -c config\config.xml -p packages MangaReader-setup.exe
  ```

## TODOs

### Priority A

- Create setup for all platforms with following functionalities:
  - automatic check for updates
  - add desktop shortcut

### Priority B

- Enable user to set reading direction

- Solve bug background disappear first library opening

- restore current manga/page when close and reopen app

- enable close tabs (except first)


### Priority C



### Priority D

