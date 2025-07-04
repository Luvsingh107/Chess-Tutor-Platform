// student.h

#ifndef STUDENT_H
#define STUDENT_H

#include "functions.h"

// Structure to hold student details
typedef struct {
    int student_id;
    char name[MAX_NAME_LENGTH];
    int elo_rating;
    char learning_goals[MAX_GOALS][MAX_NAME_LENGTH];
    int availability[MAX_AVAILABILITY]; // 1 if available, 0 otherwise
    char preferred_coaching_styles[MAX_PREFERENCES][MAX_NAME_LENGTH]; // e.g., "aggressive"
    int current_trainer_id; // -1 if none assigned
    char weak_areas[MAX_GOALS][MAX_NAME_LENGTH];
    int games_played;
    int games_won;
    int puzzles_solved;
    int rating_changes[MAX_PERFORMANCE_DATA]; // Rating changes for the last 12 months
} Student;

// Function declarations
int add_student(Student students[], int *student_count);
int delete_student(Student students[], int *student_count, int student_id, struct Trainer trainers[], int trainer_count);
void list_students(const Student students[], int student_count);
void sort_students(Student students[], int student_count);
void find_students_with_improved_rating(const Student students[], int student_count, Student improved_students[], int *improved_count);

#endif // STUDENT_H
