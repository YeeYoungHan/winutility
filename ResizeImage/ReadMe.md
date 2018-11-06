# 이미지 크기 변환 프로그램

### 소개

* 윈도우 도스창에서 이미지 크기를 변환하는 프로그램 

### 라이선스

* 본 프로젝트는 GPLv3 라이선스입니다. 

### 사용법

```
ResizeImage.exe {input image filename} {output image filename} {output image width} {output image height}

dos> ResizeImage.exe input.jpg output.jpg 640 480

지정된 폴더에 포함된 모든 이미지 파일의 크기를 수정하고 싶으면 아래와 같이 실행하시면 됩니다.

dos> ResizeImage.exe -d c:\temp\test 640 480
```
