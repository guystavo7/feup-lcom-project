#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 #include "Vector.h" 

/* ========================================================================== */
/* TESTING FRAMEWORK                              */
/* ========================================================================== */

// Helper to format and print the aligned test output
void print_result(const char* description, long long expected, long long obtained) {
    int passed = (expected == obtained);
    printf("%-45s | Expected: %-5lld | Obtained: %-5lld | [%s]\n", 
           description, 
           expected, 
           obtained, 
           passed ? "TEST PASSED" : "TEST NOT PASSED");
}


/* ========================================================================== */
/* TEST CASES                                 */
/* ========================================================================== */

void test1_create_and_size() {
    printf("\n--- Test 1: vec_create, vec_size, vec_free ---\n");
    Vector* v = vec_create(sizeof(long long int));
    
    // We expect the newly created vector to have a size of 0
    print_result("vec_create initializes size to 0", 0, vec_size(v));
    
    vec_free(v);
}

void test2_push_back_and_get() {
    printf("\n--- Test 2: vec_push_back, vec_get ---\n");
    Vector* v = vec_create(sizeof(long long int));
    long long int val1 = 100;
    long long int val2 = 200;

    vec_push_back(v, (char*)&val1);
    print_result("vec_push_back increments size to 1", 1, vec_size(v));

    vec_push_back(v, (char*)&val2);
    print_result("vec_push_back increments size to 2", 2, vec_size(v));

    long long int* res1 = (long long int*)vec_get(v, 0);
    long long int* res2 = (long long int*)vec_get(v, 1);

    print_result("vec_get retrieves 1st element correctly", 100, res1 ? *res1 : -1);
    print_result("vec_get retrieves 2nd element correctly", 200, res2 ? *res2 : -1);

    vec_free(v);

    free(res1);
    free(res2);
}

void test3_push_back_val() {
    printf("\n--- Test 3: vec_push_back_val ---\n");
    Vector* v = vec_create(sizeof(long long int));
    
    vec_push_back_val(v, 555LL);
    print_result("vec_push_back_val increments size", 1, vec_size(v));
    
    long long int* res = (long long int*)vec_get(v, 0);
    print_result("vec_get retrieves pushed long long val", 555, res ? *res : -1);
    
    vec_free(v);
    free(res);
}

void test4_set_and_set_val() {
    printf("\n--- Test 4: vec_set, vec_set_val ---\n");
    Vector* v = vec_create(sizeof(long long int));
    vec_push_back_val(v, 10LL);
    vec_push_back_val(v, 20LL);

    long long int new_val = 99LL;
    vec_set(v, 0, (char*)&new_val);
    long long int* res1 = (long long int*)vec_get(v, 0);
    print_result("vec_set modifies 1st element via pointer", 99, res1 ? *res1 : -1);

    vec_set_val(v, 1, 88LL);
    long long int* res2 = (long long int*)vec_get(v, 1);
    print_result("vec_set_val modifies 2nd element directly", 88, res2 ? *res2 : -1);

    vec_free(v);
    free(res1);free(res2);
}

void test5_insert() {
    printf("\n--- Test 5: vec_insert ---\n");
    Vector* v = vec_create(sizeof(long long int));
    vec_push_back_val(v, 1LL);
    vec_push_back_val(v, 3LL);

    long long int mid_val = 2LL;
    vec_insert(v, 1, (char*)&mid_val); // Insert 2 at pos 1
    
    print_result("vec_insert increases size", 3, vec_size(v));
    
    long long int* res = (long long int*)vec_get(v, 1);
    print_result("vec_insert places element at index 1", 2, res ? *res : -1);

    long long int* res2 = (long long int*)vec_get(v, 2);
    print_result("vec_insert shifted subsequent element", 3, res2 ? *res2 : -1);

    vec_free(v);

    free(res);free(res2);
}

void test6_pop_back() {
    printf("\n--- Test 6: vec_pop_back ---\n");
    Vector* v = vec_create(sizeof(long long int));
    vec_push_back_val(v, 10LL);
    vec_push_back_val(v, 20LL);
    
    vec_pop_back(v);
    print_result("vec_pop_back reduces size by 1", 1, vec_size(v));

    long long int* res = (long long int*)vec_get(v, 0);
    print_result("Remaining element is untouched", 10, res ? *res : -1);

    vec_free(v);

    free(res);
}

void test7_remove() {
    printf("\n--- Test 7: vec_remove ---\n");
    Vector* v = vec_create(sizeof(long long int));
    vec_push_back_val(v, 100LL);
    vec_push_back_val(v, 200LL);
    vec_push_back_val(v, 300LL);
    
    vec_remove(v, 1); // Remove the 200 at pos 1
    
    print_result("vec_remove reduces size", 2, vec_size(v));
    
    long long int* res = (long long int*)vec_get(v, 1);
    print_result("Elements are shifted left after remove", 300, res ? *res : -1);

    vec_free(v);

    free(res);
}

void test8_resize() {
    printf("\n--- Test 8: vec_resize ---\n");
    Vector* v = vec_create(sizeof(long long int));
    vec_push_back_val(v, 1LL);
    vec_push_back_val(v, 2LL);
    
    vec_resize(v, 10);
    print_result("vec_resize grows the vector size", 10, vec_size(v));
    
    vec_resize(v, 1);
    print_result("vec_resize shrinks the vector size", 1, vec_size(v));
    
    vec_free(v);
}

void test9_find() {
    printf("\n--- Test 9: vec_find ---\n");
    Vector* v = vec_create(sizeof(long long int));
    vec_push_back_val(v, 44LL);
    vec_push_back_val(v, 55LL);
    vec_push_back_val(v, 66LL);
    
    long long int target = 55LL;
    int index_found = vec_find(v, (char*)&target);
    print_result("vec_find locates existing element", 1, index_found);
    
    long long int missing = 99LL;
    int index_missing = vec_find(v, (char*)&missing);
    print_result("vec_find returns -1 for missing elements", -1, index_missing);

    vec_free(v);
}

/* ========================================================================== */
/* MAIN TEST SUIT                              */
/* ========================================================================== */

void testSuit() {
    printf("==================================================================================\n");
    printf("                            VECTOR INTERFACE TEST SUITE                           \n");
    printf("==================================================================================\n");
    
    test1_create_and_size();
    test2_push_back_and_get();
    test3_push_back_val();
    test4_set_and_set_val();
    test5_insert();
    test6_pop_back();
    test7_remove();
    test8_resize();
    test9_find();

    printf("\n==================================================================================\n");
    printf("                                END OF TEST SUITE                                 \n");
    printf("==================================================================================\n");
}

int main() {
    // Run the full test suite
    testSuit();
    return 0;
}