// student.c

#include <stdio.h>
#include <string.h>
#include "student.h"
#include "trainer.h" // For accessing Trainer structure in delete_student

// Function to add a new student
int add_student(Student students[], int *student_count) {
    if (*student_count >= MAX_STUDENTS) {
        printf("Maximum number of students reached.\n");
        return -1;
    }

    Student new_student;
    new_student.student_id = (*student_count > 0) ? students[*student_count - 1].student_id + 1 : 1; // Unique ID

    printf("Enter student name: ");
    fgets(new_student.name, MAX_NAME_LENGTH, stdin);
    new_student.name[strcspn(new_student.name, "\n")] = '\0'; // Remove newline

    printf("Enter ELO rating: ");
    scanf("%d", &new_student.elo_rating);
    getchar(); // Consume newline

    printf("Enter number of learning goals (max %d): ", MAX_GOALS);
    int num_goals;
    scanf("%d", &num_goals);
    getchar();
    if (num_goals > MAX_GOALS) num_goals = MAX_GOALS;
    for(int i = 0; i < num_goals; i++) {
        printf("Enter learning goal %d: ", i+1);
        fgets(new_student.learning_goals[i], MAX_NAME_LENGTH, stdin);
        new_student.learning_goals[i][strcspn(new_student.learning_goals[i], "\n")] = '\0';
    }

    // Initialize availability to 0
    for(int i = 0; i < MAX_AVAILABILITY; i++) {
        new_student.availability[i] = 0;
    }
    printf("Enter number of available time slots (max %d): ", MAX_AVAILABILITY);
    int num_slots;
    scanf("%d", &num_slots);
    getchar();
    for(int i = 0; i < num_slots && i < MAX_AVAILABILITY; i++) {
        printf("Enter available time slot %d (0 to %d): ", i+1, MAX_AVAILABILITY-1);
        int slot;
        scanf("%d", &slot);
        getchar();
        if(slot >=0 && slot < MAX_AVAILABILITY) {
            new_student.availability[slot] = 1;
        }
    }

    // Preferred coaching styles
    for(int i = 0; i < MAX_PREFERENCES; i++) {
        printf("Enter preferred coaching style %d (aggressive, defensive, positional) or 'none' to skip: ", i+1);
        char style[MAX_NAME_LENGTH];
        fgets(style, MAX_NAME_LENGTH, stdin);
        style[strcspn(style, "\n")] = '\0';
        if(strcmp(style, "none") == 0) {
            strcpy(new_student.preferred_coaching_styles[i], "");
        }
        else {
            strcpy(new_student.preferred_coaching_styles[i], style);
        }
    }

    new_student.current_trainer_id = -1; // No trainer assigned initially

    // Weak areas
    for(int i = 0; i < MAX_GOALS; i++) {
        printf("Enter weak area %d or 'none' to skip: ", i+1);
        char weak[MAX_NAME_LENGTH];
        fgets(new_student.weak_areas[i], MAX_NAME_LENGTH, stdin);
        new_student.weak_areas[i][strcspn(new_student.weak_areas[i], "\n")] = '\0';
    }

    printf("Enter number of games played: ");
    scanf("%d", &new_student.games_played);
    getchar();

    printf("Enter number of games won: ");
    scanf("%d", &new_student.games_won);
    getchar();

    printf("Enter number of puzzles solved: ");
    scanf("%d", &new_student.puzzles_solved);
    getchar();

    // Rating changes
    printf("Enter rating changes for the last 12 months:\n");
    for(int i = 0; i < MAX_PERFORMANCE_DATA; i++) {
        printf("Month %d: ", i+1);
        scanf("%d", &new_student.rating_changes[i]);
        getchar();
    }

    students[*student_count] = new_student;
    (*student_count)++;

    printf("Student added with ID %d.\n", new_student.student_id);
    return new_student.student_id;
}

// Function to delete a student
int delete_student(Student students[], int *student_count, int student_id, Trainer trainers[], int trainer_count) {
    int found = -1;
    for(int i = 0; i < *student_count; i++) {
        if(students[i].student_id == student_id) {
            found = i;
            break;
        }
    }
    if(found == -1) {
        printf("Student with ID %d not found.\n", student_id);
        return -1;
    }

    // If student has a trainer, remove student from trainer's list
    if(students[found].current_trainer_id != -1) {
        // Find the trainer
        for(int t = 0; t < trainer_count; t++) {
            if(trainers[t].trainer_id == students[found].current_trainer_id) {
                // Remove student from trainer's assigned_students
                int s_found = -1;
                for(int s = 0; s < trainers[t].current_student_count; s++) {
                    if(trainers[t].assigned_students[s] == student_id) {
                        s_found = s;
                        break;
                    }
                }
                if(s_found != -1) {
                    // Shift students in the array
                    for(int s = s_found; s < trainers[t].current_student_count -1; s++) {
                        trainers[t].assigned_students[s] = trainers[t].assigned_students[s+1];
                    }
                    trainers[t].assigned_students[trainers[t].current_student_count -1] = -1;
                    trainers[t].current_student_count--;
                }
                break;
            }
        }
    }

    // Shift students in the array to delete
    for(int i = found; i < (*student_count) -1; i++) {
        students[i] = students[i+1];
    }
    (*student_count)--;

    printf("Student with ID %d deleted.\n", student_id);
    return 0;
}

// Function to list all students
void list_students(const Student students[], int student_count) {
    printf("---- List of Students ----\n");
    for(int i = 0; i < student_count; i++) {
        printf("ID: %d, Name: %s, ELO: %d, Trainer ID: %d\n", 
            students[i].student_id, students[i].name, students[i].elo_rating, students[i].current_trainer_id);
        // Additional details can be printed as needed
    }
    printf("--------------------------\n");
}

// Function to sort students based on ELO and games won
void sort_students(Student students[], int student_count) {
    for(int i = 0; i < student_count -1; i++) {
        for(int j = 0; j < student_count - i -1; j++) {
            if(students[j].elo_rating < students[j+1].elo_rating || 
               (students[j].elo_rating == students[j+1].elo_rating && students[j].games_won < students[j+1].games_won)) {
                // Swap
                Student temp = students[j];
                students[j] = students[j+1];
                students[j+1] = temp;
            }
        }
    }
    printf("Students sorted successfully.\n");
}

// Function to find students with successive ELO improvements
void find_students_with_improved_rating(const Student students[], int student_count, Student improved_students[], int *improved_count) {
    *improved_count = 0;
    for(int i = 0; i < student_count; i++) {
        int improved = 1;
        for(int j = 1; j < MAX_PERFORMANCE_DATA; j++) {
            if(students[i].rating_changes[j] <= students[i].rating_changes[j-1]) {
                improved = 0;
                break;
            }
        }
        if(improved) {
            improved_students[*improved_count] = students[i];
            (*improved_count)++;
        }
    }

    // Sort improved_students by total ELO gains in decreasing order
    for(int i = 0; i < *improved_count -1; i++) {
        for(int j = 0; j < *improved_count - i -1; j++) {
            int gain_j = 0, gain_j1 = 0;
            for(int k = 0; k < MAX_PERFORMANCE_DATA; k++) {
                gain_j += students[j].rating_changes[k];
                gain_j1 += students[j+1].rating_changes[k];
            }
            if(gain_j < gain_j1) {
                // Swap
                Student temp = improved_students[j];
                improved_students[j] = improved_students[j+1];
                improved_students[j+1] = temp;
            }
        }
    }

    printf("Found %d students with successive rating improvements.\n", *improved_count);
}

