# MangaReader

## Notes for myself

### Generating the Windows installer

1. Build the project in Release mode (static build)

**The following commands should be executed from the main folder**

2. Update the version and release dates in `config\config.xml` and `packages\com.ariou.mangareader.root\meta\package.xml`.

3. Generate the archive containing the executable and `.o` files
  ```
  C:\Qt\QtIFW-4.0.1\bin\archivegen.exe packages\com.ariou.mangareader.root\data\MangaReader.7z ..\build-MangaReader-Stat
  ```

4. Generate the repository
  ```
  
  ```

### Windows installation

- One need to install openSSL and put the dll in system32 directory. Look at [this link](https://stackoverflow.com/questions/20351155/how-can-i-enable-ssl-in-qt-windows-application) for more details.

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

