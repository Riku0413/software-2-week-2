#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "obj.h"

int main(){
    Obj h1 = { .a = 1.0, .b = 'c', .c = 2.0 }; // double, char, double
    Obj h2 = h1;
    
    // before trick
    printf("\n(%f %f)\n",h1.a, h2.a);
    printf("(%f %f)\n",h1.c, h2.c);
    printf("(%c %c)\n\n",h1.b, h2.b);
    
    printf("%s\n", (!memcmp(&h1,&h2,sizeof(Obj)))?"true":"false");
    printf("%s\n", (!memcmp(&h1.a+1,&h2.a+1,sizeof(char)))?"true":"false");
    printf("%s\n\n", (!memcmp(&h1.a+1,&h2.a+1,sizeof(double)))?"true":"false");

    h2 = trick(h2); // h2 をいじる

    // after trick
    printf("(%f %f)\n",h1.a, h2.a);
    printf("(%f %f)\n",h1.c, h2.c);
    printf("(%c %c)\n\n",h1.b, h2.b);
    
    printf("%s\n", (!memcmp(&h1,&h2,sizeof(Obj)))?"true":"false");
    printf("%s\n", (!memcmp(&h1.a+1,&h2.a+1,sizeof(char)))?"true":"false");
    printf("%s\n\n", (!memcmp(&h1.a+1,&h2.a+1,sizeof(double)))?"true":"false");

    printf("size of Obj = %lu\n", sizeof(Obj));
    printf("size of double = %lu\n", sizeof(double));
    printf("size of char = %lu\n\n", sizeof(char));

    char *p1 = (char*)&h1; 
    char *p2 = (char*)&h2;
    for (int i = 0; i < sizeof(Obj); i++) {
        printf("p + %d : '%c' => '%c'\n", i, *p1, *p2);
        p1++;
        p2++;
    }
    printf("\n");

    return 0;
}
