#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <locale.h>



struct writer//объявление структуры
{
    char surname[20], country[20];//символьные поля исс - фамилия и страна
    unsigned int birth_year, amount_public;//числовые поля исс - год рождения и количество опубл книг
};

struct writer input_keyboard(void);
void young_writers();
void menu();
void file_creation();
void presentation();
void search();
void create();
void output();
void edit_note();
void delete_note();
void pause ();
void ywriters_present();

int main()//главная функция (вызов функции меню и функции открытия файла)
{
    char file_name[30];

    setlocale(LC_ALL, "Rus");

    printf("\n\t\t\tHello, User!\n ");
    pause();
    file_creation(file_name);
    menu(file_name);
    return 0;
}

void file_creation(char *file_name)//функция выбора файла для сохранения базы данных
{
    char q;
    FILE *fin;
    do
    {
        system("clear");
        printf("\n\tHow do you want to create date base?\n 1. Open an existing file.\n 2. Create a new file.\n\n Press suitable answer - ");
        q = getchar();
    } while (q != '1' && q != '2');
    if (q == '1')
    {
        printf("\n\n Input file name: ");
        scanf("%30s", file_name);
    }
    else
    {
        printf("\n\n Input a wished file name: ");
        scanf("%30s", file_name);
        fin = fopen(file_name, "wb");
        fclose(fin);
    }
}

void menu(char *file_name)//функция меню(интерфейс взаимодействия пользователя с программой)
{
    char answer;

    while (1)
    {
        do
        {
            system("clear");
            printf("\n\t\tMENU\n 1. Creating or adding objects.\n 2. Showing.\n 3. Looking for the author.\n 4. Authors younger than 30 y.o.\n 5. Edit a note.\n 6. Delete a note.\n 7. Exit.\n\n Input the menu marker - ");
            answer = getchar();
        } while (answer != '1' && answer != '2' && answer != '3' && answer != '4' && answer != '5' && answer != '6' && answer != '7');
        switch (answer)
        {
        case '1':
            create(file_name, input_keyboard);
            break;
        case '2':
            presentation(file_name);
            break;
        case '3':
            search(file_name);
            break;
        case '4':
            young_writers(file_name, ywriters_present);
            break;
        case '5':
            edit_note(file_name);
            break;
        case '6':
            delete_note(file_name);
            break;
        case '7':
        {
            system("clear");
            printf("\n\t\tEnd of the programm\n ");
            pause();
            exit(0);
        }
        }
    }
}

struct writer input_keyboard(void)//функция ввода одной записи с клавиатуры
{
    struct writer new;

    system("clear");
    printf("\n\t\tInput information about writer\n Surname - ");
    scanf("%20s", &new.surname);
    printf(" The year of birth - ");
    scanf("%u", &new.birth_year);
    printf(" Country - ");
    scanf("%20s", &new.country);
    printf(" How many published books - ");
    scanf("%u", &new.amount_public);
    return new;
}

void create(char *file_name, struct writer (*input) (void))//функция добавления новой записи в файл
{
    FILE *fout;
    struct writer new;
    char answer;

    fout = fopen(file_name, "a+b");
    while (1)
    {
        system("clear");
        new=input();
        fwrite(&new, sizeof(struct writer), 1, fout);
        fflush(fout);
        do
        {
            printf("\n Do you want to continue (y/n)? - ");
            answer = getchar();
        } while (answer != 'n' && answer != 'N' && answer != 'y' && answer != 'Y');
        printf("\n ");
        pause();
        if (answer == 'n' || answer == 'N')
            break;
    }
    fclose(fout);
}

void young_writers(char *file_name, void (*ywriters_present)())//функция вывода всех писателей, младше 30 с вводимой страны
{
    char country[20];
    unsigned int cur_year;
    FILE *fin;

    system("clear");
    printf("\n Input a country: ");
    scanf("%s", country);
    printf(" Input current year: ");
    scanf("%u", &cur_year);
    fin = fopen(file_name, "rb");
    if (!fin)
    {
        printf(" This file is empty!\n ");
        pause();
        return;
    }
    printf("\n\n\t\t\t\t\tYoung writers\n");
    ywriters_present(fin, cur_year, country);
    fclose(fin);
}

void presentation(char *file_name)//функция вывода всех писателей на экран
{
    struct writer new;
    FILE *fin;

    fin = fopen(file_name, "rb");
    system("clear");
    if (!fin)
    {
        printf(" file opening error!\n ");
        pause();
        return;
    }
    if(fread(&new, sizeof(struct writer), 1, fin))
    {
        rewind(fin);
        printf("\n\n\t\t\t\t\tWriters\n\n");
        printf("\t __________________________________________________________________________\n");
        printf("\t|                    |        |                   |                        |\n");
        printf("\t|       Surname      |  Year  |      Country      |  How many publications |\n");
        printf("\t|____________________|________|___________________|________________________|\n");
        while (fread(&new, sizeof(struct writer), 1, fin))
        {
            printf("\t|                    |        |                   |                        |\n");
            output(new);
            printf("\n");
            printf("\t|____________________|________|___________________|________________________|\n");
        }
    }
    else printf("\n\tThis file is empty!\n");
    printf("\n");
    pause();
    fclose(fin);
    
}

void search(char *file_name)//функция поиска писателя по ключевым полям
{
    struct writer new;
    FILE *fin;
    char surname[20], country[20];
    unsigned int var = 0;

    system("clear");
    fin = fopen(file_name, "rb");
    if (!fin)
    {
        printf(" This file is empty!\n ");
        pause();
        return;
    }
    printf("\n\tLooking for the writer\n Input surname: ");
    scanf("%s", surname);
    printf(" Input country: ");
    scanf("%s", country);
    while (fread(&new, sizeof(struct writer), 1, fin))
    {
        if (!strcmp(surname, new.surname) && !strcmp(country, new.country))
        {
            var = 1;
            break;
        }
    }
    if (var)
    {
        printf("\n\n\t\t\t\t\tSought writer\n");
        printf("\t __________________________________________________________________________\n");
        printf("\t|                    |        |                   |                        |\n");
        printf("\t|       Surname      |  Year  |      Country      |  How many publications |\n");
        printf("\t|____________________|________|___________________|________________________|\n");
        printf("\t|                    |        |                   |                        |\n");
        output(new);
        printf("\n");
        printf("\t|____________________|________|___________________|________________________|\n\n");
    }
    else
    {
        printf("\tSought writer doesn't exist!\n\n ");
    }
    pause();
    fclose(fin);
}

void output(struct writer new)//функция вывода одной записи
{
    printf("\t|%20s|%8u|%19s|%24u|", new.surname, new.birth_year, new.country, new.amount_public);
}

void delete_note(char *file_name)//функция удаления одной записи
{
    unsigned int  n = 0;
    struct writer new;
    FILE *fin, *fout;
    char surname[20], country[20];

    system("clear");
    fin = fopen(file_name, "rb");
    if (!fin)
    {
        printf(" This file is empty!\n ");
        pause();
        return;
    }
    fout = fopen("file1", "wb");
    printf("\n\t\tNote deleting\n Input surname: ");
    scanf("%s", surname);
    printf(" Input country: ");
    scanf("%s", country);
    while (fread(&new, sizeof(struct writer), 1, fin))
    {
        if (!strcmp(surname, new.surname) && !strcmp(country, new.country))
        {
            n++;
            continue;
        }
        fwrite(&new, sizeof(struct writer), 1, fout);
    }
    fclose(fin);
    fclose(fout);
    remove(file_name);
    rename("file1", file_name);
    if (n)
    {
        printf("\n  Deleting is completed.\n\n ");
    }
    else
        printf("\n  This writer doesn't exist!\n\n ");
    pause();
}

void edit_note(char *file_name)//функция редактирования записи
{
    struct writer new;
    FILE *fin;
    char surname[20], country[20];
    unsigned int answer, flag = 0;

    system("clear");
    fin = fopen(file_name, "r+b");
    if (!fin)
    {
        printf(" This file is empty!\n ");
        pause();
        return;
    }

    printf("\n\t\tNote editing\n Input an initial surname: ");
    scanf("%20s", surname);
    printf(" Input an initial country: ");
    scanf("%20s", country);
    while (fread(&new, sizeof(struct writer), 1, fin))
    {
        if (!strcmp(surname, new.surname) && !strcmp(country, new.country))
        {
            flag++;
            break;
        }
    }
    if (!flag)
    {
        printf("\n\tThis writer doesn't exist!\n ");
        pause();
        return;
    }
    do
    {
        system("clear");
        printf("\n\tWhat do you want to change?\n 1. Surname.\n 2. Birth year.\n 3. Country.\n 4. Amount of publications.\n 5. Cancel the editting.\n\n Give your choice - ");
        scanf("%d", &answer);
    } while (answer != 1 && answer != 2 && answer != 3 && answer != 4 && answer != 5);
    system("clear");
    switch (answer)
    {
    case 1:
    {
        printf(" Input a new surname: ");
        scanf("%20s", &new.surname);
        break;
    }
    case 2:
    {
        printf(" Input a new birth year: ");
        scanf("%u", &new.birth_year);
        break;
    }
    case 3:
    {
        printf(" Input a new country: ");
        scanf("%20s", &new.country);
        break;
    }
    case 4:
    {
        printf(" Input a new amount of publications: ");
        scanf("%u", &new.amount_public);
        break;
    }
    case 5:
    {
        return;
    }
    }
    fseek(fin, ftell(fin) - sizeof(new), SEEK_SET);
    fwrite(&new, sizeof(new), 1, fin);
    fclose(fin);
    printf("\t\tEditting is completed!");
}
void pause(void){
    char key[2];
    printf("Tap any key and then 'enter'... ");
    scanf("%s", key);

}

void ywriters_present(FILE *fin, unsigned int cur_year, char* country){
    struct writer new;
    int var=0;

    while (fread(&new, sizeof(struct writer), 1, fin))
    {
        if ((cur_year - new.birth_year) < 30 && !strcmp(country, new.country))
        {
            var++;
            break;
        }
        else
            continue;
    }
    rewind(fin);
    if (var)
    {
        printf("\t __________________________________________________________________________\n");
        printf("\t|                    |        |                   |                        |\n");
        printf("\t|       Surname      |  Year  |      Country      |  How many publications |\n");
        printf("\t|____________________|________|___________________|________________________|\n");
        while (fread(&new, sizeof(struct writer), 1, fin))
        {
            if ((cur_year - new.birth_year) < 30 && !strcmp(country, new.country))
            {
                printf("\t|                    |        |                   |                        |\n");
                output(new);
                printf("\n");
                printf("\t|____________________|________|___________________|________________________|\n");
            }
        }
    }
    else
        printf("\n\tThese writers don't exist!\n");
    printf("\n ");
    pause();
}