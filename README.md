# [HYU] 2019_1학기_SCSC OS_운영체제 실습

## 개발 환경 설정
### Requirements
- [VirtualBox](https://www.vagrantup.com/)
- [Vagrant](https://www.virtualbox.org/)

위 Requirements를 자신의 운영체제(OS)에 맞게 설치

1. 프로젝트 루트 폴더 경로에 Vagrantfile을 자신에게 맞게 설정을 한다. [Vagrantfile 설정 가이드](https://www.vagrantup.com/docs/vagrantfile/)
2. 아래의 코드를 실행하여 VM을 생성한다.

```
cd YourProjectRootFolder
vagrant up
```

3. 아래의 명령어로 SSH를 통해 해당 VM 터미널에 접속

```
vagrant ssh
```

4. 아래의 명령어로 VM 실행 중지, VM 제거가 가능하다.

```
vagrant halt (shutdown)
vagrant destroy (remove virtual machine)
```

## 실습 정리 (표)
| 날짜   |      실습 내용      |
|----------|:-------------:|
| 2019-03-13 | ubuntu 16.04 LTS 설치 및 vim 기본 이용 |
| 2019-03-20 | 커널 3.x 버전 설치 (cscope, ctags)  |
| 2019-03-27 | 예정 |
