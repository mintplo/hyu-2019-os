## [HYU] 2019_1학기_SCSC_OS_운영체제 실습

## 개발 환경 설정 (based on OSX)
### Requirements
- [VirtualBox](https://www.vagrantup.com/)
- [Vagrant](https://www.virtualbox.org/)
- [Vagrant Disk Size Plugins](https://github.com/sprotheroe/vagrant-disksize)

위 Requirements를 자신의 운영체제(OS)에 맞게 설치

1. 프로젝트 루트 폴더 경로에 Vagrantfile을 자신에게 맞게 설정을 한다. [Vagrantfile 설정 가이드](https://www.vagrantup.com/docs/vagrantfile/)

VB Disk Size 설정을 위한 vagrant plugin 설치를 아래의 커맨드로 진행한다.
```
vagrant plugin install vagrant-disksize
```

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
| 2019-04-03 | Process Fork 기본 실습 + 커널 빌드 |
| 2019-04-10 | Prcoess Fork and Exec |
| 2019-04-17 | Pipeline + Signal |
| 2019-04-24 | 시험으로 인한 Skip |
| 2019-05-01 | IPC + Message Queue + Shared Memory |
| 2019-05-08 | Semaphore + Mutex |

## 실습
1. Ubuntu 16.04 LTS 에서 5.0.5 커널 빌드 및 설치시 Out of Memory, Disk Size 문제가 발생

디스크의 사이즈는 최소한 30GB 이상, 메모리는 2048MB로 설정하는 것을 권장

2. #include <asm/uaccess.h> 은 5.x 버전에서 <linux/uaccess.h> 로 사용해야 함

access_ok, copy_to_user 등 uaccess.h 에 정의된 함수를 사용하기 위해서는 5.x 버전에서 <linux/uaccess.h>로 include

## 프로젝트 #1

Semaphore, Mutex를 이용해 동시성과 교착 상태가 발생하는 [식사하는 철학자들 문제(Philosopher Dining Problem)](https://ko.wikipedia.org/wiki/%EC%8B%9D%EC%82%AC%ED%95%98%EB%8A%94_%EC%B2%A0%ED%95%99%EC%9E%90%EB%93%A4_%EB%AC%B8%EC%A0%9C)를 해결.

### 문제

`N명의 철학자`가 `원형 탁자`에 앉아있다고 가정. 각자의 앞에는 `음식`이 있고 `젓가락`이 하나씩 존재. 철학자는 `음식`을 먹기 위해서 `양 옆의 젓가락`을 동시에 들어야 함. 각 철학자가 자신의 왼쪽에 있는 젓가락을 먼저 들고 그 다음 자신의 오른쪽에 있는 젓가락을 들고 음식을 먹으려 한다면 N명의 철학자 모두가 동시에 자신의 왼쪽 젓가락을 들 경우에 `(동시성)` 자신의 오른쪽 젓가락을 무한히 기다려야 하는 상황이 발생할 수 있음. `(교착상태)`

동시에 양쪽 젓가락을 집을 수 없어 식사를 하지 못하는 기아 상태가 발생할 수도 있고, 몇몇 철학자가 다른 철학자보다 식사를 적게 하는 경우가 발생하기도 한다.

### 해결

다익스트라에 의해 고안된 `Semaphore` 방법을 이용하여 `젓가락`을 공유하는 변수로 보고 `Semaphore 변수`로 적용. `N-1명의 철학자`는 자신의 왼쪽 젓가락을 먼저 집도록(확인하도록) 하고 `N번째 철학자`는 자신의 오른쪽 젓가락을 먼저 집도록(확인하도록) 하는 방법을 적용. 이럴 경우 모두가 동시에 자신의 젓가락을 집어도 `(동시성)` 자신의 다른 쪽 젓가락을 무한히 기다려야 하는 상황은 발생하지 않음. `(교착상태 Solved)`

## 프로젝트 #2

CPU 스케쥴러의 FCFS, non-preempt SJF, RR, PS를 Simulation 하기.

### FCFS (First Come, First Served)

- 가장 단순한 스케줄링
- CPU 요청 순서대로 할당
- 먼저 사용신청을 한 프로세스부터 차례로 CPU를 할당함
- 비선점 스케줄링 중 한가지

### Shortest Job First (SJF) non-preempt

- 버스트 시간이 짧은 프로세스부터 CPU 할당
- 버스트 시간이 짧은 것을 우선 처리하는 스케줄링 방식
- 버스트 시간이 짧을수록 우선순위를 높게 책정하는 우선순위 스케줄링
- 버스트 시간이 같으면 FCFS 스케줄링
- 비선점형, 선점형 모두 가능

### Round Robin (RR)

- 선점형 스케줄링의 하나
- 시분할 시스템을 위해 설계됨
- 시간 할당량(time quantum) 단위로 CPU를 할당
- 일반적으로 10 ~ 100 ms
- 알고리즘의 성능은 시간 할당량의 크기에 좌우됨
- 시간할당량이 매우 크면 FCFS 스케줄링과 같아짐
- 시간할당량이 매우 작으면 프로세서 공유와 같아짐

### Priority Scheduling (PS)

- 우선순위가 높은 프로세스에 CPU를 우선 할당하는 스케줄링 방식
- 우선순위가 같으면 FCFS 스케줄링
- 우선순위는 내부적/외부적 조건에 따라 정의될 수 있음
- 시간 제한, 메모리 요구량, 열린 파일의 수, 프로세스의 중요성, 자원사용 비용 등
- 예를 들어 SJF 스케줄링은 버스트 시간이 짧을수록 우선순위를 높게 책정하는 경우라 할 수 있음
- 선점형, 비선점형 모두 가능
