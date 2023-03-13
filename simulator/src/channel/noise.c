#include <stdlib.h>
#include <math.h>

double gaussian(double mean, double var)
{
        const double PI = 3.14159265358979323846;
        double temp1, temp2, x1, x2;

        x1 = (rand() + 1);//x1에 0이 발생하지 않도록
        x2 = rand();//0이 발생해도 관계 없습니다.
        //x1 = (rand_r(&tmp_seed) + 1);//x1에 0이 발생하지 않도록
        //x2 = rand_r(&tmp_seed);//0이 발생해도 관계 없습니다.
        //        infoMes;
        //        printf("x1: %g,\tx2: %g,\t",x1,x2);

        x1 = x1 / (((double)RAND_MAX) + 1.0);
        //x1 = x1 / (RAND_MAX);
        x2 = x2 / ((double)RAND_MAX);
                //printf("x1: %g,\tx2: %g\n",x1,x2);
        temp1 = log(x1);
        temp1 = temp1 * (-2);
        temp1 = sqrt(temp1);//강의노트의 식. sqrt(-2*log(x1))

        temp2 = 2 * PI;
        temp2 = temp2 * x2;
        temp2 = cos(temp2);//강의노트의 식. cos(x2*2*PI)

        temp1 = temp1 * temp2;//위 두 식을 곱하여 N[0,1] 생성

        temp1 = (sqrt(var) * temp1);//adjust variance

        return temp1 + mean;//adjust mean
}
