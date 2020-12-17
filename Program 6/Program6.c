#include<stdio.h>
#include<omp.h>
#include<string.h>

#define COUNT 10
#define FILE_NAME "words.txt"

char search_words[20][COUNT] = {"The","around","graphics","from","by","be","any","which","various","mount"};
long counts[COUNT];
int line_c = 0;
int is_alpha(char c){
	return ((c >= 65 && c <= 90) || (c >= 97 && c <= 122));
}

int is_equal(char* a,const char* key, int ignore_case){
	char b[20];
	strcpy(b,key);
	int len_a = strlen(a),len_b = strlen(b);
	if(len_a != len_b){
		return 0;
	}
	
	if(ignore_case != 0){
		int i;
		for(i=0;i<len_a;i++){
			if(a[i] > 90){
				a[i] -= 32;
			}
		}
		
		for(i=0;i<len_b;i++){
			if(b[i] > 90){
				b[i] -= 32;
			}
		}
		
	}
	return (strcmp(a,b) == 0);
}

void read_word(char *temp, FILE *fp){
int i=0;
	char ch;
	
	while((ch = fgetc(fp)) != EOF
		&& is_alpha(ch) == 0){
	
	}
	
	while(ch != EOF
		&& is_alpha(ch) != 0){
		temp[i++] = ch;
		ch = fgetc(fp);
	}
	
	temp[i] = '\0';

}

long determine_count(const char *file_name, const char *key, int ignore_case){
	int key_index=0,key_len = strlen(key);
	long word_count=0;
	char ch;
	FILE *fp = fopen(file_name,"r");
	
	char temp[40];
	int i=0;
	
	while(feof(fp) == 0){
		read_word(temp,fp);
		if(is_equal(temp,key,ignore_case) != 0){
			word_count++;
		}
		//printf("%s ",temp);
	}
	
	
	//printf("\nWord %s: %ld",key,word_count);
	return word_count;
}

void main(){
	int i;
	for(i=0;i<COUNT;i++){
		counts[i] = 0;
	}
	double t = omp_get_wtime();
	int num;
	printf("Enter the number of threads\n");
	scanf("%d",&num);
	#pragma omp parallel for num_threads(num)
	for(i=0;i<COUNT;i++){

    		counts[i] = determine_count(FILE_NAME,search_words[i],1);
	}
	
	t = omp_get_wtime() - t;
	
	for(i=0;i<COUNT;i++){
		printf("\n%s: %ld",search_words[i],counts[i]);
	}
	
	
	printf("\nTime Taken: %lf\n",t);
	
}
