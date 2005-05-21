/* new H definitions */
#ifndef HDEF_H
#define HDEF_H

#define H_ACTIVE_LIMIT (HDEF_h * 6)

#define H_ETERNITY 0xffffffff

enum
{
    HDEF_s = 1,
    HDEF_m = 60,
    HDEF_h = 60  * HDEF_m,
    HDEF_d = 24  * HDEF_h,
    HDEF_w = 7   * HDEF_d,
    HDEF_M = 30  * HDEF_w,
    HDEF_y = 365 * HDEF_d
};

enum
{
    HLAZY,
    HNOW
};

typedef enum
{
    H_OFF,
    H_ON
} hflagchange;

typedef enum
{
    H_LAMER,
    H_PEON,
    H_TRIAL,
    H_STAFF,
    H_OPER,
    H_ADMIN,
    H_SERVICE,
    H_NONE, /* _NOT_ a real level, just used for resetting */
    H_ANY /* for the count function, NOT a real level */
} hlevel;

const char *hlevel_name(hlevel);

#endif