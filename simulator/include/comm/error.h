struct struct_errorComponent
{
    struct_powerFormPolynomials *erroredCodeWord;
    struct_powerFormPolynomials *errorLocationVector;
}typedef struct_errorComponent;

/* Constructor and Destructor */
struct_errorComponent *createErrorComponent(unsigned int length);
char closeErrorComponent(struct_errorComponent **p);
struct_errorComponent *recreateErrorComponent(struct_errorComponent **p, unsigned int length);

/* Operation */
struct_errorComponent *ifNotExistCreateErrorComponent(struct_errorComponent **p, unsigned int length);
void errorOccurationWhichAreRandomPosition(struct_powerFormPolynomials *p, unsigned int numberOfError);
