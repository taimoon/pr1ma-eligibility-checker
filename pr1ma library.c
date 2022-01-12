#include <stdio.h>
#include <string.h>
#include <stdlib.h>//system("cls");
/**
Important note:
CSV implementation is a legacy and have been moved to another library. Currently, fread() and fwrite() are used.
there are two main groups of function:
1) file operation (work underneath)
2) pr1ma menu side (interaction between client and program)
**/

#define CSV_CELL_MAXLEN 256
#define NAME_MAXLEN 128
#define TELNO_MAXLEN 15
#define ADDRESS_MAXLEN 256
#define POSTCODE_MAXLEN 8
#define IC_MAXLEN 15
#define CITY_MAXLEN 15
#define STATE_MAXLEN 15
#define OCCUPATION_MAXLEN 32
#define CHOICE_MAXLEN 32
#define PROPERTY_MAXLEN 32

char mainFilename[] = "pr1ma file.bin";
///the order is useful
/*These constants are defined in char
because C doesn't provide boolean data type.
Secondly, these constants are not meant to be calculated.
Thirdly, it is easier to extract char from file to program rather using fscanf() and sscanf().
*/
const char FEMALE = 'F'; const char MALE = 'M';
const char SINGLE = '0'; const char MARRIED = '1';
const char NOT_BANKRUPT = '0'; const char BANKRUPT = '1';

typedef struct pr1ma_Form
{
    char name[NAME_MAXLEN];
    int age;
    char ic[IC_MAXLEN];
    char gender;
    char marital_status;
    int children_N;
    char address[ADDRESS_MAXLEN];
    char postcode[POSTCODE_MAXLEN];
    char city[CITY_MAXLEN];
    char state[STATE_MAXLEN];
    char handphone[TELNO_MAXLEN];
    char tel[TELNO_MAXLEN];
    char occupation[OCCUPATION_MAXLEN];
    int income;
    char bankruptcy;
    char pr1ma_state[STATE_MAXLEN];
    char pr1ma_city[CITY_MAXLEN];
    char property_type[PROPERTY_MAXLEN];
    char choice[CHOICE_MAXLEN];
}form;

///function list

int int_input();
/**This function is used to simulate the python style input(read and return);
it is used to simplfy the arithemtic calcution occured at income check in pr1ma_eligibility_check function.
Instead of defining 2 temp int var, it is better to total up 2 incomes such that
total_income  = int_input();
total_income += int_input();

rather than
int total_income, temp;
scanf("%d %d", &total_income, &temp);
total_income += temp;
**/
//menu and printing
void pr1ma_print_all_name(int size, form record[]);
void pr1ma_print_form(form record);
//pr1ma register
int pr1ma_eligibility_check(form record);
int pr1ma_register_eligibility_check(int *age, int *income, char *marital_status, char *bankruptcy_status);//return 1 if eligible, or esle return 0
void pr1ma_register(char *filename, int bufferSize, form record[]);
//file
int get_record_size(char *filename);
void file_init(char *filename);
void file_load(char *filename, form record[]);
void file_cell_extract(FILE *fp, char cell[CSV_CELL_MAXLEN], int MAX_LEN); //this is not independent function, but being used with file_load
/**there are 2 modes, "wb" is used when save the new file after delete one of the people,
"ab" is used when adding new one to the file**/
void file_save(char *mode, int size, char *filename, form record[]);
void row_delete(char *filename, int position,form record[]);
///end of the function list
int int_input()
{
    int temp;
    fflush(stdin);
    scanf("%d", &temp);
    return temp;
}
void pr1ma_print_all_name(int size, form record[])
{
    for(int i = 0; i < size; ++i)
    printf("%d)\t%s\n", i+1, record[i].name);
}
void pr1ma_print_form(form record)
{
    printf("Name\t\t\t: %s\n", record.name);
    printf("Age\t\t\t: %d\n", record.age);
    printf("IC\t\t\t: %s\n", record.ic);
    printf("Gender\t\t\t: ");
    if(record.gender == 'F')
        printf("female\n");
    else if(record.gender == 'M')
        printf("male\n");
    else
        printf("unspecified\n");
    const char script_Marriage[][8] = {"Single", "Married"};
    printf("Marital status\t\t: %s\n", script_Marriage[(int)record.marital_status-'0']);
    printf("Number of children\t: %d\n", record.children_N);
    printf("Address\t\t\t: %s\n", record.address);
    printf("Postcode\t\t: %s\n", record.postcode);
    printf("City\t\t\t: %s\n", record.city);
    printf("State\t\t\t: %s\n", record.state);
    printf("Handphone\t\t: %s\n", record.handphone);
    printf("Home Telephone\t\t: %s\n", record.tel);
    printf("Occupation\t\t: %s\n", record.occupation);
    printf("Income(RM)\t\t: %d\n", record.income);
    const char script_YesNo[][4] = {"No", "Yes"};
    printf("Bankruptcy\t\t: %s\n", script_YesNo[(int)record.bankruptcy-'0']);

}
int pr1ma_eligibility_check(form record)//return 1 if eligible, or esle return 0
{
    if(record.age < 21
       || record.income < 2500 || record.income > 15000
       || record.bankruptcy == BANKRUPT
       )return 0;
    else
        return 1;
}
int pr1ma_register_eligibility_check(int *age, int *income, char *marital_status, char *bankruptcy_status)//return 1 if eligible, or esle return 0
{
    /*
        The passed parameters are not immediately assign new values but only assigned after it is eligible.
        This is because it is not necessary for all registration is eligible.
        If it is not eligible, then these variables need not new value.
    */
    int tempAge, tempIncome;
    char tempMarital_status;
    printf("\nEligibility Criteria:\n"
           "Malayisan Citizen\n"
           "Single or married, age 21 above (married couples only apply one)\n"
           "Individual or combined household income (husband & wife) between RM2,500 to RM15,000 monthly (gross income).\n"
           "Purchase as your first or second home\n"
           );
    printf("\n");
    fflush(stdin);
    printf("Are you a Malaysian citizen? Enter 1 if yes, 0 of no: ");
    if(getchar() != '1')
    {
        printf("The non-Malaysian is not eligible for PR1MA program.\n");
        return 0;
    }
    fflush(stdin);
    printf("Are you buying the house as first or second home? Enter 1 if yes, 0 of no: ");
    if(getchar() != '1')
    {
        printf("PR1MA only eligible for those who are buying house as first or second house.\n");
        return 0;
    }
    fflush(stdin);
    printf("Are you currently bankrupted?Enter %c if yes, %c of no: ", BANKRUPT, NOT_BANKRUPT);
    if(getchar() != NOT_BANKRUPT)
    {
        printf("Bankrupted applicant also not eligible for PR1MA.\n");
        return 0;
    }
    printf("Marital Status(Enter %c if single, Enter %c if married): ", SINGLE, MARRIED);
    fflush(stdin);
    tempMarital_status = getchar();
    if(tempMarital_status == MARRIED) ///Arithmetic element
    {
        printf("Enter your gross your income here: RM ");
        tempIncome = int_input();
        printf("Enter your gross your partner income here: RM ");
        tempIncome += int_input();
    }
    else
    {
        printf("Enter your gross your income here: RM ");
        tempIncome = int_input();
    }
    if(tempIncome < 2500 || tempIncome > 15000)
    {
        printf("Combined income ranges between RM 2 500 and RM 15 000 for eligible applicant.\n");
        return 0;
    }
    printf("Enter your age here: ");
    scanf("%d", &tempAge);
    if(tempAge < 21)
    {
        printf("The age cannot be smaller than 21 eligible applicant.\n");
        return 0;
    }
    *marital_status = (char)tempMarital_status;
    *age = tempAge;
    *income = tempIncome;
    *bankruptcy_status = (char) NOT_BANKRUPT;
    return 1;
}
void pr1ma_register(char *filename, int bufferSize,form record[])
{
    int size = get_record_size(filename);
    form tempForm;
    if(size == bufferSize) //this can be avoided if malloc() is implemented
    {
        printf("Buffer exceeded, please reopen the program again");
        exit(-1);
    }
    if(!(pr1ma_register_eligibility_check(&tempForm.age, &tempForm.income, &tempForm.marital_status, &tempForm.bankruptcy)))
    {
        printf("Not eligible.\nEnter any key to continue");
        fflush(stdin);getchar();
        return;
    }
    printf("You're eligible for applying PR1MA houses. Pleae enter the revelant information.\n");
    printf("Enter your name: ");
    fflush(stdin);
    gets(tempForm.name);
    printf("Enter your ic: ");
    gets(tempForm.ic);
    printf("Enter your gender(Enter f if female, m if male ): ");
    tempForm.gender = getchar()-'a'+'A';fflush(stdin);
    printf("How many children you have?: ");
    scanf("%d", &tempForm.children_N);fflush(stdin);
    printf("Enter your address: ");
    gets(tempForm.address);
    printf("Enter your city: ");
    gets(tempForm.city);
    printf("Enter your state: ");
    gets(tempForm.state);
    printf("Enter your postcode: ");
    gets(tempForm.postcode);
    printf("Enter your handphone number: ");
    gets(tempForm.handphone);
    printf("Enter your home phone number: ");
    gets(tempForm.tel);
    printf("Enter your occupation: ");
    gets(tempForm.occupation);
    record[size] = tempForm;
    file_save("ab",get_record_size(filename), filename, &record[0]);
}
void pr1ma_edit(char *filename, int position, form record[])
{
    const char script_pr1ma_edit_option[][32] = {"name","age","ic","gender","marital_status","number of children",
"address","postcode","city","state","handphone","Home phone","occupation","income","bankruptcy status"};
    int optionN = sizeof(script_pr1ma_edit_option)/sizeof(script_pr1ma_edit_option[0]);
    int userInput = optionN+1;
    while(userInput != -1)
    {
        system("cls");
        pr1ma_print_form(record[position]);
        printf("\n");
        if(!(pr1ma_eligibility_check(record[position])) && userInput < optionN)
        {
            printf("Due to the %s, the current applicant is not eligible after edited.\n", script_pr1ma_edit_option[userInput]);
            printf("Enter 1 to edit again if you think you input wrong data.\n");
            printf("Enter any key if the data is correct, you are understood that you're not eligible for PR1MA \nand your application will be removed.\n");
            fflush(stdin);
            if(getchar() != '1') userInput = -1;
        }
        else
        {
            for(int i = 0; i < optionN; ++i)
            printf("%d) %s\n", i+1, script_pr1ma_edit_option[i]);
            printf("0) Quit\n");
            fflush(stdin);
            printf("Enter left side value that correspond the information you want to edit: ");
            scanf("%d", &userInput);
            --userInput;
        }

        fflush(stdin);
        switch(userInput){
        case 0:
            printf("Enter your name: ");
            gets(record[position].name);
            break;
        case 1:
            printf("Enter your age here: ");
            scanf("%d", &record[position].age);
            break;
        case 2:
            printf("Enter your ic: ");
            gets(record[position].ic);
            break;
        case 3:
            printf("Enter your gender(Enter f if female, m if male ): ");
            record[position].gender = getchar()-'a'+'A';
            break;
        case 4:
            printf("Marital Status(Enter 0 if single, Enter 1 if married): ");
            record[position].marital_status = getchar();
            break;
        case 5:
            printf("How many children you have?: ");
            scanf("%d", &record[position].children_N);
            break;
        case 6: //address
            printf("Enter your address: ");
            gets(record[position].address);
            break;
        case 7://postcode
            printf("Enter your postcode: ");
            gets(record[position].postcode);
            break;
        case 8://city
            printf("Enter your city: ");
            gets(record[position].city);
            break;
        case 9://state
            printf("Enter your state: ");
            gets(record[position].state);
            break;
        case 10://handphone
            printf("Enter your handphone number: ");
            gets(record[position].handphone);
            break;
        case 11: //home phone
            printf("Enter your home phone number: ");
            gets(record[position].tel);
            break;
        case 12: //occupation
            printf("Enter your occupation: ");
            gets(record[position].occupation);
            break;
        case 13: //income
            printf("Enter your gross income here(combined with your partner if married): RM ");
            scanf("%d", &record[position].income);
            break;
        case 14: //bankruptcy
            printf("Are you currently bankrupted?Enter 1 if yes, 0 of no: ");
            record[position].bankruptcy = getchar();
            break;
        case -1:
            break;
        default:
            printf("Invalid input. Enter any key to continue");
            fflush(stdin);
            getchar();
            break;
        }
    }
    system("cls");
    if(!(pr1ma_eligibility_check(record[position])))
        row_delete(filename, position, &record[0]);
    else
        file_save("wb",get_record_size(filename), filename, &record[0]);
}
int get_record_size(char *filename)
{
    FILE *fp;
    fp = fopen(filename, "rb");
    if(fp == NULL)
    {
        fclose(fp);
        return 0;
    }
    fseek(fp, 0, SEEK_END);
    long lSize = ftell(fp);
    fclose(fp);
    return lSize/sizeof(struct pr1ma_Form);
}
void file_init(char *filename)
{
    FILE *fp;
    fp = fopen(filename, "rb");
    if(fp == 0)
    {
        printf("Cannot open the file, then a new file is created : %s", filename);
        fp = fopen(filename, "wb");
        exit(0);
    }
    fclose(fp);
}
void file_load(char *filename, form record[])
{
    FILE *fp;
    fp = fopen(filename,"rb");
    fread(&record[0], sizeof(struct pr1ma_Form), get_record_size(filename), fp);
    fclose(fp);
};
void file_save(char *mode, int size, char *filename, form record[])
{
    FILE *fp;
    fp = fopen(filename, mode);
    if(!(strcmp(mode, "ab")))
        fwrite(&record[size], sizeof(struct pr1ma_Form), 1, fp);
    else if(!(strcmp(mode, "wb")))
        fwrite(&record[0], sizeof(struct pr1ma_Form), size, fp);
    fclose(fp);
}
void row_delete(char* filename, int position, form record[])
{
    int size = get_record_size(filename)-1;
    if(position > size)
    {
        printf("The index exceeds the size of array.");
        exit(-1);
    }
    record[position] = record[size];
    file_save("wb", size, filename, &record[0]);
}
