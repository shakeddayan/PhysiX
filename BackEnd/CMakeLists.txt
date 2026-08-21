cmake_minimum_required(VERSION 3.31)
project(BackEnd C)

set(CMAKE_C_STANDARD 99)


add_library(BackEnd SHARED
        world.c
        structs.c structs.h
        constants.h
        calculator.c calculator.h
        benchmark.c
        benchmark.h)


add_executable(TestApp
        world.c
        structs.c structs.h
        constants.h
        calculator.c calculator.h
        benchmark.c
        benchmark.h)

# -O3: רמת האופטימיזציה הגבוהה ביותר (מפעיל אוטומטית Auto-vectorization)
# -march=native: מרשה לקומפיילר להשתמש בפקודות החדישות ביותר של המעבד הספציפי שלך (כמו AVX2 או AVX-512)
# -fopt-info-vec-optimized: מבקש מ-GCC להדפיס למסך הודעה על כל לולאה שהוא הצליח להפוך ל-SIMD
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -march=native -ffast-math -fopt-info-vec-optimized")
