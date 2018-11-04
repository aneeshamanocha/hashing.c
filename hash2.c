#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define DEFAULT_SIZE 3

typedef struct node {
    char *key;
    char *val;
    size_t hash;
    struct node *next;
} Node;

typedef struct hashmap {
    Node **arr;
    size_t capacity;
    size_t size;
    int resizes;
    int collisions;
} Hashmap;

size_t oldHash(char *string);
size_t hash(char *string);
size_t insert(Hashmap *hm, char *key, char *val);
Hashmap *init();
void resize(Hashmap *hm);
bool isPrime(size_t n);
unsigned int power(unsigned int a, unsigned int b, unsigned int mod);
void print(Hashmap *hm);

int main(int argc, char *argv[]) {
    //Node *arr = malloc(sizeof(Node) * 100);

    //printf("%zu\n", hash("abc"));
    //
    /*
    for(int i = 3; i <= 100000; i++) {

        printf("%d is prime: %d\n", i, isPrime(i));
    }*/

    Hashmap *hm = init();
    char tmp[3];
    tmp[2] = '\0';
    for(int i = 'A'; i <= 'z'; ++i) {
        for(int j = 'A'; j <= 'z'; ++j) {
            tmp[0] = (char)i;
            tmp[1] = (char)j;
            insert(hm, tmp, tmp);
        }
    }
    insert(hm, "a", "a");
    insert(hm, "b", "b");
    insert(hm, "c", "c");
    insert(hm, "d", "d");
    insert(hm, "e", "e");
    insert(hm, "f", "f");
    insert(hm, "g", "g");
    insert(hm, "h", "h");
    insert(hm, "i", "i");
    insert(hm, "j", "j");
    print(hm);
    printf("R: %d, C: %d\n", hm->resizes, hm->collisions);
    return 0;
}

size_t oldHash(char *string) {
    int num = 31;
    size_t out = 0;
    for(size_t i = 0; string[i] != '\0'; i++) {
        for(size_t k = 0; k <= i; k++) {
            out += (size_t) pow(num, i-k) * string[k];
        }
    }
    return out;
}

size_t hash(char *string) {
    int num = 31;
    size_t out = 0, tmp = 0;
    for(size_t i = 0; string[i] != '\0'; i++) {
        tmp = string[i] + tmp * num;
        out += tmp;
    }
    return out;
}

size_t insert(Hashmap *hm, char *key, char *val) {
    resize(hm);
    size_t hashedKey = hash(key);
    Node *new = malloc(sizeof(Node));
    new->key = key;
    new->val = val;
    new->hash = hashedKey;
    size_t index = hashedKey % hm->capacity;
    new->next = hm->arr[index];
    hm->arr[index] = new;
    if(new->next == NULL)
        hm->size++;
    else
        hm->collisions++;
    return hashedKey;
}

Hashmap *init() {
    Hashmap *hm = malloc(sizeof(Hashmap));
    hm->capacity = DEFAULT_SIZE;
    hm->size = 0;
    hm->arr = calloc(hm->capacity, sizeof(Node *));
    hm->resizes = 0;
    hm->collisions = 0;
    return hm;
}

void resize(Hashmap *hm) {
    if(hm->capacity * 0.7 < hm->size) {
        size_t i;
        size_t oldCap = hm->capacity;
        for(i = hm->capacity*2+1; !isPrime(i); i += 2);
        hm->capacity = i;
        Node **tmp = calloc(hm->capacity, sizeof(Node *));
        hm->resizes++;
        Node *walk, *cur;
        hm->size = 0;
        for(i = 0; i < oldCap; ++i) {
            walk = hm->arr[i];
            while(walk) {
                cur = walk;
                walk = cur->next;
                cur->next = tmp[cur->hash % hm->capacity];
                if(cur->next) ++hm->collisions;
                else ++hm->size;
                tmp[cur->hash % hm->capacity] = cur;
            }
        }
        free(hm->arr);
        hm->arr = tmp;
    }
}

bool isPrime(size_t n) {
    unsigned int a, s = 0, d = n-1, r;
    unsigned int witnesses[5] = {2, 3, 5, 7, 11};
    for(a = 0; a < 5; a++)
        if(witnesses[a] == n)
            return true;
    while(d % 2 == 0) {
        d /= 2;
        s++;
    }
    for(a = 0; a < 5; a++) {
        if((unsigned int)power(witnesses[a], d, n) % n == 1)
            continue;
        for(r = 0; r < s; r++)
            if((unsigned int)power(witnesses[a] , power(2, r, n)*d, n) % n == n-1)
                break;
        if(r == s)
            return false;
    }
    return true;
}

unsigned int power(unsigned int a, unsigned int b, unsigned int mod) {
    if(b == 1)
        return a;
    if(b == 0)
        return 1;
    if(b % 2 == 0)
        return power((a*a) % mod, b/2, mod) % mod;
    else
        return a * power((a*a) % mod, b/2, mod) % mod;
}

void print(Hashmap *hm) {
    Node *cur;
    for(size_t i = 0; i < hm->capacity; ++i) {
        cur = hm->arr[i];
        while(cur) {
            printf("K: %s, V: %s -> ", cur->key, cur->val);
            cur = cur->next;
        }
        printf("NULL\n");
    }
}
