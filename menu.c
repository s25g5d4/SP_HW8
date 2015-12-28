/*
 * menu :
 * A generalised menu function
 * The argument is base of an array of Menu_item, which
 * must be suitably NULL-teminated
 */

#include "sms.h"

void menu(Menu_item *menu)
{
    while (1) {
        Menu_item *menu_it = menu;
        while (menu_it->chat != NULL) {
            printf("\t%s\n\n", menu_it->chat);
            ++menu_it;
        }

        unsigned int menu_num = menu_it - menu;
        printf("Please choose (1 - %u) : ", menu_num);
        unsigned int choice;
        scanf("%u", &choice);
        --choice;

        if (choice < menu_num)
            menu[choice].act();

    }
}
