#define NAMEMAX 12

struct card
{
    char usrname[NAMEMAX];
    // 카드를 발급 받아 사용할 소유자 이름.
    int cardnum;
    // 카드 번호.
    int usrnum;
    // 소유자의 주민번호
    int cardtype;
    // 발급받으려는 카드의 종류. (신용카드 / 체크카드)
    // 0이면 신용카드, 1이면 체크카드로 판단.
};