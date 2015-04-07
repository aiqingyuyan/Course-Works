// Copyright <Yan Yu> Miami University, 2015
#include <cstdlib>
#include <vector>
#include <iostream>

#define REPETITIONS 500000000

typedef union vec4 {
    float vec[4];
    float x, y, z, w;

    vec4() {
        vec[0] = vec[1] = vec[2] = vec[3] = 0.0f;
    }

    vec4(float x, float y, float z, float w) {
        vec[0] = x;
        vec[1] = y;
        vec[2] = z;
        vec[3] = w;
    }

    float operator()(int index) const {
        return vec[index];
    }

    float& operator()(int index) {
        return vec[index];
    }
} vec4;

void vec_product_approach1(const vec4& v1, const vec4& v2, float* result) {
    for (int i = 0; i < 4; i++) {
        result[i] = v1(i) * v2(i);
    }
}

void vec_product_approach2(const vec4& v1, const vec4& v2, float* result) {
    result[0] = v1.x * v2.x;
    result[1] = v1.y * v2.y;
    result[2] = v1.z * v2.z;
    result[3] = v1.w * v2.w;
}

int main() {
    float finalResult = 0;

    for (int i = 0; i < REPETITIONS; i++) {  // REPETITIONS times of experiments
        float result[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        vec4 v1(i, i - 1, i + 1, -i), v2(i - 1, i + 1, -i, -i + 1);
        vec_product_approach1(v1, v2, result);
        finalResult += (result[0] + result[1] + result[2] + result[3]);
    }

    std::cout << "Result: " << finalResult << "\n";

    return 0;
}
