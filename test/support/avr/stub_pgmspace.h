#ifndef CLIONTESTPROJECT_STUBPGMSPACE_H
#define CLIONTESTPROJECT_STUBPGMSPACE_H

#define PROGMEM /* empty */
#define pgm_read_byte(x) (*(x))
#define pgm_read_word(x) (*(x))
#define pgm_read_float(x) (*(x))
#define PGM_P const char *

#endif // CLIONTESTPROJECT_STUBPGMSPACE_H
