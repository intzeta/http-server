#include "update.h"

// TODO: format returned string to be better
// Also idk if static should be here but for now it will

static void dirwalk(char *dir, void (*fnp)(char *)); // pointer to fname
static void fname(const char *name); //name is path defined in UPDATE_H
