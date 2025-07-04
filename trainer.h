// trainer.h

#ifndef TRAINER_H
#define TRAINER_H

#include "functions.h"
#include "student.h"

// Structure to hold trainer details
typedef struct {
    int trainer_id;
    char name[MAX_NAME_LENGTH];
    int elo_rating;
    char coaching_style[MAX_NAME_LENGTH]; // "aggressive", "defensive", "positional"
    int availability[MAX_AVAILABILITY]; // 1 if available, 0 otherwise
    int experience_years;
    int min_elo; // E
    int max_students;
    int assigned_students[MAX_STUDENTS]; // Array of student IDs
    int current_student_count;
} Trainer;

// Function declarations
int add_trainer(Trainer trainers[], int *trainer_count);
int delete_trainer(Trainer trainers[], int *trainer_count, int trainer_id, Student students[], int student_count);
void list_trainers(const Trainer trainers[], int trainer_count);
void list_students_per_trainer(const Trainer trainers[], int trainer_count, const Student students[], int student_count);
void sort_trainers_by_popularity(Trainer trainers[], int trainer_count, const Student students[], int student_count);
void sort_trainers_by_strength(Trainer trainers[], int trainer_count, const Student students[], int student_count);
int assign_trainer_to_student(Trainer trainers[], int trainer_count, Student students[], int student_count, int student_id);

#endif // TRAINER_H
