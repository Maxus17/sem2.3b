#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

#include "dialog.h"

int main()
{
	Table *table = NULL;
	IndexType msize = 0;
	
	int opt = -1;
	
	char c;

	Err code = ERR_OK;
	int inp = 2;

	while (opt != 0)
	{
		printf("\n");
		printf("****МЕНЮ****\n");
		printf("0) выход\n");
		printf("1) добавление элемента в таблицу по значению ключа\n");
		printf("2) удаление элемента по значиению ключа\n");
		printf("3) поиск по ключу\n");
		printf("4) вывод содержимого\n");	
		printf("5) импорт из бинарного файла\n");	
		printf("6) эксопрт в бинарный файла\n");
		printf("7) поиск по ключу и версии\n");
		printf("8) удаление версии\n");
		int ent = scanf("%d%c", &opt, &c);
		if (ent == EOF)
		{
			freeTab(table);
			return 0;
		}

		switch (opt)
		{
			case 0:
				printf("Выход...\n");
				break;
			case 1:
				if (!table)
				{
					while (inp == 2)
					{
						printf("Введите максимальное количество ячеек\n> ");
						inp = input(&msize);
						if (inp == EOF)
						{
							return ERR_EOF;
						}
					}
					inp = 2;
					
					table = createTable(msize);
				}
				
				code = dialogInput(&table);
				printErr(code);
				if (code == ERR_EOF)
				{
					freeTab(table);
					return 0;
				}
				break;
			case 2:
				code = dialogGet(table);
				printErr(code);
				if (code == ERR_EOF)
				{
					freeTab(table);
					return 0;
				}
				break;
			case 3:
				code = dialogFindKey(table);
				printErr(code);
				if (code == ERR_EOF)
				{
					freeTab(table);
					return 0;
				}
				break;
			case 4:
				code = printTab(table);
				printErr(code);
				break;
			case 5:
				code = dialogImport(&table);
				printErr(code);
				if (code == ERR_EOF)
				{
					freeTab(table);
					return 0;
				}
				break;
			case 6:
				code = dialogExport(table);
				printErr(code);
				if (code == ERR_EOF)
				{
					freeTab(table);
					return 0;
				}
				break;
			case 7:
				code = dialogFindVerse(table);
				printErr(code);
				if (code == ERR_EOF)
				{
					freeTab(table);
					return 0;
				}
				break;
			case 8:
				code = dialogGetRelease(table);
				printErr(code);
				if (code == ERR_EOF)
				{
					freeTab(table);
					return 0;
				}
				break;
			default:
				printf("Такой команды нет\n");
				break;				
		}
	}
	
	freeTab(table);
	return 0;
}
