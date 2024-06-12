#define IDMAX 15
#define PWMAX 20

struct useraccount
{
    char ID[IDMAX];
    // 사용자가 login할 때의 ID
    char PassWord[PWMAX];
    // 사용자가 login할 때의 Password
};