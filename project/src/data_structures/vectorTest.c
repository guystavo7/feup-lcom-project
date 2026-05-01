#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/data_structures/vector.h" 

/* ========================================================================== */
/* TESTING FRAMEWORK                              */
/* ========================================================================== */

// Helper to format and print the aligned test output
void print_result(const char* description, long long expected, long long obtained) {
    int passed = (expected == obtained);
    printf("%-50s | Expected: %-20lld | Obtained: %-20lld | [%s]\n", 
           description, 
           expected, 
           obtained, 
           passed ? "TEST PASSED" : "TEST NOT PASSED");
}


/* ========================================================================== */
/* TEST CASES                                 */
/* ========================================================================== */

void test1_create_and_size() {
    printf("\n--- Test 1: vec_create, vec_size (Multiple Lengths) ---\n");
    
    Vector* v1 = vec_create(sizeof(char));          
    Vector* v4 = vec_create(sizeof(int));           
    Vector* v8 = vec_create(sizeof(long long int)); 
    
    print_result("vec_create size 0 (1-byte data)", 0, vec_size(v1));
    print_result("vec_create size 0 (4-byte data)", 0, vec_size(v4));
    print_result("vec_create size 0 (8-byte data)", 0, vec_size(v8));
    
    vec_free(v1); vec_free(v4); vec_free(v8);
}

void test2_push_back_and_get() {
    printf("\n--- Test 2: vec_push_back, vec_get_copy (Different Byte Lengths) ---\n");
    
    Vector* v_char = vec_create(sizeof(char));
    char c = -50; 
    vec_push_back(v_char, (char*)&c);
    char* res_c = (char*)vec_get_copy(v_char, 0);
    print_result("1-Byte Push/Get Copy (char)", -50, res_c ? *res_c : 0);
    
    Vector* v_int = vec_create(sizeof(int));
    int i = 2147483647; 
    vec_push_back(v_int, (char*)&i);
    int* res_i = (int*)vec_get_copy(v_int, 0);
    print_result("4-Byte Push/Get Copy (int)", 2147483647, res_i ? *res_i : 0);

    Vector* v_ll = vec_create(sizeof(long long int));
    long long int ll = 0x123456789ABCDEF0LL; 
    vec_push_back(v_ll, (char*)&ll);
    long long int* res_ll = (long long int*)vec_get_copy(v_ll, 0);
    print_result("8-Byte Push/Get Copy (long long)", 0x123456789ABCDEF0LL, res_ll ? *res_ll : 0);

    vec_free(v_char); vec_free(v_int); vec_free(v_ll);
    free(res_c); free(res_i); free(res_ll);
}

void test3_push_back_val() {
    printf("\n--- Test 3: vec_push_back_val, vec_get_val (Edge Cases) ---\n");
    Vector* v = vec_create(sizeof(long long int)); 
    
    vec_push_back_val(v, 666LL);                  
    vec_push_back_val(v, -1LL);                   
    vec_push_back_val(v, 0LL);                    
    vec_push_back_val(v, 9223372036854775807LL);  
    
    print_result("vec_get_val (Standard Positive)", 666LL, vec_get_val(v, 0));
    print_result("vec_get_val (Negative -1)", -1LL, vec_get_val(v, 1));
    print_result("vec_get_val (Zero)", 0LL, vec_get_val(v, 2));
    print_result("vec_get_val (Max 64-bit int)", 9223372036854775807LL, vec_get_val(v, 3));
    
    vec_free(v);
}

void test4_set_and_set_val() {
    printf("\n--- Test 4: vec_set, vec_set_val (Using 2-byte short) ---\n");
    Vector* v = vec_create(sizeof(short)); 
    
    vec_push_back_val(v, 10LL);
    vec_push_back_val(v, 20LL);

    short new_val = 32000;
    vec_set(v, 0, (char*)&new_val);
    short* res1 = (short*)vec_get_copy(v, 0);
    print_result("vec_set modifies 1st element via pointer", 32000, res1 ? *res1 : -1);

    vec_set_val(v, 1, -15000LL); 
    short* res2 = (short*)vec_get_copy(v, 1);
    print_result("vec_set_val modifies 2nd element directly", -15000, res2 ? *res2 : -1);

    vec_free(v);
    free(res1); free(res2);
}

void test5_insert() {
    printf("\n--- Test 5: vec_insert (Block Shifting) ---\n");
    Vector* v = vec_create(sizeof(char)); 
    
    vec_push_back_val(v, 10LL);
    vec_push_back_val(v, 30LL);

    char mid_val = 20;
    vec_insert(v, 1, (char*)&mid_val); 
    
    print_result("vec_insert increases size", 3, vec_size(v));
    
    char* res = (char*)vec_get_copy(v, 1);
    print_result("vec_insert places element at index 1", 20, res ? *res : -1);

    char* res2 = (char*)vec_get_copy(v, 2);
    print_result("vec_insert shifted subsequent element", 30, res2 ? *res2 : -1);

    vec_free(v);
    free(res); free(res2);
}

void test6_pop_back() {
    printf("\n--- Test 6: vec_pop_back ---\n");
    Vector* v = vec_create(sizeof(int));
    vec_push_back_val(v, 100000LL);
    vec_push_back_val(v, 200000LL);
    
    vec_pop_back(v);
    print_result("vec_pop_back reduces size by 1", 1, vec_size(v));

    int* res = (int*)vec_get_copy(v, 0);
    print_result("Remaining element is untouched", 100000, res ? *res : -1);

    vec_free(v);
    free(res);
}

void test7_remove() {
    printf("\n--- Test 7: vec_remove ---\n");
    Vector* v = vec_create(sizeof(long long int));
    vec_push_back_val(v, 100LL);
    vec_push_back_val(v, 200LL);
    vec_push_back_val(v, 300LL);
    
    vec_remove(v, 1); 
    
    print_result("vec_remove reduces size", 2, vec_size(v));
    
    long long int* res = (long long int*)vec_get_copy(v, 1);
    print_result("Elements are shifted left after remove", 300, res ? *res : -1);

    vec_free(v);
    free(res);
}

void test8_resize() {
    printf("\n--- Test 8: vec_resize ---\n");
    Vector* v = vec_create(sizeof(int)); 
    vec_push_back_val(v, 1LL);
    vec_push_back_val(v, 2LL);
    
    vec_resize(v, 10);
    print_result("vec_resize grows the vector capacity", 10, vec_size(v));
    
    vec_resize(v, 1);
    print_result("vec_resize shrinks the vector capacity", 1, vec_size(v));
    
    vec_free(v);
}

void test9_find() {
    printf("\n--- Test 9: vec_find ---\n");
    Vector* v = vec_create(sizeof(int)); 
    
    vec_push_back_val(v, 44444LL);
    vec_push_back_val(v, 55555LL);
    vec_push_back_val(v, 66666LL);
    
    int target = 55555;
    print_result("vec_find locates existing element", 1, vec_find(v, (char*)&target));
    
    int missing = 99999;
    print_result("vec_find returns -1 for missing elements", -1, vec_find(v, (char*)&missing));

    vec_free(v);
}

void test10_stress_test() {
    printf("\n--- Test 10: Stress Test (10,000 Items Dynamic Reallocation) ---\n");
    Vector* v = vec_create(sizeof(int));
    
    int iterations = 10000;
    for(int i = 0; i < iterations; i++) {
        vec_push_back_val(v, (long long int)i);
    }
    
    print_result("Handled massive dynamic reallocations (size)", iterations, vec_size(v));
    
    int* first = (int*)vec_get_copy(v, 0);
    int* middle = (int*)vec_get_copy(v, 5000);
    int* last = (int*)vec_get_copy(v, 9999);
    
    print_result("Stress Test: 1st element intact", 0, first ? *first : -1);
    print_result("Stress Test: 5001st element intact", 5000, middle ? *middle : -1);
    print_result("Stress Test: Last element intact", 9999, last ? *last : -1);
    
    vec_free(v);
    free(first); free(middle); free(last);
}

void test11_custom_structs() {
    printf("\n--- Test 11: Complex Custom Structs (Mixed Types/Padding) ---\n");
    
    typedef struct {
        int id;
        double health;
    } Player;
    
    Vector* v = vec_create(sizeof(Player));
    
    Player p1 = {1, 100.5};
    Player p2 = {2, 75.0};
    
    vec_push_back(v, (char*)&p1);
    vec_push_back(v, (char*)&p2);
    
    Player* res = (Player*)vec_get_copy(v, 1);
    
    // Test if both fields copied cleanly without memory corruption
    int struct_id_match = (res && res->id == 2) ? 1 : 0;
    int struct_hp_match = (res && res->health == 75.0) ? 1 : 0;
    
    print_result("Struct Copy: Integer field intact", 1, struct_id_match);
    print_result("Struct Copy: Double field intact", 1, struct_hp_match);
    
    vec_free(v);
    if(res) free(res);
}

void test12_boundary_manipulations() {
    printf("\n--- Test 12: Extreme Boundary Insertions & Removals ---\n");
    Vector* v = vec_create(sizeof(int));
    
    vec_push_back_val(v, 10LL);
    vec_push_back_val(v, 20LL);
    
    // Insert at absolute front (Index 0)
    int front = 999;
    vec_insert(v, 0, (char*)&front);
    int* res_front = (int*)vec_get_copy(v, 0);
    print_result("Insert at Head (Index 0)", 999, res_front ? *res_front : -1);
    
    // Remove at absolute front
    vec_remove(v, 0);
    int* res_after_rm = (int*)vec_get_copy(v, 0);
    print_result("Remove Head (Shifted correctly)", 10, res_after_rm ? *res_after_rm : -1);
    
    vec_free(v);
    free(res_front); free(res_after_rm);
}

void test13_strings() {
    printf("\n--- Test 13: Character Arrays (Strings) ---\n");
    // Allocate 16 bytes per slot to hold strings
    Vector* v = vec_create(16); 
    
    char word1[16] = "Hello";
    char word2[16] = "Vector";
    
    vec_push_back(v, word1);
    vec_push_back(v, word2);
    
    char* res = (char*)vec_get_copy(v, 1);
    int string_match = (res && strcmp(res, "Vector") == 0) ? 1 : 0;
    
    print_result("String pushed and copied correctly", 1, string_match);
    
    vec_free(v);
    if(res) free(res);
}

void test14_floats() {
    printf("\n--- Test 14: Floating Point Numbers (float & double) ---\n");
    
    // --- 4-BYTE FLOAT TEST ---
    Vector* v_float = vec_create(sizeof(float)); 
    float f1 = 3.14159f;
    float f2 = -987.654f;

    // We MUST use pointer-based push/get for floats to preserve the IEEE 754 binary format
    vec_push_back(v_float, (char*)&f1);
    vec_push_back(v_float, (char*)&f2);

    float* res_f1 = (float*)vec_get_copy(v_float, 0);
    float* res_f2 = (float*)vec_get_copy(v_float, 1);

    // Because memory copying is exact, we can strictly compare the exact decimal values
    int f_match1 = (res_f1 && *res_f1 == 3.14159f) ? 1 : 0;
    int f_match2 = (res_f2 && *res_f2 == -987.654f) ? 1 : 0;

    print_result("Float Copy: 1st element exact (3.14159)", 1, f_match1);
    print_result("Float Copy: 2nd element exact (-987.654)", 1, f_match2);

    // --- 8-BYTE DOUBLE TEST ---
    Vector* v_double = vec_create(sizeof(double));
    double d1 = 2.718281828459045; // High precision Euler's number
    
    vec_push_back(v_double, (char*)&d1);
    double* res_d1 = (double*)vec_get_copy(v_double, 0);
    
    int d_match = (res_d1 && *res_d1 == 2.718281828459045) ? 1 : 0;
    print_result("Double Copy: High precision exact match", 1, d_match);

    // Cleanup
    vec_free(v_float);
    vec_free(v_double);
    if(res_f1) free(res_f1);
    if(res_f2) free(res_f2);
    if(res_d1) free(res_d1);
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
    
    // New Extensive Tests
    test10_stress_test();
    test11_custom_structs();
    test12_boundary_manipulations();
    test13_strings();
    test14_floats();

    printf("\n==================================================================================\n");
    printf("                                END OF TEST SUITE                                 \n");
    printf("==================================================================================\n");
}

int main() {
    // Run the full test suite
    testSuit();
    return 0;
}