#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

void fun(int n) {
    printf("%d", n);
}

int main() {
    int n;
    scanf("%d", &n);
    fun(n);
    if (n > 1) {
        n =  100;
    } else {
        n = 1;
    }
    if (n) {
        fun(1);
    }
    char buf[26];
    buf[0] = 'a' + n;
    int x = send(1, buf, 2, 0);
}