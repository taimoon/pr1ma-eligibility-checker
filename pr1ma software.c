#include "pr1ma library.c"

int pr1ma_comparator(const form *a, const form *b){
    return strcmp(a->name, b->name);
}
int main(void)
{
    file_init(mainFilename);
    int bufferSize = get_record_size(mainFilename)*2+4;
    form record[bufferSize];
    char userInput;
    int userNumInput;
    while(userInput != '0')
    {
        int recordSize = get_record_size(mainFilename);
        system("cls");
        file_load(mainFilename, &record[0]);
        pr1ma_print_all_name(recordSize, &record[0]);
        qsort(record, recordSize, sizeof(record[0]), pr1ma_comparator);
        pr1ma_print_all_name(recordSize, &record[0]);

        printf("\nEnter s to see the form\n");
        printf("Enter a to register\n");
        printf("Enter e to edit\n");
        printf("Enter d to delete\n");
        printf("Enter 0 to quit\n");
        fflush(stdin);
        userInput = getchar();
        switch(userInput)
        {
        case 's':
            printf("\nEnter the number that correspond the name you see:");
            scanf("%d", &userNumInput);
            --userNumInput;
            pr1ma_print_form(record[userNumInput]);
                printf("Enter any key to continue:");
                fflush(stdin);
                getchar();
                system("cls");
                break;
        case 'a':
            pr1ma_register(mainFilename, bufferSize,&record[0]);
            break;
        case 'e':
            printf("\nEnter the number that correspond the name you want edit:");
            scanf("%d", &userNumInput);
            --userNumInput;
            pr1ma_edit(mainFilename, userNumInput,&record[0]);
            break;
        case 'd':
            printf("\nEnter the number that correspond the name you want to delete:");
            scanf("%d", &userNumInput);
            --userNumInput;
            row_delete(mainFilename, userNumInput,&record[0]);
            break;
        case '0':
            break;
        }
    }
    file_save("wb", get_record_size(mainFilename), mainFilename, &record[0]);
    return 0;
}
