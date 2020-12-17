# MangaReader

This software is a manga e-reader. The following features are implemented:

- Automatic download of last chapters for offline reading
- Fullscreen and double page displaying for comfortable reading

Feel free to contact me if you face bugs or issues.

## Notes for myself

### Generating the Windows installer

1. Build the application in Release mode

**The following commands should be executed from the main folder**

2. Update the version and release dates in `config\windows.xml` and `packages\com.ariou.mangareader.root\meta\package.xml`.

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

- Enable user to set reading direction

- Solve bug background disappear first library opening

- restore current manga/page when close and reopen app

- enable close tabs (except first)



