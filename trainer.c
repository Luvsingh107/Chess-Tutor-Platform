// trainer.c

#include <stdio.h>
#include <string.h>
#include "trainer.h"

// Function to add a new trainer
int add_trainer(Trainer trainers[], int *trainer_count) {
    if(*trainer_count >= MAX_TRAINERS) {
        printf("Maximum number of trainers reached.\n");
        return -1;
    }

    Trainer new_trainer;
    new_trainer.trainer_id = (*trainer_count > 0) ? trainers[*trainer_count -1].trainer_id +1 : 1; // Unique ID

    printf("Enter trainer name: ");
    fgets(new_trainer.name, MAX_NAME_LENGTH, stdin);
    new_trainer.name[strcspn(new_trainer.name, "\n")] = '\0';

    printf("Enter ELO rating: ");
    scanf("%d", &new_trainer.elo_rating);
    getchar(); // Consume newline

    printf("Enter coaching style (aggressive, defensive, positional): ");
    fgets(new_trainer.coaching_style, MAX_NAME_LENGTH, stdin);
    new_trainer.coaching_style[strcspn(new_trainer.coaching_style, "\n")] = '\0';

    // Initialize availability to 0
    for(int i = 0; i < MAX_AVAILABILITY; i++) {
        new_trainer.availability[i] = 0;
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
            new_trainer.availability[slot] =1;
        }
    }

    printf("Enter years of coaching experience: ");
    scanf("%d", &new_trainer.experience_years);
    getchar();

    printf("Enter minimum ELO rating to admit students: ");
    scanf("%d", &new_trainer.min_elo);
    getchar();

    printf("Enter maximum number of students the trainer can take: ");
    scanf("%d", &new_trainer.max_students);
    getchar();

    // Initialize assigned students
    for(int i =0; i < MAX_STUDENTS; i++) {
        new_trainer.assigned_students[i] = -1; // -1 indicates no student
    }
    new_trainer.current_student_count =0;

    trainers[*trainer_count] = new_trainer;
    (*trainer_count)++;

    printf("Trainer added with ID %d.\n", new_trainer.trainer_id);
    return new_trainer.trainer_id;
}

// Function to delete a trainer
int delete_trainer(Trainer trainers[], int *trainer_count, int trainer_id, Student students[], int student_count) {
    int found = -1;
    for(int i =0; i < *trainer_count; i++) {
        if(trainers[i].trainer_id == trainer_id) {
            found = i;
            break;
        }
    }
    if(found == -1) {
        printf("Trainer with ID %d not found.\n", trainer_id);
        return -1;
    }

    // Remove trainer's assigned students
    for(int i =0; i < trainers[found].current_student_count; i++) {
        int student_id = trainers[found].assigned_students[i];
        // Find student and set current_trainer_id to -1
        for(int j =0; j < student_count; j++) {
            if(students[j].student_id == student_id) {
                students[j].current_trainer_id = -1;
                break;
            }
        }
    }

    // Shift trainers in the array to delete
    for(int i = found; i < (*trainer_count) -1; i++) {
        trainers[i] = trainers[i+1];
    }
    (*trainer_count)--;

    printf("Trainer with ID %d deleted.\n", trainer_id);
    return 0;
}

// Function to list all trainers
void list_trainers(const Trainer trainers[], int trainer_count) {
    printf("---- List of Trainers ----\n");
    for(int i =0; i < trainer_count; i++) {
        printf("ID: %d, Name: %s, ELO: %d, Coaching Style: %s, Experience: %d years, Min ELO: %d, Max Students: %d\n", 
            trainers[i].trainer_id, trainers[i].name, trainers[i].elo_rating, trainers[i].coaching_style, 
            trainers[i].experience_years, trainers[i].min_elo, trainers[i].max_students);
        // Additional details can be printed as needed
    }
    printf("--------------------------\n");
}

// Function to list students assigned to each trainer
void list_students_per_trainer(const Trainer trainers[], int trainer_count, const Student students[], int student_count) {
    for(int i =0; i < trainer_count; i++) {
        printf("Trainer ID %d, Name: %s\n", trainers[i].trainer_id, trainers[i].name);
        printf("Assigned Students:\n");
        for(int j =0; j < trainers[i].current_student_count; j++) {
            int student_id = trainers[i].assigned_students[j];
            for(int k =0; k < student_count; k++) {
                if(students[k].student_id == student_id) {
                    printf("\tID: %d, Name: %s\n", students[k].student_id, students[k].name);
                    break;
                }
            }
        }
        printf("\n");
    }
}

// Function to sort trainers based on popularity
void sort_trainers_by_popularity(Trainer trainers[], int trainer_count, const Student students[], int student_count) {
    for(int i =0; i < trainer_count -1; i++) {
        for(int j =0; j < trainer_count -i -1; j++) {
            int pop_j = trainers[j].current_student_count;
            int pop_j1 = trainers[j+1].current_student_count;
            if(pop_j < pop_j1) {
                // Swap
                Trainer temp = trainers[j];
                trainers[j] = trainers[j+1];
                trainers[j+1] = temp;
            }
            else if(pop_j == pop_j1) {
                // Calculate average ELO
                int sum_j =0, sum_j1 =0;
                for(int s =0; s < trainers[j].current_student_count; s++) {
                    for(int t =0; t < student_count; t++) {
                        if(students[t].student_id == trainers[j].assigned_students[s]) {
                            sum_j += students[t].elo_rating;
                            break;
                        }
                    }
                }
                for(int s =0; s < trainers[j+1].current_student_count; s++) {
                    for(int t =0; t < student_count; t++) {
                        if(students[t].student_id == trainers[j+1].assigned_students[s]) {
                            sum_j1 += students[t].elo_rating;
                            break;
                        }
                    }
                }
                double avg_j = (pop_j >0) ? (double)sum_j / pop_j : 0;
                double avg_j1 = (pop_j1 >0) ? (double)sum_j1 / pop_j1 :0;
                if(avg_j < avg_j1) {
                    // Swap
                    Trainer temp = trainers[j];
                    trainers[j] = trainers[j+1];
                    trainers[j+1] = temp;
                }
            }
        }
    }
    printf("Trainers sorted by popularity.\n");
}

// Function to sort trainers based on strength (average ELO of students)
void sort_trainers_by_strength(Trainer trainers[], int trainer_count, const Student students[], int student_count) {
    for(int i =0; i < trainer_count -1; i++) {
        for(int j =0; j < trainer_count -i -1; j++) {
            // Calculate average ELO of trainers[j] and trainers[j+1]
            double avg_j =0, avg_j1 =0;
            if(trainers[j].current_student_count >0) {
                for(int s =0; s < trainers[j].current_student_count; s++) {
                    for(int t =0; t < student_count; t++) {
                        if(students[t].student_id == trainers[j].assigned_students[s]) {
                            avg_j += students[t].elo_rating;
                            break;
                        }
                    }
                }
                avg_j /= trainers[j].current_student_count;
            }
            if(trainers[j+1].current_student_count >0) {
                for(int s =0; s < trainers[j+1].current_student_count; s++) {
                    for(int t =0; t < student_count; t++) {
                        if(students[t].student_id == trainers[j+1].assigned_students[s]) {
                            avg_j1 += students[t].elo_rating;
                            break;
                        }
                    }
                }
                avg_j1 /= trainers[j+1].current_student_count;
            }

            if(avg_j < avg_j1) {
                // Swap
                Trainer temp = trainers[j];
                trainers[j] = trainers[j+1];
                trainers[j+1] = temp;
            }
        }
    }
    printf("Trainers sorted by strength.\n");
}

// Function to assign a trainer to a student based on matching criteria
int assign_trainer_to_student(Trainer trainers[], int trainer_count, Student students[], int student_count, int student_id) {
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
        return -1;
    }

    if(students[s_index].current_trainer_id != -1) {
        printf("Student already has a trainer assigned.\n");
        return -1;
    }

    // List of candidate trainers
    Trainer candidates[MAX_TRAINERS];
    int candidate_count =0;

    // First, find all trainers with overlapping availability
    for(int t =0; t < trainer_count; t++) {
        int available =0;
        for(int a =0; a < MAX_AVAILABILITY; a++) {
            if(students[s_index].availability[a] && trainers[t].availability[a]) {
                available =1;
                break;
            }
        }
        if(available && trainers[t].current_student_count < trainers[t].max_students) {
            candidates[candidate_count++] = trainers[t];
        }
    }

    // Now, try to match preferred coaching styles in order
    for(int p =0; p < MAX_PREFERENCES; p++) {
        if(strcmp(students[s_index].preferred_coaching_styles[p], "") ==0)
            continue;
        for(int c =0; c < candidate_count; c++) {
            if(strcmp(candidates[c].coaching_style, students[s_index].preferred_coaching_styles[p]) ==0 &&
               students[s_index].elo_rating >= candidates[c].min_elo) {
                // Assign this trainer
                for(int t =0; t < trainer_count; t++) {
                    if(trainers[t].trainer_id == candidates[c].trainer_id) {
                        trainers[t].assigned_students[trainers[t].current_student_count] = students[s_index].student_id;
                        trainers[t].current_student_count++;
                        students[s_index].current_trainer_id = trainers[t].trainer_id;
                        printf("Trainer %d assigned to student %d.\n", trainers[t].trainer_id, students[s_index].student_id);
                        return trainers[t].trainer_id;
                    }
                }
            }
        }
    }

    // If no match found, try to assign based on availability only, compromise on coaching style and ELO
    for(int c =0; c < candidate_count; c++) {
        // Assign the first available
        for(int t =0; t < trainer_count; t++) {
            if(trainers[t].trainer_id == candidates[c].trainer_id) {
                trainers[t].assigned_students[trainers[t].current_student_count] = students[s_index].student_id;
                trainers[t].current_student_count++;
                students[s_index].current_trainer_id = trainers[t].trainer_id;
                printf("Trainer %d assigned to student %d.\n", trainers[t].trainer_id, students[s_index].student_id);
                return trainers[t].trainer_id;
            }
        }
    }

    printf("No suitable trainer found for student %d.\n", students[s_index].student_id);
    return -1;
}