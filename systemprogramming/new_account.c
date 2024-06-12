#include "user.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "final.h"

void account()
{

    struct useraccount user;
    printf("------------회원가입 과정입니다.---------------\n");
    printf("ID를 입력하세요.\n");
    printf("영문과 숫자만 가능하며, 최대 15글자로 입력해주세요.\n");
    scanf("%s", user.ID);
    // ID 입력받아 구조체의 멤버에 저장하기.

    printf("Password를 입력하세요.\n");
    printf("영문과 숫자만 가능하며, 최대 20글자로 입력해주세요.\n");
    scanf("%s", user.PassWord);
    // Password 입력받아 구조체의 멤버에 저장하기.

    printf("%s", user.ID);
    // 회원 가입 완료 확인을 위해 id 출력하기.
}
