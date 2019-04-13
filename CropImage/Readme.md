# 이미지 파일들을 커팅하 프로그램

### 소개
본 프로그램은 설정된 커팅 이미지 크기로 N 개의 파일을 편리하게 커팅하여서 저장할 수 있는 프로그램으로 아래와 같은 기능이 포함되어 있습니다.

* 커팅하여서 저장할 이미지 크기를 설정 기능
* 커팅하여서 저장한 이미지의 thumbnail 이미지 생성하는 기능
* 원본 이미지 위에 커팅될 이미지 위치와 크기를 보여주는 기능

### 라이선스

* 본 프로젝트는 GPLv3 라이선스입니다. 

### 사용법
사용법은 아래의 포스트를 참고해 주세요.

* https://blog.naver.com/websearch/221512614384

### 실행화면
실행화면에서는 이미지가 다소 어색하게 보이지만 커팅된 이미지는 정상적으로 보여집니다.

![Alt text](https://postfiles.pstatic.net/MjAxOTA0MTNfMjM4/MDAxNTU1MTI0MjQ4MDY1.LK61A1zmyUkvu_3sSXSpNQFewyoNoMJpiutQ5O0JUfUg.HgVkh1mkV80CAPgWND9bX-mYUF5jHhDdXNlOPwWIM1Ug.PNG.websearch/CropImage-2.png?type=w773)
![Alt text](https://postfiles.pstatic.net/MjAxOTA0MTNfMTQ3/MDAxNTU1MTI0NDMzNzU5.OKm3HpIAD1mdPhmmHY44BWP--1QajBRFrSv2Z14rQWYg._6ZGR-ZhQIUnGvmAIHpEFbNT_dPUIsktnULEREM1bCYg.PNG.websearch/CropImage-3.png?type=w773)

### 알려진 문제

* MFC CImage 클래스의 문제로 가로 < 세로인 이미지는 90도 회전해서 보여지는 현상이 있습니다.
