/*
 * viki 主脑
 */

#include "mheads.h"
#include "lheads.h"

HDF  *g_cfg = NULL;
HASH *g_datah = NULL;

int main(int argc, char **argv)
{
    NEOERR *err;
    
    msys_makesure_coredump();

    err = mcfg_parse_file(APPCONFIG, &g_cfg);
    OUTPUT_NOK(err);

    err = lerr_init();
    DIE_NOK(err);
    
    mtc_init(hdf_get_value(g_cfg, PRE_CONFIG".trace_file", "viki"),
             hdf_get_int_value(g_cfg, PRE_CONFIG".trace_level", TC_DEFAULT_LEVEL));

    mcfg_cleanup(&g_cfg);

    return 0;
}
