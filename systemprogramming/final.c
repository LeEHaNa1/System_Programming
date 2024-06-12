#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include "final.h"
#include "card.h"

#define MAX_BUF 256

/* 학부 : 컴퓨터공학부    학번 : 202202854     이름 : 이하나

   프로젝트 주제: 간단한 카드 발급 시스템

   1. 기능

   1) 회원 가입 진행하기.
   2) 카드 발급하기.

   2. 기능에 대한 설명

   1) 회원 가입 여부를 묻고, 회원 가입을 하지 않았다면, 회원 가입을 진행한다.
      회원 가입은 간단히 id와 password를 입력 및 저장하는 과정으로 진행된다.

    2) 카드 발급을 위해, 사용자의 개인 정보를 입력받는다.
       이름 및 개인 정보를 입력받고, 카드 번호는 회사 쪽에서(해당 파일에서) 발급한다.
       카드 발급이 완료되면, 간단한 카드 발급 정보에 대한 출력이 이루어진다.


*/

void sig_handler(int signo)
{
    exit(0);
    // SIGINT가 확인되면, 프로세스 종료.
}
int main()
{
    pid_t pid;
    int shmid;
    int child, status;
    char user;
    time_t tloc;
    char *shmaddr, *shmaddr2;
    struct card newcard;
    int fd1[2], fd2[2];
    char buf[MAX_BUF] = {
        0,
    };
    char cardnumm[MAX_BUF];

    void (*exit_signal)(int);

    exit_signal = signal(SIGINT, sig_handler);
    // 실행 도중에 SIGINT가 확인되었다면, sig_handler라는 함수로 처리한다.

    shmid = shmget(IPC_PRIVATE, 2, IPC_CREAT | 0644);
    // 공유 메모리 할당을 위한 식별자 생성.

    if (shmid == -1)
    {
        perror("shmget");
        exit(1);
    }
    // 식별자 생성에서 문제가 생겼을 때에는 오류 메시지 출력 및 프로세스 종료.

    if (exit_signal == SIG_ERR)
    {
        perror("시스템 종료: ");
        exit(1);
    }

    if (pipe(fd1) == -1)
    {
        perror("make pipe");
        exit(1);
    } // 양방향 통신을 위한 파이프 1 만들기.

    if (pipe(fd2) == -1)
    {
        perror("make pipe");
        exit(1);
    } // 양방향 통신을 위한 파이프 2 만들기.

    switch (pid = fork())
    {
    case -1:
        perror("fork");
        exit(1);
        break;
    case 0:
        shmaddr = (char *)shmat(shmid, (char *)NULL, 0);
        // 공유 메모리 사용을 위한 주소 연결.
        close(fd1[1]);
        // 읽기.
        close(fd2[0]);
        // 쓰기.
        write(1, "From HUFS CARD\n", 16);
        read(fd1[0], buf, MAX_BUF);
        write(1, buf, strlen(buf) + 1);
        scanf("%c", &user);
        // 회원 가입 여부에 대한 대답을 user에 저장하기.(1-> 완료. 2 -> 미완료.)
        shmaddr[0] = user;
        // 공유 메모리에 해당 값을 저장한다.
        shmdt((char *)shmaddr);
        // 공유 메모리 해제하기.
        exit(0);
        // 자식 프로세스 종료.
        break;
    default:
        close(fd1[0]);
        // 쓰기.
        close(fd2[1]);
        // 읽기.
        write(fd1[1], "회원 가입을 완료하셨습니까?\n완료 = 1, 미완료 = 2\n", 80);
        // 회원 가입 여부 묻기.
        child = wait(&status);
        // 자식 프로세스가 종료될 때까지 기다리기.
        shmaddr2 = (char *)shmat(shmid, (char *)NULL, 0);
        // 공유 메모리 사용을 위한 메모리 주소 연결.
        user = shmaddr2[0];
        // 회원 가입 여부에 대해 물었기 때문에, 대답을 듣는다.
        shmdt((char *)shmaddr2);
        shmctl(shmid, IPC_RMID, (struct shmid_ds *)NULL);
        // 해당 공유 메모리 해제 및 관련 데이터 삭제.
        if (user == '2')
        { //  공유 메모리에서 읽어온 값이 '2'여서 미완료를 의미한다면, 회원가입을 담당하는 final.h으로 이동.
            // final.h에 선언되어 있는 함수를 사용하기 위해 new_account.c로 이동.
            account();
            // 회원 가입 과정 진행하기.
            printf(" 회원님, 가입이 완료되었습니다.\n");

            printf("------------------------\n");
        }

        printf("카드 발급 단계로 이동합니다...\n");
        printf("----------카드 발급 과정입니다.---------\n");
        printf("성함을 입력해주세요.\n");
        scanf("%s", newcard.usrname);
        printf("고객님의 주민번호를 - 없이 입력해주세요.\n");
        scanf("%d", &newcard.usrnum);
        printf("발급받고자 하는 카드가 신용카드라면 0 체크카드라면 1을 입력해주세요.\n");
        scanf("%d", &newcard.cardtype);
        // 카드 발급을 위한 기본 정보 사용자에게서 받기.
        time(&tloc);
        newcard.cardnum = pid + (int)tloc;
        // 카드 번호는 회사 측에서 설정.
        // 최대한 겹치지 않도록, pid에 현재 시각 정보를 더해 카드 번호 발급하기.
        printf("--------------카드 발급 정보-----------\n");
        if (newcard.cardtype == 1)
        {
            strcpy(cardnumm, "체크카드");
        }
        else
        {
            strcpy(cardnumm, "신용카드");
        }
        // 한 줄로 카드 발급 정보를 출력하기 위해서 cardnumm에 문자열 복사하기.
        printf("%s 고객님의 %s의 카드 번호는 %d입니다.\n", newcard.usrname, cardnumm, newcard.cardnum);
        printf("카드 발급이 완료되었습니다. 감사합니다.\n");
        if (execl("/bin/echo", "echo", "Making Card System is completed", (char *)0) == -1)
        {
            // exec군 함수를 활용하여 echo 한 후, 프로세스 종료하기.
            perror("execl");
            exit(1);
        }
        break;
    }
}