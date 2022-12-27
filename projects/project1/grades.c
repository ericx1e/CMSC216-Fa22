/*
Eric Xie
118862019
exie
Project 1
*/

#include <math.h>
#include <stdio.h>
#define MAX_ASSIGNMENTS 50

/* Prototypes */
int valid_weights(int weights[], int n);
int *drop_assingments(int num_drops, int assignment_scores[],
                      int assignment_weights[], int num_assignments);
double calculate_adjusted_score(int score, int days_late, int penalty_per_day);
double calculate_mean(int array[], int length);
double calculate_std(int array[], int length);

int main() {
  double numeric_score, total_weight;
  int penalty_per_day_late, num_drops, num_assignments, assignment_num, i;
  char stats; /* Y or N */
  int assignmemt_scores[MAX_ASSIGNMENTS], assignment_weights[MAX_ASSIGNMENTS],
      days_late_array[MAX_ASSIGNMENTS], adjusted_scores[MAX_ASSIGNMENTS];
  int *dropped_ptr;

  scanf(" %d %d %c", &penalty_per_day_late, &num_drops, &stats);
  scanf(" %d", &num_assignments);

  /* Populates parallel arrays based on the assignment number */
  /* Assignment 1 is at index 0 */
  for (i = 0; i < num_assignments; i++) {
    scanf("%d", &assignment_num);
    scanf(", %d, %d, %d\n", &assignmemt_scores[assignment_num - 1],
          &assignment_weights[assignment_num - 1],
          &days_late_array[assignment_num - 1]);
    adjusted_scores[assignment_num - 1] = calculate_adjusted_score(
        assignmemt_scores[assignment_num - 1],
        days_late_array[assignment_num - 1], penalty_per_day_late);
  }

  /* Checks if weights add to 100 */
  if (!valid_weights(assignment_weights, num_assignments)) {
    printf("ERROR: Invalid values provided\n");
    /* Terminate the program */
    return 0;
  }

  numeric_score = 0;

  dropped_ptr = drop_assingments(num_drops, assignmemt_scores,
                                 assignment_weights, num_assignments);
  /* Treat dropped_ptr as a parallel array */

  for (i = 0; i < num_assignments; i++) {
    if (!dropped_ptr[i]) {
      numeric_score += adjusted_scores[i] * assignment_weights[i];
      total_weight += assignment_weights[i];
    }
  }

  /* Adjusts by new total weight after dropping assignments */
  numeric_score /= total_weight;

  printf("Numeric Score: %5.4f\n", numeric_score);
  printf("Points Penalty Per Day Late: %d\n", penalty_per_day_late);
  printf("Number of Assignments Dropped: %d\n", num_drops);
  printf("Values Provided:\nAssignment, Score, Weight, Days Late\n");

  for (i = 0; i < num_assignments; i++) {
    printf("%d, %d, %d, %d\n", i + 1, assignmemt_scores[i],
           assignment_weights[i], days_late_array[i]);
  }

  /* Prints out stats if requested */
  if (stats == 'Y' || stats == 'y') {
    printf("Mean: %5.4f, Standard Deviation: %5.4f\n",
           calculate_mean(adjusted_scores, num_assignments),
           calculate_std(adjusted_scores, num_assignments));
  }

  return 0;
}

/* Returns 1 if the n weights add up to 100, otherwise return 0 */
int valid_weights(int weights[], int n) {
  int i;
  int sum = 0;

  for (i = 0; i < n; i++) {
    sum += weights[i];
  }

  return sum == 100;
}

/* Returns the pointer to a parallel array
which is 1 at each dropped assignment */
int *drop_assingments(int num_drops, int assignment_scores[],
                      int assignment_weights[], int num_assignments) {
  int i, j, min_index, min_value, cur_value;
  /* Parallel array indicating whether an assignment was already dropped */
  /* Static so it lives in memory after the function ends */
  static int dropped_array[MAX_ASSIGNMENTS];

  /* Initialize dropped_array with zeroes) */
  for (i = 0; i < num_assignments; i++) {
    dropped_array[i] = 0;
  }

  /* Drops the lowest value assignment that hasn't been dropped yet */
  /* Since the assignments are indexed based on assignment number, the
   * the lowest assignment number will be dropped in the case of a tie */
  for (i = 0; i < num_drops; i++) {
    min_index = -1;
    min_value = 0;

    /* Loop over all the assignments and keeps
    track of the lowest index and value seen */
    for (j = 0; j < num_assignments; j++) {
      if (!dropped_array[j]) {
        cur_value = assignment_scores[j] * assignment_weights[j];
        /* If min_index is -1, then this is the first non-dropped
         assingment and can be assumed to be the min */
        if (min_index == -1 || cur_value < min_value) {
          min_value = cur_value;
          min_index = j;
        }
      }
    }

    /* Update dropped array to drop the lowest value assignment */
    dropped_array[min_index] = 1;
  }

  return dropped_array;
}

/* Calculates scores factoring in the number of days late */
double calculate_adjusted_score(int score, int days_late, int penalty_per_day) {
  int penalty = days_late * penalty_per_day;

  /* If resulting score would be less than zero, return 0 */
  if (score - penalty <= 0) {
    return 0;
  }

  return score - penalty;
}

/* Calculates the mean of an array */
double calculate_mean(int array[], int length) {
  int i;
  double sum = 0;

  for (i = 0; i < length; i++) {
    sum += array[i];
  }

  return sum / length;
}

/* Calculates the standard deviation of an array */
double calculate_std(int array[], int length) {
  int i, diff;
  int mean = calculate_mean(array, length);
  int variance[MAX_ASSIGNMENTS];

  for (i = 0; i < length; i++) {
    diff = array[i] - mean;
    variance[i] = diff * diff;
  }

  return sqrt(calculate_mean(variance, length));
}