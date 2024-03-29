#define F_CPU 16000000UL

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "lm75.h"
#include "i2cmaster.h"
#include "lcd.h"
#include "usart.h"
#include <avr/interrupt.h>

//Create dataset
//Training examples
const char M = 9;
//Features
const char N = 2;
//For the inputs 1/10, for the output 1/1000 scalar is used to avoid too big numbers
const float x_train[2][9] = {{4.6,5.2,6.3,4.6,8.9,1.2,12,3.1,8},{2.4,2.7,1.3,2.8,6,7,8,1,2}}; //size in m^2, time from centre in mins
const float y_train[] = {9.905,11,10.9,11.2,15,18,20,25,10}; //price

//Create functions we will need
float compute_cost(float theta_any[], float b_any);
float calc_pred(float theta[], float b, unsigned char example_number);
float calc_grad(unsigned char feature_number, float theta[], float b);
void grad_descent(float alpha, unsigned int iters, float *theta, float *b);

//Set count variable to accurately measure execution time
volatile int count = 0;


int main()
{
    uart_init();
    io_redirect();
    //enable interrupts
    sei();    
    // enable timer1 overflow interrupt
    TIMSK1 |= (1 << TOIE1);

    //prescaler to 1024 & start timer1 - reaching full = 4.194 seconds, as max value is 65535
    TCCR1B |= (1 << CS12) | (1 << CS10);


    printf("We have %d training examples and %d features.\n",M,N);

    //Test compute_cost function with arbitrary values
    float theta_guess[] = {1.42,1.44};
    float b_guess = 0.6;
    printf("Cost of our prediction: %f\n",compute_cost(theta_guess,b_guess));

    //Set parameters for gradient descent
    //learning rate
    float alpha_init = 0.014;
    //iterations
    unsigned int iters_init = 5000;
    //initilize parameters
    float theta_init[] = {0,0};
    float b_init = 0;
    //Apply gradient descent
    grad_descent(alpha_init, iters_init, theta_init, &b_init);

    //Print equation of predicted model & cost
    printf("Predicted model is: (%f, %f)*X + %f\n",theta_init[0],theta_init[1],b_init);
    float final_cost = compute_cost(theta_init,b_init);

    printf("Final cost of predicted model is: %f\n",final_cost);

    //Stop timer
    TCCR1B &= ~((1 << CS12) | (1 << CS10));
    //Show time 
    float time = (TCNT1+1)*1.024;
    time *= 1000;
    time /= F_CPU;
    time += count*4.194;
    printf("Operation time was %fs\n",time);

    //Create dynamic memory for used input
    float *your_data = malloc(sizeof(your_data));

    if(your_data != NULL){
        printf("Dynamic memory created.\n");

        printf("What is the size of the apartment?: ");
        scanf("%f", your_data);

        printf("\nHow long does it take to get into the centre?: ");
        scanf("%f", your_data+1);
        printf("\nModel takes in X= (%f, %f)\n",your_data[0],your_data[1]);

        float price_predicted = 0;
        price_predicted = (((your_data[0]/10) * theta_init[0]) + ((your_data[1]/10)*theta_init[1]) + b_init)*1000;
        printf("Predicted price of your apartment: %f DKK\n",price_predicted);
    }
    else{
        printf("Memory wasn't allocated. \n");
    }

    //Release space from heap to avoid memory leakage
    free(your_data);
    your_data = NULL;
    printf("Dynamic memory freed.\n");

    return 0;
}

//What happens when timer1 overflow event happens
ISR(TIMER1_OVF_vect){
    count++;
}

//Gradient descent function: finds optimum parameters
void grad_descent(float alpha, unsigned int iters, float *theta, float *b){

    //Create dynamic memory to create array to make arguments easier to pass
    float *theta_current = malloc(N*sizeof(theta_current));
    if(theta_current != NULL){
        printf("Dynamic memory allocated. \n");
        float current_gradient = 0;
        for(unsigned int i=0; i<iters;i++){ //For one iteration
            for(int f=0;f<N;f++){ //For one feature
                // Put thetas into an array
                for(int c=0;c<N;c++){
                    theta_current[c] = *(theta+c);
                }
                current_gradient = calc_grad(f,theta_current,*b);
                *(theta+f) = *(theta+f) - alpha*current_gradient;
                *b = *b - alpha*current_gradient;
            }
        }
        printf("Iterations complete.\n");
        free(theta_current);
        theta_current = NULL;
        printf("Dynamic memory freed.\n");
        
    }
    else{
        printf("Dynamic memory wasn't allocated for theta.\n");
        free(theta_current);
        theta_current = NULL;
    }
}

// Calculate gradient for given feature number
float calc_grad(unsigned char feature_number, float theta[], float b){
    float error = 0;
    float h_x = 0;
    float sum = 0;
    for(int a=0; a<M; a++){ //loop through training examples
        h_x = calc_pred(theta,b,a);
        error = h_x - y_train[a];
        sum += error * x_train[feature_number][a];
    }
    sum = sum/M;
    return sum;
}

// Calculate prediction for given training example number
float calc_pred(float theta[], float b, unsigned char example_number){
    float h_x = 0; //prediction itself
    for(int k=0; k < N;k++){ //loop through features
        h_x += theta[k]*x_train[k][example_number];
    }
    h_x += b;
    return h_x;
}

// Compute cost for given theta, b
float compute_cost(float theta_any[], float b_any){

    float total_cost = 0;
    float h_x = 0;
    float error = 0;

    for(int l=0;l<M;l++){
        h_x = calc_pred(theta_any,b_any,l);
        error = h_x - y_train[l];
        total_cost += (error*error);
    }

    total_cost = total_cost / (2*M);
    return total_cost;

}