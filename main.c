// main.c

#include <stdio.h>
#include "student.h"
#include "trainer.h"
#include "performance.h"

int main() {
    Student students[MAX_STUDENTS];
    int student_count =0;

    Trainer trainers[MAX_TRAINERS];
    int trainer_count =0;

    int choice;
    while(1) {
        printf("\n---- Chess Coaching Platform ----\n");
        printf("1. Add Student\n");
        printf("2. Add Trainer\n");
        printf("3. Delete Student\n");
        printf("4. Delete Trainer\n");
        printf("5. List All Students\n");
        printf("6. List All Trainers\n");
        printf("7. Assign Trainer to Student\n");
        printf("8. List Students per Trainer\n");
        printf("9. Sort Students by ELO and Games Won\n");
        printf("10. Sort Trainers by Popularity\n");
        printf("11. Sort Trainers by Strength\n");
        printf("12. Update Student Performance\n");
        printf("13. Display Student Performance\n");
        printf("14. Find Students with Improved Ratings\n");
        printf("15. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline

        if(choice ==1) {
            add_student(students, &student_count);
        }
        else if(choice ==2) {
            add_trainer(trainers, &trainer_count);
        }
        else if(choice ==3) {
            int sid;
            printf("Enter Student ID to delete: ");
            scanf("%d", &sid);
            getchar();
            delete_student(students, &student_count, sid, trainers, trainer_count);
        }
        else if(choice ==4) {
            int tid;
            printf("Enter Trainer ID to delete: ");
            scanf("%d", &tid);
            getchar();
            delete_trainer(trainers, &trainer_count, tid, students, student_count);
        }
        else if(choice ==5) {
            list_students(students, student_count);
        }
        else if(choice ==6) {
            list_trainers(trainers, trainer_count);
        }
        else if(choice ==7) {
            int sid;
            printf("Enter Student ID to assign a trainer: ");
            scanf("%d", &sid);
            getchar();
            assign_trainer_to_student(trainers, trainer_count, students, student_count, sid);
        }
        else if(choice ==8) {
            list_students_per_trainer(trainers, trainer_count, students, student_count);
        }
        else if(choice ==9) {
            sort_students(students, student_count);
        }
        else if(choice ==10) {
            sort_trainers_by_popularity(trainers, trainer_count, students, student_count);
        }
        else if(choice ==11) {
            sort_trainers_by_strength(trainers, trainer_count, students, student_count);
        }
        else if(choice ==12) {
            update_student_performance(students, student_count);
        }
        else if(choice ==13) {
            int sid;
            printf("Enter Student ID to display performance: ");
            scanf("%d", &sid);
            getchar();
            display_student_performance(students, student_count, sid);
        }
        else if(choice ==14) {
            Student improved_students[MAX_STUDENTS];
            int improved_count =0;
            find_students_with_improved_rating(students, student_count, improved_students, &improved_count);
            printf("---- Students with Improved Ratings ----\n");
            for(int i =0; i < improved_count; i++) {
                printf("ID: %d, Name: %s, ELO: %d\n", improved_students[i].student_id, improved_students[i].name, improved_students[i].elo_rating);
            }
            printf("----------------------------------------\n");
        }
        else if(choice ==15) {
            printf("Exiting the program. Goodbye!\n");
            break;
        }
        else {
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}