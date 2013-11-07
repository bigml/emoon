#include "mheads.h"
#include "lheads.h"

NEOERR* lrend_init()
{
    NEOERR *err;
    
    err = mentity_load_dir(PATH_TPL"entity/", PATH_FLY);
    if (err != STATUS_OK) return nerr_pass(err);

    mrend_forwardrend_set_camera(NULL);
    
    return STATUS_OK;
}

NEOERR* lrend_update()
{
    return STATUS_OK;
}

NEOERR* lrend_rend()
{
    return STATUS_OK;
}

void lrend_finish()
{
    masset_finish();
    mentity_finish();
}
