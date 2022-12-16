#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "obj.h"

int main(){
    Obj h1 = { .a = 1.0, .b = 'c', .c = 2.0 }; // double, char, double
    Obj h2 = h1;
    
    // before trick
    printf("(%f %f)\n",h1.a, h2.a);
    printf("(%f %f)\n",h1.c, h2.c);
    printf("(%c %c)\n",h1.b, h2.b);
    
    printf("%s\n", (!memcmp(&h1,&h2,sizeof(Obj)))?"true":"false");
    printf("%s\n", (!memcmp(&h1.a+1,&h2.a+1,sizeof(char)))?"true":"false");
    printf("%s\n", (!memcmp(&h1.a+1,&h2.a+1,sizeof(double)))?"true":"false");

    //
    // // double* p;
    // // p = (double*) 0x16dc1b1ef;
    // printf("\n%p : %lf\n", &h1.a, *(&h1.a));
    // // printf("%p\n", p);
    // // printf("\n%p : %f\n\n", &h1, *(&h1));
    // printf("%p : %c\n", &h1.a + 1, *(&h1.a + 1));
    // printf("%p : %lf\n\n", &h1.a + 2, *(&h1.a + 2));
    //

    h2 = trick(h2); // h2 をいじる

    // after trick
    printf("(%f %f)\n",h1.a, h2.a);
    printf("(%f %f)\n",h1.c, h2.c);
    printf("(%c %c)\n",h1.b, h2.b);
    
    printf("%s\n", (!memcmp(&h1,&h2,sizeof(Obj)))?"true":"false");
    printf("%s\n", (!memcmp(&h1.a+1,&h2.a+1,sizeof(char)))?"true":"false");
    printf("%s\n", (!memcmp(&h1.a+1,&h2.a+1,sizeof(double)))?"true":"false");

    printf("\nsize of Obj = %lu\n", sizeof(Obj));
    printf("size of double = %lu\n", sizeof(double));
    printf("size of char = %lu\n\n", sizeof(char));



    // int ad1_a = (int) &h1.a;
    // int ad1_b = (int) &h1.b;
    // int ad1_c = (int) &h1.c;

    // printf("%p\n", &h1.a);
    // printf("%p\n", &h1.b);
    // printf("%p\n\n", &h1.c);

    // printf("%d\n", ad1_b - ad1_a);
    // printf("%d\n\n", ad1_c - ad1_b);



    // int ad2_a = (int) &h2.a;
    // int ad2_b = (int) &h2.b;
    // int ad2_c = (int) &h2.c;

    // printf("%p\n", &h2.a);
    // printf("%p\n", &h2.b);
    // printf("%p\n\n", &h2.c);
    
    // printf("%d\n", ad2_b - ad2_a);
    // printf("%d\n\n", ad2_c - ad2_b);

    return 0;
}
