signed long intPower(int radix, int exponent)
{
    signed long buffer = 1;
    for(;exponent;exponent--)
    {
        buffer*=radix;
    }
    return buffer;
}
