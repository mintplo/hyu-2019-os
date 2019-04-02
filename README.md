# [HYU] 2019_1학기_SCSC_OS_운영체제 실습

## 개발 환경 설정 (based on OSX)
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

## 실습 Overview
| 날짜   |      실습 내용      |
|----------|:-------------:|
| 2019-03-13 | ubuntu 16.04 LTS 설치 및 vim 기본 이용 |
| 2019-03-20 | 커널 3.x 버전 설치 (cscope, ctags)  |
| 2019-03-27 | 커널 5.x 버전 설치 및 user custom syscall 추가 |

## 실습
1. Ubuntu 16.04 LTS 에서 5.0.5 커널 빌드 및 설치시 Out of Memory, Disk Size 문제가 발생
디스크의 사이즈는 최소한 30GB 이상, 메모리는 2048MB로 설정하는 것을 권장

2. #include <asm/uaccess.h> 은 5.x 버전에서 <linux/uaccess.h> 로 사용해야 함
access_ok, copy_to_user 등 uaccess.h 에 정의된 함수를 사용하기 위해서는 5.x 버전에서 <linux/uaccess.h>로 include
