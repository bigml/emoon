#include "mheads.h"
#include "lheads.h"

int LERR_NOTLOGIN = 0;              /* 14 */

static int lerrInited = 0;

NEOERR* lerr_init()
{
    NEOERR *err;

    if (lerrInited == 0) {
        err = nerr_init();
        if (err != STATUS_OK) return nerr_pass(err);

        err = nerr_register(&LERR_NOTLOGIN, "请登录后操作");
        if (err != STATUS_OK) return nerr_pass(err);

        lerrInited = 1;
    }

    return STATUS_OK;
}
