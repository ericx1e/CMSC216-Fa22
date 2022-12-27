#include "calendar.h"
#include "event.h"
#include "my_memory_checker_216.h"
#include <stdio.h>
#include <stdlib.h>

/*****************************************************/
/* In this file you will provide tests for your      */
/* calendar application.  Each test should be named  */
/* test1(), test2(), etc. Each test must have a      */
/* brief description of what it is testing (this     */
/* description is important).                        */
/*                                                   */
/* You can tell whether any test failed if after     */
/* executing the students tests, you executed        */
/* "echo $?" on the command line and you get a value */
/* other than 0.  "echo $?" prints the status of     */
/* the last command executed by the shell.           */
/*                                                   */
/* Notice that main just calls test1(), test2(), etc.*/
/* and once one fails, the program eventually        */
/* return EXIT_FAILURE; otherwise EXIT_SUCCESS will  */
/* be returned.                                      */
/*****************************************************/

static int comp_minutes(const void *ptr1, const void *ptr2) {
  return ((Event *)ptr1)->duration_minutes - ((Event *)ptr2)->duration_minutes;
}

typedef struct task_info {
  double cost;
  char *food;
} Task_info;

static int comp_name(const void *ptr1, const void *ptr2) {
  return strcmp(((Event *)ptr1)->name, ((Event *)ptr2)->name);
}

static Task_info *create_task_info(double cost, const char *food) {
  Task_info *task_info = malloc(sizeof(Task_info));

  if (task_info) {
    task_info->food = malloc(strlen(food) + 1);
    if (task_info->food) {
      task_info->cost = cost;
      strcpy(task_info->food, food);
      return task_info;
    }
  }

  return NULL;
}

static void free_task_info(void *ptr) {
  Task_info *task_info = (Task_info *)ptr;

  free(task_info->food);
  free(task_info);
}

/* Checks init_calendar, print_calendar, and destroy_calendar */
static int test1() {
  int days = 7;
  Calendar *calendar;

  if (init_calendar("Spr", days, comp_minutes, NULL, &calendar) == SUCCESS) {
    if (add_event(calendar, "lunch", 100, 60, NULL, 1) == SUCCESS &&
        add_event(calendar, "dinner", 600, 30, NULL, 1) == SUCCESS) {
      if (print_calendar(calendar, stdout, 1) == SUCCESS) {
        if (print_calendar(calendar, stdout, 0) == SUCCESS) {
          return destroy_calendar(calendar);
        }
      }
    }
  }

  return FAILURE;
}

/* Checks add_event */
static int test2() {
  int days = 4;
  Calendar *calendar;

  if (init_calendar("Hello", days, comp_minutes, NULL, &calendar) == SUCCESS) {
    if (add_event(calendar, "lunch", 100, 60, NULL, 1) == SUCCESS &&
        add_event(calendar, "dinner", 600, 30, NULL, 1) == SUCCESS &&
        add_event(calendar, "meeting", 2, 1000, NULL, 3) == SUCCESS &&
        add_event(calendar, "lunch", 100, 100, NULL, 1) == FAILURE &&
        add_event(calendar, "a", 2, 1, NULL, 3) == SUCCESS &&
        add_event(calendar, "b", 2, 2, NULL, 3) == SUCCESS &&
        add_event(calendar, "c", 2, 3, NULL, 3) == SUCCESS &&
        add_event(calendar, "d", 2, 4, NULL, 3) == SUCCESS) {
      if (print_calendar(calendar, stdout, 1) == SUCCESS) {
        return destroy_calendar(calendar);
      }
    }
  }
  return FAILURE;
}

/* Checks clear_day */
static int test3() {
  int days = 3;
  Calendar *calendar;

  if (init_calendar("Sleepy", days, comp_minutes, NULL, &calendar) == SUCCESS) {
    if (add_event(calendar, "sleep", 100, 1, NULL, 1) == SUCCESS &&
        add_event(calendar, "nap", 600, 1, NULL, 1) == SUCCESS &&
        add_event(calendar, "wake up", 2, 10, NULL, 3) == SUCCESS) {
      if (print_calendar(calendar, stdout, 1) == SUCCESS) {
        if (clear_day(calendar, 1) == SUCCESS) {
          if (print_calendar(calendar, stdout, 1) == SUCCESS) {
            if (clear_day(calendar, 5) == FAILURE) {
              return destroy_calendar(calendar);
            }
          }
        }
      }
    }
  }
  return FAILURE;
}

/* Checks clear_calendar */
static int test4() {
  int days = 10;
  Calendar *calendar;

  if (init_calendar("clear me", days, comp_minutes, NULL, &calendar) ==
      SUCCESS) {
    if (add_event(calendar, "junk1", 100, 1, NULL, 1) == SUCCESS &&
        add_event(calendar, "trash1", 600, 1, NULL, 2) == SUCCESS &&
        add_event(calendar, "junk2", 2, 10, NULL, 1) == SUCCESS) {
      if (print_calendar(calendar, stdout, 1) == SUCCESS) {
        if (clear_calendar(calendar) == SUCCESS) {
          if (add_event(calendar, "fun", 10, 10, NULL, 1) == SUCCESS) {
            if (print_calendar(calendar, stdout, 1) == SUCCESS) {
              return destroy_calendar(calendar);
            }
          }
        }
      }
    }
  }
  return FAILURE;
}

/* Checks find_event and find_event_in_day*/
static int test5() {
  int days = 3, day;
  Calendar *calendar;
  Event *event;

  if (init_calendar("search me", days, comp_minutes, NULL, &calendar) ==
      SUCCESS) {
    if (add_event(calendar, "a", 100, 1, NULL, 1) == SUCCESS &&
        add_event(calendar, "b", 600, 1, NULL, 2) == SUCCESS &&
        add_event(calendar, "c", 2, 3, NULL, 1) == SUCCESS) {
      if (find_event(calendar, "c", &event) == SUCCESS) {
        printf("Event %s found, start: %d, duration: %d\n", event->name,
               event->start_time, event->duration_minutes);
      } else {
        printf("Event %s not found\n");
      }
      if (find_event(calendar, "hello", &event) == SUCCESS) {
        printf("Event %s found, start: %d, duration: %d\n", event->name,
               event->start_time, event->duration_minutes);
      } else {
        printf("Event %s not found\n");
      }

      day = 1;
      if (find_event_in_day(calendar, "c", day, &event) == SUCCESS) {
        printf("Event %s found in day %d, start: %d, duration: %d\n",
               event->name, day, event->start_time, event->duration_minutes);
      } else {
        printf("Event not found in day %d\n", day);
      }

      day = 2;
      if (find_event_in_day(calendar, "c", day, &event) == SUCCESS) {
        printf("Event %s found in day %d, start: %d, duration: %d\n",
               event->name, day, event->start_time, event->duration_minutes);
      } else {
        printf("Event not found in day %d\n", day);
      }
      return destroy_calendar(calendar);
    }
  }
  return FAILURE;
}

/* Checks remove_event */
static int test6() {
  int days = 4;
  Calendar *calendar;

  if (init_calendar("remove_stuff", days, comp_minutes, NULL, &calendar) ==
      SUCCESS) {
    if (add_event(calendar, "a", 2, 1, NULL, 3) == SUCCESS &&
        add_event(calendar, "b", 2, 2, NULL, 3) == SUCCESS &&
        add_event(calendar, "c", 2, 3, NULL, 3) == SUCCESS &&
        add_event(calendar, "d", 2, 4, NULL, 3) == SUCCESS) {
      if (print_calendar(calendar, stdout, 1) == SUCCESS) {
        if (remove_event(calendar, "a") == SUCCESS &&
            remove_event(calendar, "b") == SUCCESS &&
            remove_event(calendar, "hi") == FAILURE) {
          if (print_calendar(calendar, stdout, 1) == SUCCESS) {

            return destroy_calendar(calendar);
          }
        }
      }
    }
  }
  return FAILURE;
}

static int test7() {
  int days = 4;
  Calendar *calendar;
  Task_info *info;

  if (init_calendar("yummy", days, comp_minutes, NULL, &calendar) == SUCCESS) {
    info = create_task_info(1, "sandwich");
    if (add_event(calendar, "lunch", 100, 60, info, 1) == SUCCESS) {
      info = create_task_info(3, "pasta");
      if (add_event(calendar, "dinner", 600, 30, info, 1) == SUCCESS) {
        if (get_event_info(calendar, "lunch") == SUCCESS) {
          return destroy_calendar(calendar);
        }
      }
    }
  }
}
int main() {
  int result = SUCCESS;

  /***** Starting memory checking *****/
  start_memory_check();
  /***** Starting memory checking *****/

  printf("TEST 1:\n");
  if (test1() == FAILURE) {
    printf("Test 1 failed\n");
    result = FAILURE;
  }
  printf("\nTEST 2:\n");
  if (test2() == FAILURE) {
    printf("Test 2 failed\n");
    result = FAILURE;
  }
  printf("\nTEST 3:\n");
  if (test3() == FAILURE) {
    printf("Test 3 failed\n");
    result = FAILURE;
  }
  printf("\nTEST 4:\n");
  if (test4() == FAILURE) {
    printf("Test 4 failed\n");
    result = FAILURE;
  }
  printf("\nTEST 5:\n");
  if (test5() == FAILURE) {
    printf("Test 5 failed\n");
    result = FAILURE;
  }
  printf("\nTEST 6:\n");
  if (test6() == FAILURE) {
    printf("Test 6 failed\n");
    result = FAILURE;
  }
  printf("\nTEST 7:\n");
  if (test7() == FAILURE) {
    printf("Test 7 failed\n");
    result = FAILURE;
  }

  /****** Gathering memory checking info *****/
  stop_memory_check();
  /****** Gathering memory checking info *****/

  if (result == FAILURE) {
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
