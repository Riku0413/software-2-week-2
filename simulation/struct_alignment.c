#include <stdio.h>

typedef struct student {
    int id;
    char name[100];
    int age;
    double height;
    double weight;
} Student;

typedef struct tagged_student1 {
    int id;
    char name[100];
    int age;
    double height;
    double weight;
    char tag;
} TStudent1;

typedef struct tagged_student2 {
    char tag;
    int id;
    char name[100];
    int age;
    double height;
    double weight;
} TStudent2;

int main (int argc, char**argv){
    Student s_begin;
    Student s1;
    TStudent1 s2;
    TStudent2 s3;
    TStudent2 s_end;

    /* 以下に各構造体の先頭アドレスを表示するプログラムを書く */
    /* Hint: printf で %p フォーマットを指定する*/
    /* 逆順に表示（s_end, s3, s2, s1, s_begin の順）*/

    printf("\n(%p, %p, %p, %p, %p)\n", &s_end, &s3, &s2, &s1, &s_begin);

    /* 以下には各構造体のサイズをsizeof演算子で計算し、表示する */
    /* printf 表示には%zu を用いる*/

    printf("\n(%zu, %zu, %zu, %zu, %zu)\n", sizeof(s_end), sizeof(s3), sizeof(s2), sizeof(s1), sizeof(s_begin));

    printf("\n%zu : size of s1\n", sizeof(s1));
    printf("%zu : size of s1.id\n", sizeof(s1.id));
    printf("%zu : size of s1.name\n", sizeof(s1.name));
    printf("%zu : size of s1.age\n", sizeof(s1.age));
    printf("%zu : size of s1.height\n", sizeof(s1.height));
    printf("%zu : size of s1.weight\n", sizeof(s1.weight));
    printf("\n%p : s1.id\n", &s1.id);
    printf("%p : s1.name[0]\n%p : s1.name[99]\n", &s1.name[0], &s1.name[99]);
    printf("%p : s1.age\n%p : s1.height\n%p : s1.weight\n\n", &s1.age, &s1.height, &s1.weight);

    int ad1_id = (int)&s1.id;
    int ad1_name0 = (int)&s1.name[0]; 
    int ad1_age = (int)&s1.age;
    int ad1_height = (int)&s1.height;
    int ad1_weight = (int)&s1.weight;

    printf("\nid : %d\n", ad1_name0 - ad1_id);
    printf("name : %d\n", ad1_age - ad1_name0);
    printf("age : %d\n", ad1_height - ad1_age);
    printf("height : %d\n", ad1_weight - ad1_height);
    printf("weight : 8?\n\n");



    // s2
    printf("\n%zu : size of s2\n", sizeof(s2));
    printf("%zu : size of s2.id\n", sizeof(s2.id));
    printf("%zu : size of s2.name\n", sizeof(s2.name));
    printf("%zu : size of s2.age\n", sizeof(s2.age));
    printf("%zu : size of s2.height\n", sizeof(s2.height));
    printf("%zu : size of s2.weight\n", sizeof(s2.weight));
    printf("%zu : size of s2.tag\n", sizeof(s2.tag));
    printf("\n%p : s2.id\n", &s2.id);
    printf("%p : s2.name[0]\n%p : s2.name[99]\n", &s2.name[0], &s2.name[99]);
    printf("%p : s2.age\n%p : s2.height\n%p : s2.weight\n%p : s2.tag\n", &s2.age, &s2.height, &s2.weight, &s2.tag);

    int ad_id = (int)&s2.id;
    int ad_name0 = (int)&s2.name[0]; 
    int ad_age = (int)&s2.age;
    int ad_height = (int)&s2.height;
    int ad_weight = (int)&s2.weight;
    int ad_tag = (int)&s2.tag;

    printf("\nid : %d\n", ad_name0 - ad_id);
    printf("name : %d\n", ad_age - ad_name0);
    printf("age : %d\n", ad_height - ad_age);
    printf("height : %d\n", ad_weight - ad_height);
    printf("weight : %d\n", ad_tag - ad_weight);
    printf("tag : 8?\n\n");



    // s3
    printf("\n%zu : size of s3\n", sizeof(s3));
    printf("%zu : size of s3.tag\n", sizeof(s3.tag));
    printf("%zu : size of s3.id\n", sizeof(s3.id));
    printf("%zu : size of s3.name\n", sizeof(s3.name));
    printf("%zu : size of s3.age\n", sizeof(s3.age));
    printf("%zu : size of s3.height\n", sizeof(s3.height));
    printf("%zu : size of s3.weight\n", sizeof(s3.weight));
    printf("\n%p : s3.tag\n%p : s3.id\n", &s3.tag, &s3.id);
    printf("%p : s3.name[0]\n%p : s3.name[99]\n", &s3.name[0], &s3.name[99]);
    printf("%p : s3.age\n%p : s3.height\n%p : s3.weight\n", &s3.age, &s3.height, &s3.weight);

    int ad3_tag = (int)&s3.tag;
    int ad3_id = (int)&s3.id;
    int ad3_name0 = (int)&s3.name[0]; 
    int ad3_age = (int)&s3.age;
    int ad3_height = (int)&s3.height;
    int ad3_weight = (int)&s3.weight;

    printf("\ntag : %d\n", ad3_id - ad3_tag);
    printf("id : %d\n", ad3_name0 - ad3_id);
    printf("name : %d\n", ad3_age - ad3_name0);
    printf("age : %d\n", ad3_height - ad3_age);
    printf("height : %d\n", ad3_weight - ad3_height);
    printf("weight : 8?\n\n");

    return 0;
}
