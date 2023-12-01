/**
 * @author Lukáš Gronich <xgronil00>
 * @date 26.10.2023
 **/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// 1. Podukol
typedef struct
{
    int rows;
    int cols;
    unsigned char *cells;
} Map;

Map *initialization_map(char *file_name)
{
    Map *M;
    M = malloc(sizeof(Map));
    if (M == NULL)
        return NULL;

    FILE *file = fopen(file_name, "r");
    if (file == NULL)
    {
        printf("Nelze otevrit soubor %s\n", file_name);
        return NULL;
    }

    int number;
    for (int i = 0; i < 2; i++)
    {
        if (fscanf(file, "%d", &number) == 1)
        {
            if (i == 0)
                M->rows = number;
            if (i == 1)
                M->cols = number;
        }
    }
    fclose(file);

    M->cells = malloc(M->rows * M->cols * sizeof(unsigned char));
    if (M->cells == NULL)
        return NULL;
    return M;
}

// 2. Podukol
bool isborder(Map *map, int r, int c, int border)
{
    int field_value = map->cells[(r - 1) * map->cols + (c - 1)];
    if (field_value == 7)
        return true;
    if (border == 0) // Left
    {
        if (field_value % 2 != 0)
            return true;
    }

    if (border == 1) // Right
        if (field_value == 2 || field_value == 3 || field_value == 6)
            return true;

    if (border == 2 && (field_value > 3 && field_value < 8)) // Upper / Lower
        return true;
    return false;
}

int map_load(Map *M, char *file_name)
{
    FILE *file = fopen(file_name, "r");
    if (file == NULL)
    {
        return 1;
    }

    int number;
    int i = -2;
    while (fscanf(file, "%d", &number) == 1)
    {
        if (i >= 0)
        {
            M->cells[i] = number;
        }
        i++;
    }
    fclose(file);
    if (i == (M->rows * M->cols))
    {
        for (int x = 1; x <= M->rows; x++)
        {
            for (int y = 1; y <= M->cols - 1; y++)
            {
                if ((isborder(M, x, y, 1) == false && isborder(M, x, y + 1, 0) == true) || (isborder(M, x, y, 1) == true && isborder(M, x, y + 1, 0) == false))
                {
                    printf("1\n");
                    return 1;
                }
            }
        }
        for (int x = 1; x <= M->rows - 1; x++)
        {
            for (int y = 1; y <= M->cols; y++)
            {
                if ((x % 2 == 0 && y % 2 != 0) || (x % 2 != 0 && y % 2 == 0))
                {
                    if ((isborder(M, x, y, 2) == false && isborder(M, x + 1, y, 2) == true) || (isborder(M, x, y, 2) == true && isborder(M, x + 1, y, 2) == false))
                    {
                        printf("2\n");
                        return 1;
                    }
                }
            }
        }
        return 0;
    }
    else
        return 1;
    return 0;
}

void map_free(Map *M)
{
    free(M->cells);
    free(M);
}

// 3. Podukol
int start_border(Map *map, int r, int c, int leftright)
{
    if (c == 1 && r % 2 != 0 && isborder(map, r, c, 0) == false)
    {
        if (leftright == 1)
            return 1;
        if (leftright == 2)
            return 2;
    }

    if (c == 1 && r % 2 == 0 && isborder(map, r, c, 0) == false)
    {
        if (leftright == 1)
            return 2;
        if (leftright == 2)
            return 1;
    }

    if (r == 1 && c % 2 != 0 && isborder(map, r, c, 2) == false)
    {
        if (leftright == 1)
            return 0;
        if (leftright == 2)
            return 1;
    }

    if (r == map->rows && isborder(map, r, c, 2) == false)
    {
        if (leftright == 1)
            return 1;
        if (leftright == 2)
            return 0;
    }

    if (c == map->cols && r % 2 != 0 && isborder(map, r, c, 1) == false)
    {
        if (leftright == 1)
            return 2;
        if (leftright == 2)
            return 0;
    }

    if (c == map->cols && r % 2 == 0 && isborder(map, r, c, 1) == false)
    {
        if (leftright == 1)
            return 0;
        if (leftright == 2)
            return 2;
    }
    return 3;
}

void find_path(Map *map, int r, int c, int leftright, int start)
{
    int last_start;

    if (isborder(map, r, c, start) == true)
    {
        if (leftright == 1)
            start = start_border(map, r, c, 2);
        if (leftright == 2)
            start = start_border(map, r, c, 1);
    }

    while (r > 0 && r <= map->rows && c > 0 && c <= map->cols)
    {
        if (leftright == 1)
        {
            if (isborder(map, r, c, start) == false)
            {
                if (start == 1)
                {
                    if ((last_start == 1) && ((r % 2 == 0 && c % 2 != 0) || (r % 2 != 0 && c % 2 == 0)) && isborder(map, r, c, 2) == false)
                        start = 2;

                    else if (last_start != 0 || (last_start == 0 && isborder(map, r, c, 2) == true && isborder(map, r, c, 0) == true))
                    {
                        printf("%d,%d\n", r, c);
                        c++;
                        last_start = start;
                    }
                    else if ((last_start == 0) && ((r % 2 == 0 && c % 2 == 0) || (r % 2 != 0 && c % 2 != 0)))
                        start = 2;
                    else if (last_start == 0 && ((r % 2 != 0 && c % 2 == 0) || (r % 2 == 0 && c % 2 != 0)) && isborder(map, r, c, 0) == false)
                        start = 0;
                    else if (last_start == 2 && isborder(map, r, c, 2) == true)
                        start = 0;
                    else
                        start = 2;
                }

                else if (start == 2)
                {
                    if ((r % 2 != 0 && c % 2 == 0) || (r % 2 == 0 && c % 2 != 0))
                    {
                        if (last_start != start || (last_start == start && isborder(map, r, c, 0) == true && isborder(map, r, c, 1) == true))
                        {
                            printf("%d,%d\n", r, c);
                            r++;
                            last_start = start;
                            start = 0;
                        }
                        else if (last_start == start && isborder(map, r, c, 1) == true)
                            start = 0;
                    }
                    else if ((r % 2 == 0 && c % 2 == 0) || (r % 2 != 0 && c % 2 != 0))
                    {
                        if (last_start != start || (last_start == start && isborder(map, r, c, 0) == true && isborder(map, r, c, 1) == true))
                        {
                            printf("%d,%d\n", r, c);
                            r--;
                            last_start = start;
                            start = 1;
                        }
                        else if (last_start == start && isborder(map, r, c, 1) == true)
                            start = 0;
                    }
                }

                else if (start == 0)
                {
                    printf("%d,%d\n", r, c);
                    c--;
                    last_start = start;
                    start = 1;
                }
            }
            else
            {
                if (start == 1)
                {
                    start = 2;
                }
                else if (start == 2)
                {
                    start = 0;
                }
                else if (start == 0)
                {
                    start = 1;
                }
            }
        }

        if (leftright == 2)
        {
            if (isborder(map, r, c, start) == false)
            {
                if (start == 1)
                {
                    printf("%d,%d\n", r, c);
                    c++;
                    last_start = start;
                    start = 0;
                }

                else if (start == 2)
                {
                    if ((r % 2 != 0 && c % 2 == 0) || (r % 2 == 0 && c % 2 != 0))
                    {
                        if (last_start != start || (last_start == start && isborder(map, r, c, 0) == true && isborder(map, r, c, 1) == true))
                        {
                            printf("%d,%d\n", r, c);
                            r++;
                            last_start = start;
                            start = 1;
                        }
                        else if (last_start == start && isborder(map, r, c, 0) == true)
                            start = 1;
                    }
                    else if ((r % 2 == 0 && c % 2 == 0) || (r % 2 != 0 && c % 2 != 0))
                    {
                        if ((last_start != start) || (last_start == start && isborder(map, r, c, 0) == true && isborder(map, r, c, 1) == true))
                        {
                            printf("%d,%d\n", r, c);
                            r--;
                            last_start = start;
                            start = 0;
                        }
                        else if (last_start == start && isborder(map, r, c, 0) == true)
                            start = 1;
                    }
                }

                else if (start == 0)
                {
                    if ((last_start == 0) && ((r % 2 == 0 && c % 2 != 0) || (r % 2 != 0 && c % 2 == 0)) && isborder(map, r, c, 2) == false)
                        start = 2;

                    else if (last_start != 1 || (last_start == 1 && isborder(map, r, c, 2) == true && isborder(map, r, c, 1) == true))
                    {
                        printf("%d,%d\n", r, c);
                        c--;
                        last_start = start;
                    }
                    else if ((last_start == 1) && ((r % 2 == 0 && c % 2 != 0) || (r % 2 != 0 && c % 2 == 0)) && isborder(map, r, c, 1) == false)
                    {
                        start = 1;
                    }

                    else if (last_start == 2 && isborder(map, r, c, 0) == true)
                    {
                        start = 1;
                    }

                    else
                    {
                        start = 2;
                    }
                }
            }
            else
            {
                if (start == 1)
                {
                    if (last_start == 1)
                        start = 1;
                    if (last_start == 0)
                        start = 0;
                    if (last_start == 2)
                        start = 0;
                    start = 0;
                }
                else if (start == 2)
                {
                    start = 1;
                }
                else if (start == 0)
                {
                    start = 2;
                }
            }
        }
    }
}

int main(int argc, char *argv[]) // Hlavní funkce
{

    if (argc == 2)
    {
        char help[] = "--help";
        if (strcmp(help, argv[1]) == 0)
        {
            printf("Program se spousti v nasledujici podobe: \n");
            printf("./maze --test soubor.txt\n");
            printf("./maze --rpath R C soubor.txt\n");
            printf("./maze --lpath R C soubor.txt\n");
            printf("--test pouze zkontroluje, ze soubor obsahuje radnou definici mapy bludiste.\n");
            printf("--rpath hleda pruchod bludistem na vstupu na radku R a sloupci C za pomoci prave ruky.\n");
            printf("--lpath hleda pruchod bludistem na vstupu na radku R a sloupci C za pomoci leve ruky.\n");
        }
        else
            printf("Invalid input\n");
    }
    else if (argc == 3)
    {
        char test[] = "--test";
        if (strcmp(test, argv[1]) == 0)
        {
            Map *M = initialization_map(argv[2]);
            if (M == NULL)
            {
                printf("Invalid\n");
                return 1;
            }
            if (map_load(M, argv[2]) != 0)
            {
                map_free(M);
                printf("Invalid\n");
                return 1;
            }
            printf("Valid\n");
            map_free(M);
        }
        else
            printf("Invalid input\n");
    }
    else if (argc == 5)
    {
        int leftright;
        char rpath[] = "--rpath";
        char lpath[] = "--lpath";
        int r = atoi(argv[2]);
        int c = atoi(argv[3]);
        if (strcmp(rpath, argv[1]) == 0)
            leftright = 1;
        else if (strcmp(lpath, argv[1]) == 0)
            leftright = 2;
        else
            printf("Invalid input\n");
        if (r > 0 && c > 0)
        {
            Map *M = initialization_map(argv[4]);
            if (M == NULL)
            {
                return 1;
            }
            if (map_load(M, argv[4]) != 0)
            {
                map_free(M);
                return 1;
            }
            int start = (start_border(M, r, c, leftright));
            if (start == 3)
                printf("Na zadanych souradnicich neni vstup do bludiste\n");
            else
                find_path(M, r, c, leftright, start);
            map_free(M);
        }
        else
            printf("Spatne souradnice\n");
    }
    else
        printf("Invalid input\nPro napovedu spustte './maze --help'\n");
    return 0;
}