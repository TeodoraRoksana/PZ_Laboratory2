#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#define NUMBER_STR 20 //maximum tape length
#define MARKS 6 //number of estimates
#define K 10 //amount of students

void read_file_with_ListStudent(struct Students_copy arr_students[K]);
void filling_list(
	struct Students** p,
	struct Students** start,
	struct Students_copy arr_students[K]);
void print_list(struct Students* p, struct Students* start);
void add_new_item_to_list(struct Students** p, struct Students** start);
void average_grade(
	struct Students** p,
	struct Students** start,
	double average[2 * K]);
double average_grade_for_all_students(double average[2 * K]);
void delete_students(
	struct Students** p,
	struct Students** start,
	double average[2 * K],
	double average_num);
void free_memory(struct Students** p);


//main structure
struct Students {
	char surname[NUMBER_STR];
	char name[NUMBER_STR];
	char birthday[NUMBER_STR];
	int marks[MARKS];
	struct Students* next;
};
//structure for reading the file
struct Students_copy {
	char surname[NUMBER_STR];
	char name[NUMBER_STR];
	char birthday[NUMBER_STR];
	int marks[MARKS];
};

int main() {
	//an array for various calculations associated mostly with estimates
	double average[2 * K] = { 0 };

	//an array for transferring data from a file to a list
	struct Students_copy arr_students[K];

	//read data from a file
	read_file_with_ListStudent(arr_students);

	//creating the first list item
	struct Students* p = (struct Students*)malloc(sizeof(struct Students));
	
	struct Students* start = NULL;

	//create list//
	filling_list(&p, &start, arr_students);

	//list output
	print_list(p, start);

	//adding a new item
	add_new_item_to_list(&p, &start);

	//list output after adding
	printf("After adding:\n");
	print_list(p, start);

	//finding the average score of each student
	average_grade(&p, &start, average);
	//finding the average score between all students
	double average_num = average_grade_for_all_students(average);

	//removing students with a grade point average that is lower
	//than the grade point average in the list
	delete_students(&p, &start, average, average_num);

	//output of the total average score and the edited list
	printf("\n average number = %lf\n", average_num);
	print_list(p, start);

	//free memory
	free_memory(&p);

	return 0;
}

//reads data from a file
void read_file_with_ListStudent(struct Students_copy arr_students[K]) {

	FILE* finput = fopen("D:/students.bin", "rb");

	if (finput == NULL) {
		printf("error");
		return;
	}

	for (int i = 0; i < K; ++i) {
		fread(&arr_students[i], sizeof(struct Students_copy), 1, finput);
	}
}

//fills the list
void filling_list(
	struct Students** p,
	struct Students** start,
	struct Students_copy arr_students[K]) {

	int r = 0;
	do {
		strcpy((*p)->surname, arr_students[r].surname);
		strcpy((*p)->name, arr_students[r].name);
		strcpy((*p)->birthday, arr_students[r].birthday);
		for (int i = 0; i < MARKS; ++i) {
			(*p)->marks[i] = arr_students[r].marks[i];
		}

		if ((*start) == NULL) (*start) = (*p);

		struct Students* pp = (struct Students*)malloc(sizeof(struct Students));

		(*p)->next = pp;
		(*p) = pp;
		r++;
	} while (r < K);

	(*p)->next = NULL;
	(*p) = (*start);
}

//display the list on the screen
void print_list(struct Students* p, struct Students* start) {
	printf("\n____________________________________________________________________\n");
	while (p->next != NULL) {
		printf("%s    \t%s    \t%s    \t", p->surname, p->name, p->birthday);
		for (int i = 0; i < MARKS; ++i) {
			printf("%3d", p->marks[i]);
		}
		printf("\n____________________________________________________________________\n");
		p = p->next;
	}
	p = start;
}

//adds a new item
void add_new_item_to_list(struct Students** p, struct Students** start) {
	struct Students* pp = (struct Students*)malloc(sizeof(struct Students));

	printf("Enter surname:\n");
	scanf("%s", &pp->surname);

	printf("Enter name:\n");
	scanf("%s", &pp->name);

	printf("Enter data of birthday:\n");
	scanf("%s", &pp->birthday);

	for (int i = 0; i < MARKS; ++i) {
		printf("Enter mark:\n");
		scanf("%d", &pp->marks[i]);
		getchar();
	}
	(*p) = (*start);
	
	int check = -2;
	
	check = strcmp(pp->surname, (*p)->surname);

	if (check < 0)
	{
		if ((*p) == (*start))
		{
			pp->next = (*p);
			(*start) = pp;
		}
	}
	else {
		struct Students* ppp = (*start);
		(*p) = (*p)->next;
		while (1) {
			check = strcmp(pp->surname, (*p)->surname);

			if (check < 0)
			{
				pp->next = ppp->next;
				ppp->next = pp;
				break;
			}
			if ((*p)->next == NULL)
			{
				check = strcmp(pp->surname, ppp->surname);
				if (check > 0)
				{
					ppp->next = pp;
					pp->next = (*p);
					break;
				}
			}
			ppp = ppp->next;
			(*p) = (*p)->next;
		}
	}

	(*p) = (*start);
}

//finding the average score of each student
void average_grade(
	struct Students** p,
	struct Students** start,
	double average[2 * K]) {

	int r = 0, sum = 0;
	while ((*p)->next != NULL) {
		for (int i = 0; i < MARKS; ++i) {
			sum += (*p)->marks[i];
		}
		average[r] = sum / 6.0;
		sum = 0;

		(*p) = (*p)->next;
		r++;
	}
	(*p) = (*start);
}

//finding the average score between all students
double average_grade_for_all_students(double average[2 * K]) {
	double sum = 0;
	double k = K + 1;

	for (int i = 0; i < K; ++i) {
		sum += average[i];
	}
	double average_num = sum / K;
	return average_num;
}

//removing students with a grade point average that is lower
//than the grade point average in the list
void delete_students(
	struct Students** p,
	struct Students** start,
	double average[2 * K],
	double average_num) {

	int r = 1;
	while (1) {
		struct Students* pp;
		pp = (*p);
		(*p) = (*p)->next;
		r++;
		if ((*p)->next == NULL) {
			break;
		}
		if (average[(r - 1)] < average_num) {
			pp->next = (*p)->next;
			free((*p));
			(*p) = pp;
		}
	}

	(*p) = (*start);
	if (average[0] < average_num) {
		(*start) = (*p)->next;
		free((*p));
	}
	(*p) = (*start);
}

//free memory
void free_memory(struct Students** p) {
	struct Students* px;
	while (1) {

		px = (*p);
		(*p) = (*p)->next;

		if (px->next == NULL) {
			free(px);
			break;
		}
		free(px);
	}
}
