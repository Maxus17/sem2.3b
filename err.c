#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "err.h"

void printErr(Err code)
{
	switch(code)
	{
		case ERR_OK:
			break;
		case ERR_MEM:
			printf("ошибка выделения памяти\n");
			break;
		case ERR_EMPTY:
			printf("пустая таблица\n");
			break;
		case ERR_FORMAT:
			printf("неверный формат записи\n");
			break;
		case ERR_EOF:
			printf("EOF - вы нажали ctrl + D\n");
			break;
		case ERR_ELEMENT:
			printf("Элемент не найден\n");
			break;
		case ERR_FILE:
			printf("ошибка чтения файла\n");
			break;
		case ERR_FILE_NAME:
			printf("введено ошибочное название файла\n");
			break;
	}
}
