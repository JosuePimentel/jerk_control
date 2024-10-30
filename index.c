#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct
{
    
    float T1, T2, T3, T4, T5, T6, T7;
    float TA1, TA2, TA3, TA4, TA5, TA6, TA7;

} tempos_t;

tempos_t calcular_tempos ( float Jmax, float Amax, float Vm, float S);
void imprimir_tempos (tempos_t *tempos, float dt, float Jmax, float Amax, float Vm, float S);

int main() {
    system('cls');

    float Jmax = 2.0f, Amax = 4.0f, Vm = 10.0f, S = 100.0f;
    
    tempos_t T = calcular_tempos( Jmax, Amax, Vm, S);
    
    printf("T1: %.2f\nT2: %.2f\nT3: %.2f\nT4: %.2f\nT5: %.2f\nT6: %.2f\nT7: %.2f\n", T.T1 , T.T2 , T.T3 , T.T4 , T.T5 , T.T6 , T.T7);

    int n_pontos = 1000;

    imprimir_tempos(&T, (T.TA7/n_pontos), Jmax, Amax, Vm, S);
}

void imprimir_tempos (tempos_t *tempos, float dt, float Jmax, float Amax, float Vm, float S) {
    FILE *arquivo = fopen("resultados.csv", "w");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    float t_end = tempos->TA7, t = 0.0f;
    float s = 0, v = 0, a = 0, j = 0;

    float VS = 0.0f,
          V1 = (Jmax*powf(tempos->TA1, 2))/2 + VS,
          V2 = Amax*(tempos->TA2 - tempos->TA1) + V1,
          V3 = Amax*(tempos->TA3 - tempos->TA2) - (Jmax*powf((tempos->TA3 - tempos->TA2), 2))/2 + V2,
          V4 = V3,
          V5 = (Jmax*powf((tempos->TA5 - tempos->TA4), 2))/2 + V4,
          V6 = (-Amax*(tempos->TA6 - tempos->TA6)) + V5;

    while( t < t_end )
    {
        if( t < tempos->TA1 ) {
            j = Jmax;
            a = Jmax*t;
            v = (Jmax*powf(t,2))/2 + VS;
        } else if( (tempos->TA1 > t) && ( t < tempos->TA2) ) {
            j = 0;
            a = Amax;
            v = Amax*(t-tempos->TA1) + V1;
        } else if( (tempos->TA2 > t) && ( t < tempos->TA3) ) {
            j = (-Jmax);
            a = (-Jmax)*(t-tempos->TA2)+Amax;
            v = Amax*(t-tempos->TA2)-(Jmax*powf((t-tempos->TA2),2))/2 + V2;
        } else if( (tempos->TA3 > t) && ( t < tempos->TA4) ) {
            j = 0;
            a = 0;
            v = V3;
        } else if( (tempos->TA4 > t) && ( t < tempos->TA5) ) {
            j = (-Jmax);
            a = (-Jmax)*(t-tempos->TA4);
            v = (-Jmax*powf((t-tempos->TA4), 2))/2 + V4;
        } else if( (tempos->TA5 > t) && ( t < tempos->TA6) ) {
            j = 0;
            a = (-Amax);
            v = (-Amax*(t-tempos->TA5)) + V5;
        } else if( (tempos->TA7 > t) && ( t < tempos->TA7) ) {
            j = Jmax;
            a = Jmax*(t-tempos->TA6)-Amax;
            v = (-Amax*(t-tempos->TA6)) + (Jmax*powf((t-tempos->TA6), 2))/2 + V6;
        }

        fprintf( arquivo, "%.2f,%.2f,%.2f,%.2f,%.2f\n", t, s, v, a, j );
        printf( "T: %.2f, S: %.2f, V: %.2f, A: %.2f, J: %.2f\n", t, s, v, a, j );

        t += dt;
    }

    fclose(arquivo);
}

tempos_t calcular_tempos ( float Jmax, float Amax, float Vmax, float S) {
    tempos_t T;

    float T1_Temp, T2_Temp, T3_Temp;
    float V1, V2, V3;
    float A1, A2, A3;
    float S1, S2, S3;

    // Início
    A1 = Amax;
    T1_Temp = Amax/Jmax;
    V1 = (Jmax * powf(T1_Temp, 2))/2;

    if(V1 < Vmax/2) {
        V2 = Vmax - V1;
        // A2 = (V2 - V1)/(T2_Temp - T1_Temp); // (T2 - T1) mas t2 ainda nao foi descoberto
        A2 = Amax; // Suposição que faz sentido: Se há o segmento II, então no segmento 2 a aceleração se mantem constante no Amax, como é possível ver no gráfico da aceleração PG.20
        T2_Temp = (Vmax - 2*V1)/A2;
        T3_Temp = T1_Temp;
    } else {
        T1_Temp = sqrtf((Vmax/Jmax));
        T2_Temp = 0;
        T3_Temp = T1_Temp;
    }

    V3 = Vmax;

    S1 = V1*T1_Temp;
    S2 = T2_Temp ? (V2-V1)*(T2_Temp-T1_Temp) : 0;
    S3 = T2_Temp ? (V3-V2)*(T3_Temp-T2_Temp) : (V3-V1)*(T3_Temp-T1_Temp);

    if((S1 + S3) > S/2) {
        T1_Temp = powf(((0.5*S)/Jmax), 1.0/3.0); // Raiz cubica, o que esta no sol vai pra sombra e oq esta na sombra vai pro sol.
        T2_Temp = 0;
        T3_Temp = T1_Temp;
    } else {
        T2_Temp = (1/(2*Jmax*T1_Temp))*(-3*Jmax*powf(T1_Temp, 2) + sqrtf(powf(Jmax, 2)*powf(T1_Temp, 4) + 4*Jmax*T1_Temp*S));
    }

    // Tempos relativos
    T.T1 = T1_Temp;
    T.T2 = T2_Temp;
    T.T3 = T3_Temp;
    T.T4 = (S-2*(S1+S2+S3))/V3;
    T.T5 = T1_Temp;
    T.T6 = T2_Temp;
    T.T7 = T1_Temp;

    // Tempos absolutos;
    T.TA1 = T.T1;
    T.TA2 = T.T2 + T.TA1;
    T.TA3 = T.T3 + T.TA2;
    T.TA4 = T.T4 + T.TA3;
    T.TA5 = T.T5 + T.TA4;
    T.TA6 = T.T6 + T.TA5;
    T.TA7 = T.T7 + T.TA6;

    return T;
}