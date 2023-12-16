#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const double E = 2.718281828459045;
bool QUITE = false;

void compare_e_with_real_e(double e) {
  if (!QUITE) {
    fprintf(stderr, "estimated e: %.15lf\n", e);
    fprintf(stderr, "     real e: %.15lf\n", E);
  }
  return;
}

void method_a(void) {
  double area = 0.f;
  double x = 1.f;
  double delta_x = 10e-7;

  for (; area < 1.f; x += delta_x) {
    double next_x = x + delta_x;
    double midpoint = (1.f / x + 1.f / next_x) / 2.f;
    area += midpoint * delta_x;
  }

  double e = x;
  compare_e_with_real_e(e);
}

void method_b(void) {
  double n = 10e7;
  double e = pow(1.f + 1.f / n, n);

  compare_e_with_real_e(e);
}

void method_c(void) {
  size_t ITERATION_CAP = 1000;
  double EPSILON = 10e-15;

  double e = 0.f;
  double prev_e = 0.f;
  double one_over_factorial = 1.f;

  for (size_t i = 1; i < ITERATION_CAP; i++) {
    prev_e = e;
    e += one_over_factorial;
    one_over_factorial *= 1.f / (double)i;

    if (fabs(e - prev_e) < EPSILON) {
      if (!QUITE) {
        fprintf(stderr, "Stopped at iteration #%zu\n", i);
      }

      break;
    }
  }

  compare_e_with_real_e(e);
}

void benchmark(const char *methods, size_t N) {
  clock_t start, end;

  printf("Benchmarking result for estimating e %zu times:\n", N);
  printf("Method\tTime Taken(sec)\n");

  if (strchr(methods, 'a')) {
    start = clock();
    for (size_t i = 0; i < N; i++) {
      method_a();
    }
    end = clock();
    double method_a_cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("a\t%lf\n", method_a_cpu_time_used);
  }

  if (strchr(methods, 'b')) {
    start = clock();
    for (size_t i = 0; i < N; i++) {
      method_b();
    }
    end = clock();
    double method_b_cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("b\t%lf\n", method_b_cpu_time_used);
  }

  if (strchr(methods, 'c')) {
    start = clock();
    for (size_t i = 0; i < N; i++) {
      method_c();
    }
    end = clock();
    double method_c_cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("c\t%lf\n", method_c_cpu_time_used);
  }
}

char *shift_args(int *argc, char ***argv) {
  char *arg = *argv[0];
  *argv += 1;
  *argc -= 1;
  return arg;
}

void usage(const char *program) {
  fprintf(stderr, "Usage: %s [OPTIONS...]\n", program);
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "    -m METHOD    estimate e\n");
  fprintf(
      stderr,
      "    -b METHODS N benchmark specified methods by estimating e N times\n");
  fprintf(stderr, "    -h           print this help and exit\n");
  fprintf(stderr, "Example:\n");
  fprintf(stderr, "    $ %s -m a\n", program);
  fprintf(stderr, "    Estimate e with method a.\n");
  fprintf(stderr, "    $ %s -b abc 1000\n", program);
  fprintf(stderr, "    Bechmark method a, b, c by estimating e 1000 times.\n");
}

int main(int argc, char **argv) {
  char *program = shift_args(&argc, &argv);

  while (argc) {
    char *arg = shift_args(&argc, &argv);

    if (strcmp(arg, "-h") == 0) {
      usage(program);
    } else if (strcmp(arg, "-b") == 0) {
      QUITE = true;

      char *methods = shift_args(&argc, &argv);
      char *N = shift_args(&argc, &argv);
      benchmark(methods, atoi(N));
    } else if (strcmp(arg, "-m") == 0) {
      char *method = shift_args(&argc, &argv);

      if (strcmp(method, "a") == 0) {
        method_a();
      }
      if (strcmp(method, "b") == 0) {
        method_b();
      }
      if (strcmp(method, "c") == 0) {
        method_c();
      }
    }
  }

  return 0;
}
