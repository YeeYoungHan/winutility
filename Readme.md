# MS 윈도우용 유틸리티 프로그램 개발 프로젝트


### 소개

* 윈도우 도스창에서 편리하게 사용할 수 있는 유틸리티 프로그램 개발 

### 라이선스

* 본 프로젝트는 GPLv3 라이선스입니다. 

### 폴더 설명

* BackupDirectory
  * 폴더 백업 프로그램
  * 사용법
```
 dos> BackupDirectory.exe c:\Data d:\Data
```

* CleanDisk
  * 디스크의 남은 공간만큼 파일을 생성한 후, 삭제하는 프로그램
  * 디스크가 빈 상태에서 실행하면 디스크에 저장된 모든 데이터를 삭제할 수 있다.
  * 사용법
```
 dos> CleanDisk.exe d:\
```

* CopyFile
  * 파일에 지정된 부분만 복사하는 프로그램
  * 사용법
```
 dos> CopyFile.exe c:\temp\src.txt c:\temp\dest.txt 5 10
```

* DiffFile
  * 2개의 파일 내용이 동일한지 비교하는 프로그램
  * 사용법
```
dos> DiffFile.exe {first file path} {second file path}
```

* ExecTime
  * 도스 프로그램 실행 시간 측정 프로그램
  * 사용법
```
 dos> ExecTime.exe dir/w
```

* KillProcess
  * 도스창에서 프로세스 종료하는 프로그

* PrintDoc
  * 도스창에서 문서 인쇄하는 프로그램
  * 사용법
```
 dos> PrintDoc.exe c:\temp\dir.txt
```

* Publish
  * 본 프로젝트를 빌드한 release 프로그램

* RemoveBOM
  * 파일에서 BOM 헤더 제거하는 프로그램
  * 사용법
```
 dos> RemoveBOM.exe c:\temp\BOM.java c:\temp\clean.java
      RemoveBOM.exe c:\temp\BOM.java c:\temp\BOM.java
```

* RemoveSVN
  * 폴더에 포함된 모든 .svn 폴더를 제거하는 프로그램
  * 사용법
```
 dos> RemoveSVN.exe c:\temp\svn_folder
```

* RenameFiles
  * 폴더에 포함된 파일이름을 일괄 수정하는 프로그램

* ResizeImage
  * 하나의 이미지 파일의 크기를 변환하여서 새로운 이미지 파일에 저장하는 프로그램

* Tail
  * 윈도우 tail 프로그램

