# sshd �α��� ���� ���̵� �� IP �ּ� ��� ����ϴ� ���α׷�

### �Ұ�

* sshd �α��� ���� ���̵� �� IP �ּ� ��� ����ϴ� ���α׷�

### ���̼���

* �� ������Ʈ�� GPLv3 ���̼����Դϴ�. 

### ����

* CentOS ���� �Ʒ��� ���� �����Ͽ��� sshd �α��� ���� �α� ������ �����Ѵ�.
```
# cd /var/log
# grep "Failed" secure* > /tmp/sshd_failed
```

* �Ʒ��� ���� �����ϸ� sshd �α��� ���� ���̵� TOP 10 �� sshd �α��� ���� IP �ּ� TOP 10 �� ����� �ش�.
```
dos> PrintSshdFailed.exe sshd_failed 10
```

