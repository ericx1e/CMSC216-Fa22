/* Eric Xie
 * 118862019
 * exie
 * Project 4 */

#include "calendar.h"
#include "event.h"
#include <stdlib.h>
#include <string.h>

/* Helper function to free all dynamically
 * allocated memory associated with an event */
void free_event(Calendar *calendar, Event *event) {
  free(event->name);
  if (event->info != NULL && calendar->free_info_func != NULL) {
    calendar->free_info_func(event->info);
  }
  free(event);
}

/* Dynamically allocates memory for calendar and
 * name and intializates other fields accordingly */
int init_calendar(const char *name, int days,
                  int (*comp_func)(const void *ptr1, const void *ptr2),
                  void (*free_info_func)(void *ptr), Calendar **calendar) {
  Calendar *new_calendar;
  Event **events;
  char *name_ptr;

  if (!calendar || !name || days < 1) {
    return FAILURE;
  }

  new_calendar = malloc(sizeof(Calendar));
  events = calloc(days, sizeof(Event *));
  name_ptr = malloc(strlen(name) + 1);
  if (new_calendar == NULL || events == NULL ||
      !name_ptr) { /* Memorry allocation failed */
    return FAILURE;
  }

  new_calendar->days = days;
  new_calendar->total_events = 0;
  new_calendar->comp_func = comp_func;
  new_calendar->free_info_func = free_info_func;
  strcpy(name_ptr, name);
  new_calendar->name = name_ptr;
  new_calendar->events = events;
  *calendar = new_calendar;
  return SUCCESS;
}

/* Prints the every day's events of the calendar
 * with additional info if print_all is true */
int print_calendar(Calendar *calendar, FILE *output_stream, int print_all) {
  int i;
  Event *current;

  if (!calendar || !output_stream) {
    return FAILURE;
  }
  if (print_all) {
    fprintf(output_stream, "Calendar's Name: \"%s\"\n", calendar->name);
    fprintf(output_stream, "Days: %d\n", calendar->days);
    fprintf(output_stream, "Total Events: %d\n\n", calendar->total_events);
  }
  fprintf(output_stream, "**** Events ****\n");
  if (calendar->total_events > 0) {
    for (i = 0; i < calendar->days; i++) {
      fprintf(output_stream, "Day %d\n", i + 1);
      current = calendar->events[i];
      while (current != NULL) {
        fprintf(output_stream,
                "Event's Name: \"%s\", Start_time: %d, Duration: %d\n",
                current->name, current->start_time, current->duration_minutes);
        current = current->next;
      }
    }
  }

  return SUCCESS;
}

int add_event(Calendar *calendar, const char *name, int start_time,
              int duration_minutes, void *info, int day) {
  Event *current;
  Event *head;
  Event *new_event;
  char *name_ptr;

  /* Use find_event to check if the event already exists in the calendar */
  if (!calendar || !name || start_time < 0 || start_time > 2400 ||
      duration_minutes <= 0 || day < 1 || day > calendar->days ||
      calendar->comp_func == NULL ||
      find_event(calendar, name, &new_event) == SUCCESS) {
    return FAILURE;
  }

  new_event = malloc(sizeof(Event));
  name_ptr = malloc(strlen(name) + 1);
  if (new_event == NULL || name_ptr == NULL) {
    return FAILURE;
  }
  strcpy(name_ptr, name);
  new_event->name = name_ptr;
  new_event->start_time = start_time;
  new_event->duration_minutes = duration_minutes;
  new_event->info = info;
  new_event->next = NULL;

  head = calendar->events[day - 1];
  if (head == NULL || calendar->comp_func(new_event, head) < 0) {
    new_event->next = head;
    calendar->events[day - 1] = new_event;
  } else {
    current = head;
    /* Traverse the events, until current->next is in
     * order with new_event. At this point, current is the
     * node that should preced the new event */
    while (current->next != NULL &&
           calendar->comp_func(new_event, current->next) > 0) {
      current = current->next;
    }
    new_event->next = current->next;
    current->next = new_event;
  }
  calendar->total_events++;

  return SUCCESS;
}

/* Searches the calendar for an event matching the
 * name parameter and updates the out event pointer */
int find_event(Calendar *calendar, const char *name, Event **event) {
  int i;

  if (!calendar || !name) {
    return FAILURE;
  }

  /* To find an event, we can call run_event_in_day over each day */
  for (i = 0; i < calendar->days; i++) {
    if (find_event_in_day(calendar, name, i + 1, event) == SUCCESS) {
      return SUCCESS;
    }
  }

  /* We have already searched every event in the
   * calendar, so we failed to find the desired event */
  return FAILURE;
}

/* Traverses the events associated with the given day,
 * returning SUCCESS if there is an event matching the name parameter */
int find_event_in_day(Calendar *calendar, const char *name, int day,
                      Event **event) {
  Event *current;

  if (!calendar || !name || day < 1 || day > calendar->days) {
    return FAILURE;
  }
  current = calendar->events[day - 1];
  while (current != NULL) {
    if (strcmp(current->name, name) == 0) {
      if (event != NULL) {
        *event = current;
      }
      return SUCCESS;
    }
    current = current->next;
  }

  return FAILURE;
}

int remove_event(Calendar *calendar, const char *name) {
  Event *prev;
  Event *current;
  int i;

  if (!calendar || !name) {
    return FAILURE;
  }

  for (i = 0; i < calendar->days; i++) {
    prev = NULL;
    current = calendar->events[i];
    while (current != NULL) {
      /* First, search for the event to remove */
      if (strcmp(current->name, name) == 0) {
        /* If prev is null, then we are removing the first event in a day */
        if (prev == NULL) {
          calendar->events[i] = current->next;
        } else {
          /* To remove an event, set the previous event's
           * next to the removed event's next */
          prev->next = current->next;
        }
        free_event(calendar, current);
        calendar->total_events--;
        return SUCCESS;
      }
      prev = current;
      current = current->next;
    }
  }

  /* Event was not found */
  return FAILURE;
}

void *get_event_info(Calendar *calendar, const char *name) {
  Event *event;

  if (find_event(calendar, name, &event) == FAILURE) {
    return NULL;
  }

  return event->info;
}

/* Clears every event the calendar, */
int clear_calendar(Calendar *calendar) {
  int i;

  if (!calendar) {
    return FAILURE;
  }
  /* Call clear day on each day in the calendar */
  for (i = 0; i < calendar->days; i++) {
    clear_day(calendar, i + 1);
  }

  return SUCCESS;
}

/* Clears all events of a specific day, freeing all
 * associated memory and adjusting total_events accordingly */
int clear_day(Calendar *calendar, int day) {
  Event *current;
  Event *temp;

  if (!calendar || day < 1 || day > calendar->days) {
    return FAILURE;
  }

  current = calendar->events[day - 1];
  while (current != NULL) {
    /* Keep a reference to current before we increment it */
    temp = current;
    current = current->next;
    free_event(calendar, temp);
    calendar->total_events--;
  }
  calendar->events[day - 1] = NULL;

  return SUCCESS;
}

/* Destroys the calendar, freeing all dynamically allocated memory */
int destroy_calendar(Calendar *calendar) {
  if (!calendar) {
    return FAILURE;
  }
  /* First clear calendar, freeing all memory associated with events */
  clear_calendar(calendar);
  free(calendar->name);
  free(calendar->events);
  free(calendar);

  return SUCCESS;
}