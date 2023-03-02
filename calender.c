#include<stdio.h>
#include<conio.h>
#include<windows.h>


int get_first_day_of_the_year(int year);
struct Date{
    int dd;
    int mm;
    int yy;
};
struct Date date;

struct Remainder{
    int dd;
    int mm;
    char note[50];
};
struct Remainder R;


COORD xy = {0, 0};

void gotoxy (int x, int y)
{
        xy.X = x; xy.Y = y; // X and Y coordinates
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
}

//This will set the forground color for printing in a console window.
void SetColor(int ForgC)
{
     WORD wColor;
     //We will need this handle to get the current background attribute
     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_SCREEN_BUFFER_INFO csbi;

     //We use csbi for the wAttributes word.
     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
        //Mask out all but the background attribute, and add in the forgournd color
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
}

void ClearColor(){
    SetColor(15);
}

void ClearConsoleToColors(int ForgC, int BackC)
{
     WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
     //Get the handle to the current output buffer...
     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     //This is used to reset the carat/cursor to the top left.
     COORD coord = {0, 0};
     //A return value... indicating how many chars were written
     //   not used but we need to capture this since it will be
     //   written anyway (passing NULL causes an access violation).
     DWORD count;

     //This is a structure containing all of the console info
     // it is used here to find the size of the console.
     CONSOLE_SCREEN_BUFFER_INFO csbi;
     //Here we will set the current color
     SetConsoleTextAttribute(hStdOut, wColor);
     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
          //This fills the buffer with a given character (in this case 32=space).
          FillConsoleOutputCharacter(hStdOut, (TCHAR) 32, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);

          FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &count );
          //This will set our cursor position for the next print statement.
          SetConsoleCursorPosition(hStdOut, coord);
     }
     return;
}

void SetColorAndBackground(int ForgC, int BackC)
{
     WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);;
     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
     return;
}

int check_leapYear(int year){ //checks whether the year passed is leap year or not
    if((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
       return 1;
    return 0;

}

int checkNote(int dd, int mm){
    FILE *fp;
    fp = fopen("note.txt","rb");
    if(fp == NULL){
        printf("Error in Opening the file");
    }
    while(fread(&R,sizeof(R),1,fp) == 1){
        if(R.dd == dd && R.mm == mm){
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void AddNote(){
    FILE *fp;
    fp = fopen("note.txt","ab+");
    system("cls");
    gotoxy(5,7);
    printf("Enter the date(DD/MM): ");
    scanf("%d%d",&R.dd, &R.mm);
    gotoxy(5,8);
    printf("Enter the Note(50 character max): ");
    fflush(stdin);
    scanf("%[^\n]",R.note);
    if(fwrite(&R,sizeof(R),1,fp)){
        gotoxy(5,12);
        puts("Note is saved sucessfully");
        fclose(fp);
    }else{
        gotoxy(5,12);
        SetColor(12);
        puts("\aFail to save!!\a");
        ClearColor();
    }
    gotoxy(5,15);
    printf("Press any key............\n \n");
    printf("Your note>> \n");
    fp= fopen("note.txt", "r");
    char MyStr[100];
    fgets(MyStr,100,fp);
    printf("%s", MyStr);
    fclose(fp);
    getch();
    fclose(fp);
}

void showNote(int mm){
    FILE *fp;
    int i = 0, isFound = 0;
    system("cls");
    fp = fopen("note.txt","rb");
    if(fp == NULL){
        printf("Error in opening the file");
    }
    while(fread(&R,sizeof(R),1,fp) == 1){
        if(R.mm == mm){
            gotoxy(10,5+i);
            printf("Note %d Day = %d: %s", i+1, R.dd,  R.note);
            isFound = 1;
            i++;
        }
    }
    if(isFound == 0){
        gotoxy(10,5);
        printf("This Month contains no note");
    }
    gotoxy(10,7+i);
    printf("Press any key to back.......");
    getch();

}


int main()
{
    char *months[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    int i, j, total_days, week_day, year, space_counter = 0;
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int choice;
    while(1) {
        system("cls");
        printf("1. Check full year calendar>> \n");
        printf("2. Add special note>> \n");
        printf("3. Check saved notes \n");
        printf("4. Exit Program \n");
        printf("Enter your choice>>  ");
        scanf("%d", &choice);
        system("cls");
        switch (choice) {
            case 1:
                printf("Enter Year>> ");
                scanf("%d", &year);
                printf("\n\n********** Welcome to %d **********\n\n", year);

                // Check if it is a leap year
                if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
                    days_in_month[1] = 29;
                }

                // get the first day of the year
                week_day = get_first_day_of_the_year(year);

                for (i = 0; i < 12; i++) {
                    printf("\n\n\n\n----------------------- %s -----------------------\n", months[i]);
                    printf("\n   Sun   Mon   Tue   Wed   Thu   Fri   Sat\n\n");

                    for (space_counter = 1; space_counter <= week_day; space_counter++) {
                        printf("      ");
                    }

                    total_days = days_in_month[i];
                    for (j = 1; j <= total_days; j++) {
                        printf("%6d", j);
                        week_day++;
                        if (week_day > 6) {
                            week_day = 0;
                            printf("\n");
                        }
                    }
                }
                printf("\nPress any key to continue......");
                getch();
                break;
            case 2:
                AddNote();
                break;

            case 3:

                showNote(03);
                break;
            case 4:
                exit(0);
                break;

        }


    }

    return 0;
}

// first day of the year
int get_first_day_of_the_year(int year)
{
    int day = (year*365 +((year-1)/4) - ((year-1)/100)+((year-1)/400)) % 7;

    return day;
}

