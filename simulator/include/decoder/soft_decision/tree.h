enum treeTypes
{
    TREE_NONE,
    TREE_CHASE
};

struct struct_treeStructure
{
    /*
     * Tree that is triple pointer indicate magnitudeShort.
     * And it is using when find minimum magnitudeShort.
     */
    enum treeTypes treeType;
    u_int_QUANTIZ_MAGNITUDE_DIGIT*** tree_mag;
    char*** tree_hd;
    unsigned char** direction;
    unsigned int** treeIndex;

    unsigned int* treeLength;
    unsigned int treeDepth;

}typedef struct_treeStructure;
