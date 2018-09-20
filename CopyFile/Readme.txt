파일에 지정된 부분만 복사하는 프로그램

1. 소개

 * 윈도우 도스창에서 파일에 지정된 부분만 복사하는 프로그램 

2. 라이선스

 * 본 프로젝트는 GPLv3 라이선스입니다. 

3. 사용법

 dos> CopyFile.exe {src file} {dest file} {start pos} {end pos}

 src file  : 원본 파일 full path
 dest file : 저장 파일 full path
 start pos : 원본 파일에서 복사를 시작할 위치 ( byte 단위 )
 end pos   : 원본 파일에서 복사를 중지할 위치 ( byte 단위 )
  - end pos 를 설정하지 않으면 start pos 부터 파일 끝까지 복사한다.


