# sshd 로그인 실패 아이디 및 IP 주소 통계 출력하는 프로그램

### 소개

* sshd 로그인 실패 아이디 및 IP 주소 통계 출력하는 프로그램

### 라이선스

* 본 프로젝트는 GPLv3 라이선스입니다. 

### 사용법

* CentOS 에서 아래와 같이 실행하여서 sshd 로그인 실패 로그 파일을 생성한다.
```
# cd /var/log
# grep "Failed" secure* > /tmp/sshd_failed
```

* 아래와 같이 실행하면 sshd 로그인 실패 아이디 TOP 10 과 sshd 로그인 실패 IP 주소 TOP 10 을 출력해 준다.
```
dos> PrintSshdFailed.exe sshd_failed 10
```

