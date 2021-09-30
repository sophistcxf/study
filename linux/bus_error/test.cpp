#include <string.h>
#include <stdio.h>
int main(){
    char buf[256];
    int *x;
    x = (int *) & buf[1];
    //强制类型转换的是指针而非char型变量，指针指向的内存数据可以认为是任意类型，char、int 、float、double甚至结构    
    *x = 68000;
    //假设buf的开始地址为0x1000，则这句将0x1001～0x1004的内容拷贝给x（x为int型）,如前所述，int型数据地址必须为4的整数倍，此处0x1001不满足该条件，故会引起CPU内部总线错误    
    printf("*x = %d\n",*x);
    return 0;
}
