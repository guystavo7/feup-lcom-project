#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// #include "Monster.h" // Uncomment this in your actual code
#include "../../include/monster_path_test/monster.h"

/*
  gcc -g -Wall -fsanitize=address monsterTest.c monster.c vec2d.c -o monsterTest -lm
  ./monsterTest
*/



/* ========================================================================== */
/* TESTING FRAMEWORK                                                          */
/* ========================================================================== */

#define EPSILON 0.0001f

// Helper to safely compare floating point numbers
bool float_equals(float a, float b) {
    return fabs(a - b) < EPSILON;
}

// Helper for formatted float test output
void print_result_float(const char* description, float expected, float obtained) {
    int passed = float_equals(expected, obtained);
    printf("%-50s | Expected: %-10.4f | Obtained: %-10.4f | [%s]\n", 
           description, expected, obtained, passed ? "TEST PASSED" : "TEST NOT PASSED");
}

// Helper for pointer test output
void print_result_ptr(const char* description, void* expected, void* obtained) {
    int passed = (expected == obtained);
    printf("%-50s | Expected: %-10p | Obtained: %-10p | [%s]\n", 
           description, expected, obtained, passed ? "TEST PASSED" : "TEST NOT PASSED");
}

/* ========================================================================== */
/* TEST CASES                                                                 */
/* ========================================================================== */

void test1_create_and_getters() {
    printf("\n--- Test 1: Creation and Getters (NULL Sprite) ---\n");
    
    Monster* m = monsterCreate(10.5f, 20.5f, -2.0f, 3.5f);
    
    print_result_float("monsterCreate sets X correctly", 10.5f, monsterGetX(m));
    print_result_float("monsterCreate sets Y correctly", 20.5f, monsterGetY(m));
    print_result_float("monsterCreate sets VelX correctly", -2.0f, monsterGetVelX(m));
    print_result_float("monsterCreate sets VelY correctly", 3.5f, monsterGetVelY(m));

    monsterFree(m);
}

void test2_setters() {
    printf("\n--- Test 2: Position and Velocity Setters ---\n");
    
    Monster* m = monsterCreate(0.0f, 0.0f, 0.0f, 0.0f);
    
    monsterSetX(m, 100.0f);
    print_result_float("monsterSetX updates X safely", 100.0f, monsterGetX(m));
    
    monsterSetY(m, -50.25f);
    print_result_float("monsterSetY updates Y safely", -50.25f, monsterGetY(m));
    
    monsterSetVelX(m, 14.5f);
    print_result_float("monsterSetVelX updates VelX safely", 14.5f, monsterGetVelX(m));
    
    monsterSetVelY(m, -9.9f);
    print_result_float("monsterSetVelY updates VelY safely", -9.9f, monsterGetVelY(m));

    // Cross-contamination check
    print_result_float("Velocity change did not corrupt Position X", 100.0f, monsterGetX(m));

    monsterFree(m);
}

void test3_update_position() {
    printf("\n--- Test 3: Physics & Position Update ---\n");
    
    // Create monster at (10, 10) moving at (5, -2) units per second
    Monster* m = monsterCreate(10.0f, 10.0f, 5.0f, -2.0f);
    
    // Simulate passing 2.0 seconds (delta_t)
    // Expected X: 10.0 + (5.0 * 2.0) = 20.0
    // Expected Y: 10.0 + (-2.0 * 2.0) = 6.0
    monsterUpdatePos(m, 2.0);
    
    print_result_float("monsterUpdatePos calculates new X correctly", 20.0f, monsterGetX(m));
    print_result_float("monsterUpdatePos calculates new Y correctly", 6.0f, monsterGetY(m));

    // Simulate passing 0.5 seconds
    // Expected X: 20.0 + (5.0 * 0.5) = 22.5
    // Expected Y: 6.0 + (-2.0 * 0.5) = 5.0
    monsterUpdatePos(m, 0.5);
    
    print_result_float("monsterUpdatePos handles fractional delta_t (X)", 22.5f, monsterGetX(m));
    print_result_float("monsterUpdatePos handles fractional delta_t (Y)", 5.0f, monsterGetY(m));

    monsterFree(m);
}

void test4_set_direction() {
    printf("\n--- Test 4: Set Direction Logic ---\n");
    
    Monster* m = monsterCreate(0.0f, 0.0f, 10.0f, 10.0f);
    
    // Depending on your implementation, monsterSetDir might normalize the vector 
    // or just directly assign the values to VelX and VelY. We test the signs to be safe.
    monsterSetDir(m, -1.0f, 0.0f);
    
    float newVelX = monsterGetVelX(m);
    float newVelY = monsterGetVelY(m);
    
    int isMovingLeft = (newVelX < 0.0f) ? 1 : 0;
    int isNotMovingVertically = float_equals(newVelY, 0.0f) ? 1 : 0;
    
    print_result_float("monsterSetDir correctly applies negative X dir", 1.0f, (float)isMovingLeft);
    print_result_float("monsterSetDir correctly zeroes Y dir", 1.0f, (float)isNotMovingVertically);

    monsterFree(m);
}

void test5_free() {
    printf("\n--- Test 5: Safe Memory Cleanup ---\n");
    
    Monster* m = monsterCreate(0.0f, 0.0f, 0.0f, 0.0f);
    
    // monsterFree should return NULL so we can do: m = monsterFree(m);
    m = monsterFree(m);
    
    print_result_ptr("monsterFree returns NULL pointer", NULL, m);
}

/* ========================================================================== */
/* MAIN TEST SUIT                                                             */
/* ========================================================================== */

void testSuitMonster() {
    printf("==================================================================================\n");
    printf("                            MONSTER INTERFACE TEST SUITE                          \n");
    printf("==================================================================================\n");
    
    test1_create_and_getters();
    test2_setters();
    test3_update_position();
    test4_set_direction();
    test5_free();

    printf("\n==================================================================================\n");
    printf("                                  END OF TEST SUITE                               \n");
    printf("==================================================================================\n");
}

int main() {
    testSuitMonster();
    return 0;
}