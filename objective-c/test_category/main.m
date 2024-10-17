#import "Integer.h"
#import "Arithmetic.h"
#import "Display.h"

int main(void) {
    Integer* num1 = [Integer new], *num2 = [Integer new];
    int x;
    printf("Enter an integer:");
    scanf("%d", &x);
    num1.integer = x;
    [num1 showstars]

    printf("Enter an integer:");
    scanf("%d", &x);
    num2.integer = x;
    [num2 showstars]

    [num1 add:num2];
    [num1 showint];

    return 0;
}