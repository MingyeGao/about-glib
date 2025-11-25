#include <stdio.h>
#include <glib.h>
#include <assert.h>

void use_hash_table_as_dict(){
    GHashTable *dict = g_hash_table_new(NULL, NULL);
    g_hash_table_insert(dict, "a", (void*)1);
    printf("Inserted \"a\" to dict\n");
    printf("dict contains a: %d\n", g_hash_table_contains(dict, "a"));
    printf("Value of \"a\" is %ld\n", (size_t)g_hash_table_lookup(dict, "a"));


    g_hash_table_destroy(dict);
}

int main(int argc, char *argv[]){
    
}