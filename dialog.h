#ifndef DIALOG_H
#define DIALOG_H

#include "tab.h"

Err dialogInput(Table **);
Err dialogGet(Table *);
Err dialogFindKey(Table *);
Err dialogImport(Table **);
Err dialogExport(Table *);
Err dialogFindVerse(Table *);
Err dialogGetRelease(Table *);

#endif
