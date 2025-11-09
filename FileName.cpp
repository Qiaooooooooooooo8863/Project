#include<stdio.h>
int main() {
    int nums[10001] = { 0 };
    char ch;
    int i = 0;
    int n = 0;
    while (1) {
        scanf_s("%c%d", &ch, &nums[i]);
        i++;
        n++;
        if (ch == ']') {

            break;
        }
    }
    int ret = 0;
    for (int o = 0; o <= n; o++) {
        for (int p = 0; p <= n; p++) {
            int a = 0;
            int b = 0;
            if (nums[o] > nums[p]) {
                a = 1;
            }
            if (nums[o] < nums[p]) {
                b = 1;
            }
            if (a == 1 && b == 1) {
                ret += 1;
            }
        }
    }
    printf("%d", ret);




    return 0;
}