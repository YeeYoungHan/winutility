# 한국어, 영어 텍스트를 wav 파일로 저장하는 프로그램

### 소개

* MS 윈도우 Speech API 를 이용한 TTS 프로그램입니다.
* 한국어, 영어 텍스트와 출력 wav 파일 경로를 입력하여서 실행하면 한국어, 영어 텍스트를 음원으로 생성하여서 wav 파일에 저장해 줍니다.

### 라이선스

* 본 프로젝트는 GPLv3 라이선스입니다. 

### 사용법

```
dos> TTS.exe {voice} {text} {wave file}

dos> TTS.exe -l 
TTS_MS_KO-KR_HEAMI_11.0
TTS_MS_EN-US_ZIRA_11.0

dos> TTS.exe TTS_MS_EN-US_ZIRA_11.0 "Hello World" c:\temp\test.wav
```



