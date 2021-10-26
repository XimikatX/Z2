#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


typedef struct ht_entry {
    char* str;
    int count;
    struct ht_entry* next;
} ht_entry_t;


#define HT_SIZE 26801
#define BIT_MASK (0x20 - 1)

int djb2_hash(const char* str) { // djb2
    int hash_value = 0;
    while (*str != '\0') {
        hash_value = ((hash_value << 5) + hash_value + (*(str++) & BIT_MASK)) % HT_SIZE;
    }
    return hash_value;
}

int str_cmp(const char*, const char*);

void shell_sort(ht_entry_t*, int);


#define STR_LEN_MAX (1000 + 1)
#define STR_NUM_MAX 20000

int main() {

    int num_z;
    scanf("%d", &num_z);

    char* str_array = malloc(STR_NUM_MAX * STR_LEN_MAX);
    ht_entry_t* entries_array = malloc(STR_NUM_MAX * sizeof(ht_entry_t));
    ht_entry_t** hash_table = calloc(HT_SIZE, sizeof(ht_entry_t*));

    for (int z = 0; z < num_z; ++z) {

        int n;
        scanf("%d", &n);

        int num_entries = 0;
        memset(hash_table, 0, HT_SIZE * sizeof(ht_entry_t*));

        for (int i = 0; i < n; ++i) {

            char* str = str_array + i * STR_LEN_MAX;
            scanf("%s", str);

            int key = djb2_hash(str);

            ht_entry_t* entry;
            if (hash_table[key] == NULL) {

                entry = entries_array + (num_entries++);
                *entry = (ht_entry_t) {str, 1, NULL};

                hash_table[key] = entry;

            } else {

                entry = hash_table[key];

                bool str_found = false;
                if (str_cmp(str, entry->str) == 0) {
                    ++(entry->count);
                    str_found = true;
                } else {
                    while (entry->next != NULL) {
                        entry = entry->next;
                        if (str_cmp(str, entry->str) == 0) {
                            ++(entry->count);
                            str_found = true;
                            break;
                        }
                    }
                }

                if (!str_found) {
                    entry->next = entries_array + (num_entries++);
                    *entry->next = (ht_entry_t) {str, 1, NULL};
                }

            }

        }

        shell_sort(entries_array, num_entries);

        printf("%d\n", num_entries);
        for (int i = 0; i < num_entries; ++i) {
            printf("%s %d\n", entries_array[i].str, entries_array[i].count);
        }

    }

    return 0;

}


int str_cmp(const char* a, const char* b) {
    int compare;
    for (; *a != '\0' && *b != '\0'; ++a, ++b) {
        compare = (*a & BIT_MASK) - (*b & BIT_MASK);
        if (compare != 0) {
            return compare;
        }
    }
    return (int) *a - *b;
}

int ht_entry_cmp(const ht_entry_t* a, const ht_entry_t* b) {
    int compare = a->count - b->count;
    if (compare != 0) {
        return compare;
    }
    return str_cmp(b->str, a->str); // inverted natural order
}


const int GAPS[] = {701, 301, 132, 57, 23, 10, 4, 1}; // Ciura

void shell_sort(ht_entry_t* array, const int n) {

    ht_entry_t tmp;
    for (int g = 0; g < 8; ++g) {

        int gap = GAPS[g];

        for (int i = gap; i < n; ++i) {
            tmp = array[i];
            int j;
            for (j = i; j >= gap && ht_entry_cmp(array + j - gap, &tmp) < 0; j -= gap) {
                array[j] = array[j - gap];
            }
            array[j] = tmp;
        }

    }

}
