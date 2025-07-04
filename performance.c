// performance.c

#include <stdio.h>
#include "performance.h"

// Function to update student performance
void update_student_performance(Student students[], int student_count) {
    int id;
    printf("Enter student ID to update performance: ");
    scanf("%d", &id);
    getchar();

    // Find the student
    int s_index = -1;
    for(int i =0; i < student_count; i++) {
        if(students[i].student_id == id) {
            s_index = i;
            break;
        }
    }
    if(s_index == -1) {
        printf("Student with ID %d not found.\n", id);
        return;
    }

    printf("Updating performance for %s:\n", students[s_index].name);

    printf("Enter number of games played this month: ");
    int gp;
    scanf("%d", &gp);
    getchar();
    students[s_index].games_played += gp;

    printf("Enter number of games won this month: ");
    int gw;
    scanf("%d", &gw);
    getchar();
    students[s_index].games_won += gw;

    printf("Enter number of puzzles solved this month: ");
    int ps;
    scanf("%d", &ps);
    getchar();
    students[s_index].puzzles_solved += ps;

    printf("Enter ELO rating change this month: ");
    int rc;
    scanf("%d", &rc);
    getchar();
    // Shift rating changes to the left and add new change at the end
    for(int i =0; i < MAX_PERFORMANCE_DATA -1; i++) {
        students[s_index].rating_changes[i] = students[s_index].rating_changes[i+1];
    }
    students[s_index].rating_changes[MAX_PERFORMANCE_DATA -1] = rc;

    printf("Performance updated for student ID %d.\n", id);
}

// Function to display a student's performance
void display_student_performance(const Student students[], int student_count, int student_id) {
    // Find the student
    int s_index = -1;
    for(int i =0; i < student_count; i++) {
        if(students[i].student_id == student_id) {
            s_index = i;
            break;
        }
    }
    if(s_index == -1) {
        printf("Student with ID %d not found.\n", student_id);
        return;
    }

    printf("---- Performance of %s ----\n", students[s_index].name);
    printf("Games Played: %d\n", students[s_index].games_played);
    printf("Games Won: %d\n", students[s_index].games_won);
    printf("Puzzles Solved: %d\n", students[s_index].puzzles_solved);
    printf("Monthly ELO Changes:\n");
    for(int i =0; i < MAX_PERFORMANCE_DATA; i++) {
        printf("Month %d: %d\n", i+1, students[s_index].rating_changes[i]);
    }
    printf("-------------------------------\n");
}
