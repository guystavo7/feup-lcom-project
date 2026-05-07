

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "../../include/monster_path_test/vec2d.h" 

/* code for testing
  gcc -g -fsanitize=address vec2dTest.c vec2d.c -o vec2dTest -lm
  ./vec2dTest
*/

/* ========================================================================== */
/* TESTING FRAMEWORK                                                          */
/* ========================================================================== */



// Helper to safely compare floating point numbers
bool float_equals(float a, float b) {
    return fabs(a - b) < EPSILON;
}

// Helper for formatted float test output
void print_result_float(const char* description, float expected, float obtained) {
    int passed = float_equals(expected, obtained);
    printf("%-50s | Expected: %-15.4f | Obtained: %-15.4f | [%s]\n", 
           description, expected, obtained, passed ? "TEST PASSED" : "TEST NOT PASSED");
}

/* ========================================================================== */
/* TEST CASES                                                                 */
/* ========================================================================== */

void test1_create_and_getters() {
    printf("\n--- Test 1: vec2dCreate & Getters ---\n");
    
    // Standard positive values
    Vec2d* v1 = vec2dCreate(3.5f, 2.1f);
    print_result_float("vec2dCreate sets X correctly (Positive)", 3.5f, vec2dGetX(v1));
    print_result_float("vec2dCreate sets Y correctly (Positive)", 2.1f, vec2dGetY(v1));
    
    // Negative values
    Vec2d* v2 = vec2dCreate(-10.99f, -0.05f);
    print_result_float("vec2dCreate sets X correctly (Negative)", -10.99f, vec2dGetX(v2));
    print_result_float("vec2dCreate sets Y correctly (Negative)", -0.05f, vec2dGetY(v2));

    vec2dFree(v1);
    vec2dFree(v2);
}

void test2_setters() {
    printf("\n--- Test 2: Setters ---\n");
    
    Vec2d* v = vec2dCreate(0.0f, 0.0f);
    
    vec2dSetX(v, 42.42f);
    print_result_float("vec2dSetX updates X correctly", 42.42f, vec2dGetX(v));
    
    vec2dSetY(v, -99.99f);
    print_result_float("vec2dSetY updates Y correctly", -99.99f, vec2dGetY(v));
    
    // Ensure one setter doesn't overwrite the other memory block
    print_result_float("vec2dSetY did not corrupt X", 42.42f, vec2dGetX(v));

    vec2dFree(v);
}

void test3_module_magnitude() {
    printf("\n--- Test 3: Module (Magnitude/Length) ---\n");
    
    // Standard 3-4-5 Triangle
    Vec2d* v1 = vec2dCreate(3.0f, 4.0f);
    print_result_float("vec2dModule calculates 3-4-5 magnitude", 5.0f, vec2dModule(v1));
    
    // Negative values (Magnitude should always be positive)
    Vec2d* v2 = vec2dCreate(-6.0f, 8.0f);
    print_result_float("vec2dModule handles negative coordinates (6-8-10)", 10.0f, vec2dModule(v2));
    
    // Zero vector
    Vec2d* v_zero = vec2dCreate(0.0f, 0.0f);
    print_result_float("vec2dModule handles origin vector (0,0)", 0.0f, vec2dModule(v_zero));

    vec2dFree(v1);
    vec2dFree(v2);
    vec2dFree(v_zero);
}

void test4_direction_normalization() {
    printf("\n--- Test 4: Direction (Normalization) ---\n");
    
    Vec2d* v = vec2dCreate(3.0f, 4.0f);
    
    // vec2dNormalize modifies the vector IN PLACE (no new allocation)
    vec2dNormalize(v);
    
    // Normalized vector of (3,4) is (3/5, 4/5) -> (0.6, 0.8)
    print_result_float("vec2dNormalize sets X correctly", 0.6f, vec2dGetX(v));
    print_result_float("vec2dNormalize sets Y correctly", 0.8f, vec2dGetY(v));
    
    // The magnitude of a normalized vector must ALWAYS be exactly 1.0
    print_result_float("vec2dNormalize magnitude is exactly 1.0", 1.0f, vec2dModule(v));

    vec2dFree(v); // Only one vector to free now!
}

void test5_direction_edge_cases() {
    printf("\n--- Test 5: Direction Edge Cases (Divide by Zero) ---\n");
    
    Vec2d* v_zero = vec2dCreate(0.0f, 0.0f);
    
    // Attempt to normalize the zero vector in place
    vec2dNormalize(v_zero);
    
    /* 
     * If vec2dNormalize tries to divide by the module (which is 0), it will cause a NaN 
     * (Not a Number) or crash. A robust implementation should catch this and 
     * safely leave the vector as (0,0) or set to a default like (1,0). 
     * We will test assuming it safely falls back to (0,0).
     */
    bool handled_safely = false;
    
    float dx = vec2dGetX(v_zero);
    float dy = vec2dGetY(v_zero);
    
    // Check if the values are safely resting at 0.0f instead of NaN
    if (float_equals(dx, 0.0f) && float_equals(dy, 0.0f)) {
        handled_safely = true;
    }
    
    printf("%-50s | Expected: %-15s | Obtained: %-15s | [%s]\n", 
           "vec2dNormalize safely handles (0,0)", 
           "Handled", 
           handled_safely ? "Handled" : "Failed/NaN", 
           handled_safely ? "TEST PASSED" : "TEST NOT PASSED");

    vec2dFree(v_zero); // Only one vector to free now!
}

/* ========================================================================== */
/* MAIN TEST SUIT                                                             */
/* ========================================================================== */

void testSuitVec2d() {
    printf("==================================================================================\n");
    printf("                             VEC2D INTERFACE TEST SUITE                           \n");
    printf("==================================================================================\n");
    
    test1_create_and_getters();
    test2_setters();
    test3_module_magnitude();
    test4_direction_normalization();
    test5_direction_edge_cases();

    printf("\n==================================================================================\n");
    printf("                                  END OF TEST SUITE                               \n");
    printf("==================================================================================\n");
}

int main() {
    testSuitVec2d();
    return 0;
}