#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_STATES 50


struct climate_info {
    char code[3];
    unsigned long num_records;
    double humidity;
    long double sum_temperature;
    long double max_temp;
    time_t max_temp_date;
    long double min_temp;
    time_t min_temp_date;
   long int lightning_strikes;
    int snow_cover;
    long int cloudCover;

};

void analyze_file(FILE* file, struct climate_info* states[], int num_states);
void print_report(struct climate_info* states[], int num_states);
int compareStr(char* str1, char*str2);

//MAIN FUNCTION
int main(int argc, char* argv[]) {

    /* TODO: fix this conditional. You should be able to read multiple files. */
    if (argc < 1) {
        printf("Usage: %s tdv_file1 tdv_file2 ... tdv_fileN \n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Let's create an array to store our state data in. As we know, there are
     * 50 US states. */
    struct climate_info* states[NUM_STATES] = { NULL };

    int i;
    for (i = 1; i < argc; ++i) {
        /* TODO: Open the file for reading */
        FILE* file = fopen(argv[i], "r");
        if (file == NULL) {
            printf("something happened and the %s file could not be processed\n", argv[i]);
            continue;
        }

         analyze_file(file, states, NUM_STATES); 
         fclose(file);
    }
   
    print_report(states, NUM_STATES);
    return 0;
}
/*A function for the purpose of opening a file, reading its content, and determing whether to create a new struct or append an existing struct based 
* on the content of each individual line within the file.
*/
void analyze_file(FILE* file, struct climate_info** states, int num_states) {
    const int line_sz = 100;
    char line[line_sz];
    while (fgets(line, line_sz, file) != NULL) {
	 char * token_line = strtok(line, "\t");
     char* token[9];
     int tracker = 0;
     while (token_line != NULL && tracker < 9) {
         token[tracker++] = token_line;
         token_line = strtok(NULL, "\t");
     }
        for (int i = 0; i < num_states; i++){
		
	    if (compareStr((states[i]->code), token[0]) == 0) {
            states[i]->num_records += 1;
                states[i]->humidity += atof(token[3]);
                states[i]->snow_cover += atol(token[4]);
                states[i]->cloudCover += atoi(token[5]);
                states[i]->lightning_strikes += atoi(token[6]);
                states[i]->sum_temperature+= atof(token[8]);
                if (atof(token[8]) > states[i]->max_temp) 
                {
                    states[i]->max_temp = atof(token[8]);
                    states[i]->max_temp_date = atol(token[1]) / 1000;
                }
                if (atof(token[8]) < states[i]->min_temp) {
                    states[i]->min_temp = atof(token[8]);
                    states[i]->min_temp_date = atol(token[1]) / 1000;
                }
             
                   
                
		break;
            }
            else if (states[i] == NULL) {

		 struct climate_info * newInfo  = malloc(sizeof(struct climate_info));
                newInfo->num_records = 1;
               for(int j = 0; j < 3; j++){
		*(newInfo->code +j) = *(token[0]+j);
	       }
                newInfo->humidity = atof(token[3]);
                newInfo->sum_temperature = atof(token[8]);
                newInfo->max_temp = atof(token[8]);
                newInfo-> min_temp = atof(token[8]);
                newInfo->max_temp_date = atol(token[1]) / 1000;
                newInfo->min_temp_date = atol(token[1]) / 1000;
             
                newInfo->snow_cover = atoi(token[4]);
                newInfo->cloudCover = atol(token[5]);
            
                newInfo->lightning_strikes = atol(token[6]);
     
		states[i] = newInfo;
		break;
            }
        }

    }
}


// COMAPRES two state codes
int compareStr(char* str1, char* str2){
	int tbr = 1;
	for(int i = 0; i < 2; i++){
        if (str1 == NULL || str2 == NULL) {
            return tbr;
        }
	if(*(str1+i) == *(str2+i)){
	    tbr = 0;
	}
	else{
		tbr = 1;
		return tbr;
		}

	}
	return tbr;
	}

//Converts Kelvin to Farenhiet
long double convertTemp(long double temp) {
    long double returnable = (temp * 1.8) - 459.67;
    return returnable;
}
//outputs the content of each struct found in the array of states
void print_report(struct climate_info* states[], int num_states) {
    printf("States found:\n");
    int i;
    for (i = 0; i < num_states; ++i) {
        if (states[i] != NULL) {
            struct climate_info* info = states[i];
            printf("%s ", info->code);
        }
    }
    printf("\n");
    for (int j = 0; j < num_states; j++) {
        if (states[j] != NULL) {
            printf("-- State: %s --\n", states[j]->code);
            printf("Number of Records: %lu \n", states[j]->num_records);
	    printf("Average Humidity: %.1F%%\n",states[j]->humidity/ states[j]->num_records);
	    printf("Average Temperature: %.1LfF\n", convertTemp(states[j]->sum_temperature/ states[j]->num_records));
	    printf("Max Temperature: %.1LfF\n", convertTemp(states[j]->max_temp));
	    printf("Max temperature on: %s", ctime(&states[j]->max_temp_date));
	    printf("Min Temperature: %.1LfF\n", convertTemp(states[j]->min_temp));
	    printf("Min Temperature on: %s", ctime(&states[j]->min_temp_date));
	    printf("Lightning Strikes: %ld\n", states[j]->lightning_strikes);
	    printf("Records with Snow Cover: %d\n", states[j]->snow_cover);
	    printf("Average Cloud Cover: %.1F%%\n",(double) (states[j]->cloudCover/states[j]->num_records));
        printf("\n");
        }
    }
}




