# 파일 비교 프로그램

### 소개

* 윈도우 도스창에서 2개 파일의 내용이 동일한지 비교하는 프로그램 

### 라이선스

* 본 프로젝트는 GPLv3 라이선스입니다. 

### 사용법

```
dos> DiffFile.exe {first file path} {second file path}
```

### 실행 결과

```
dos>DiffFile.exe DiffFile.exe DiffFile.exe
Same file

dos>DiffFile.exe DiffFile.exe RemoveBOM.exe
files DiffFile.exe and RemoveBOM.exe differ
```

