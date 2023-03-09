const char cmdOtionKeyWords[] = "-{},:=";

enum cmdLineOptionInstructionType
{
    OPT_NOTHING,
    OPT_FLAG,
    OPT_FLAG_CASE,


    OPT_DEBUG,

    OPT_SET_VALUE,

    OPT_SET_VALUES,
    OPT_SET_VALUES_SERIES,//example) {-settingCategory=[value0],[value1],[value2],[value3]}

    OPT_SET_LANGE,//set lange, example) {-settingCategory=[start]:[step]:[end]}
    OPT_SET_LANGE_SERIES,

    OPT_STRING,
    OPT_COMPONENT,

    OPT_MANUAL,

    STRUCT_END
};

enum enum_list_valueType
{
    VALUE_TYPE_NONE,

    VALUE_TYPE_VOID,

    VALUE_TYPE_CHAR,
    VALUE_TYPE_UNSIGNED_CHAR,

    VALUE_TYPE_INT,
    VALUE_TYPE_UNSIGNED_INT,

    VALUE_TYPE_LONG,
    VALUE_TYPE_UNSIGNED_LONG,

    VALUE_TYPE_FLOAT,
    VALUE_TYPE_FLOAT_RATIO,
    VALUE_TYPE_FLOAT_RATIO_LIMIT,

    VALUE_TYPE_DOUBLE,
    VALUE_TYPE_DOUBLE_RATIO,
    VALUE_TYPE_DOUBLE_RATIO_LIMIT,

    VALUE_TYPE_STRING,
}typedef enum_list_valueType;

struct struct_cmdLineOption
{
    const char *longName;
    char shortName;

    enum cmdLineOptionInstructionType instType;
    unsigned char argument;
    unsigned char argumentNumber;

    void *value;
    unsigned int valueMask;

    enum enum_list_valueType valueType;

    struct struct_cmdLineOption *cmdAddedOption;
};

/* Displaying */
void printInstList(struct struct_cmdLineOption *p, unsigned int recursive, char *passedStrng);
