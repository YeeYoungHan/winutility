# MS 윈도우용 유틸리티 프로그램 개발 프로젝트


### 소개

* 윈도우 도스창에서 편리하게 사용할 수 있는 유틸리티 프로그램 개발 

### 라이선스

* 본 프로젝트는 GPLv3 라이선스입니다. 
* 본 프로젝트에 대한 상용 라이선스 발급을 원하시면 websearch@naver.com 로 연락해 주세요.

### 폴더 설명

* AddBOM
  * utf8 BOM 을 추가하는 프로그

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

* CropImage
  * 이미지 파일들을 편리하게 커팅할 수 있는 프로그램
  * https://blog.naver.com/websearch/221512614384

* DiffFile
  * 2개의 파일 내용이 동일한지 비교하는 프로그램
  * 사용법
```
dos> DiffFile.exe {first file path} {second file path}
```

* DosCropImage
  * 도스용 이미지 커팅 프로그램
```
dos> DosCropImage.exe {input image filename} {output image filename} {x} {y} {width} {height}
```

* ExecTime
  * 도스 프로그램 실행 시간 측정 프로그램
  * 사용법
```
 dos> ExecTime.exe dir/w
```

* KillProcess
  * 도스창에서 프로세스 종료하는 프로그램

* LoadDll
  * dll 이 로드 가능한지 확인하는 프로그

* PcapSend
  * pcap 파일을 읽어서 네트워트로 전송하는 프로그램

* PcapToText
  * pcap 파일을 텍스트 파일에 저장하는 프로그램

* PcapViewer
  * pcap 파일을 읽어서 GUI 로 보여주고 패킷 순서를 변경할 수 있는 프로그램

* PrintDoc
  * 도스창에서 문서 인쇄하는 프로그램
  * 사용법
```
 dos> PrintDoc.exe c:\temp\dir.txt
```

* PrintExtList
  * 도스창에서 지정된 폴더에 포함된 파일들을 확장자별로 개수 출력하는 프로그램

```
 dos> PrintExtList.exe {folder path}
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

* RemoveEmptyDirectory
  * 빈 폴더들을 삭제하는 프로그

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

* ShowTransparentImage
  * 투명한 png 이미지 확인하는 프로그램

* SourceCodeLine
  * 폴더에 포함된 모든 소스 코드의 라인 개수를 출력하는 프로그

* Tail
  * 윈도우 tail 프로그램

* ThumbnailImage
  * thumbnail 이미지 생성 프로그램

* TTS
  * 텍스트를 wav 파일로 생성하는 프로그

* Utf8ToAnsi
  * 폴더에 존재하는 모든 파일들을 UTF8 에서 ANSI 로 변환하는 프로그램

