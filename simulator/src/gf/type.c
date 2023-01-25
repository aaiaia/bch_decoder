void printPowerFormWithEnter(char_POWER_FORM *p)
{
    unsigned int length = strlen(p);
    unsigned int i;

    printf("power form polynomial length is %d\n", length);

    printf("\"");
    for(i=0; i<length; i++)
    {
        if(*(p+i)=='0')
        {
            printf("%c", *(p+i));
        }
        else if(*(p+i)=='1')
        {
            printf("%c", *(p+i));
        }
        else
        {
            errorMes;
            printf("%c\" power form polynomial coefficient values are must '0', '1'\n", *(p+i));
            return;
        }
    }
    printf("\"\n");

    for(i=0; i<length; i++)
    {
        if(*(p+i)=='0')
        {
            continue;
        }
        else if(*(p+i)=='1')
        {
            printf("a^%d\t", i);
        }
        else
        {
            errorMes;
            printf("power form polynomial coefficient values are must '0', '1'\n");
            return;
        }
    }
    printf("\n");
}
